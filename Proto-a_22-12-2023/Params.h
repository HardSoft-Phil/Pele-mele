//////////////////////////////////////////////////////////////////////////////////////
//                               Carte QuaTri V2                                    //
//////////////////////////////////////////////////////////////////////////////////////
// Cablage du Gyroscope MPU9250, du baromètre BMP180 et du LCD optionnel:
//                         +5V --> VCC
//                         GND --> GND
//                     GPIO 21 --> SDA
//                     GPIO 22 --> SCL
//                     GPIO 19 --> INT
//
// Cablage des ESC (+5V non connecté et GND de l'ESP32):
//                     GPIO 27 --> Moteur Arrière Droit  (sens horaire)
//                     GPIO 14 --> Moteur Avant Droit    (sens anti-horaire)
//                     GPIO 12 --> Moteur Arrière Gauche (sens anti-horaire)
//                     GPIO 13 --> Moteur Avant Gauche   (sens horaire)
//
// Le récepteur de Radio-Commande est alimenté (+5V et GND) par l'ESP32
// Cablage des canaux de réception RC :
//                    Roll ------> GPIO 36
//                    Pitch -----> GPIO 39
//                    Throttle --> GPIO 34
//                    Yaw -------> GPIO 35
//                    Aux1 ------> GPIO 32 (mode acc ou stable)
//                    Aux2 ------> GPIO 33 (accessoire - CAM_PIN)
//
// Cablage des Leds :
//                    LED rouge -> GPIO 25
//                    LED verte -> GPIO 26
//
// Sortie accessoire (CAM_PIN) --> GPIO 02
//
//////////////////////////////////////////////////////////////////////////////////////

#define DEBUG // Si non commenté -> affichage, sinon affichage inactif
int debugvalue = 0;

//----------------------------
// Pins d'accessoires
#define INTERRUPT_PIN_MPU 4//19 // Interruption MPU
#define LEDR_PIN 25
#define LEDV_PIN 26
#define CAM_PIN 2

uint32_t RledTimer = 0;
bool RblinkState = false;
uint32_t VledTimer = 0;
bool VblinkState = false;
bool camMode = 0;

//---------MOTEURS-------------
int frontLeftValue, frontRightValue, rearLeftValue, rearRightValue;

//----------------------------
// Pins de connexion télécommande
#define ROLL_PIN      36
#define PITCH_PIN     39
#define THROTTLE_PIN  34
#define YAW_PIN       35
#define AUX1_PIN      32
#define AUX2_PIN      33

//RC Ajustement valeurs reçues
#define ROLL_TRIM 0//-21
#define PITCH_TRIM 0//-21
#define THROTTLE_TRIM 0//-21
#define YAW_TRIM 0

// Exponentielle x^EXPO
#define EXPO_ROLL_PITCH 1.0
#define EXPO_YAW 1.0

// Signal timeout RF: 1.5 seconde
#define SIGNAL_TIMEOUT 1500000


//----------------------------
// Constantes PWM
#define MINCOMMAND 1000
#define MIDCOMMAND 1500
#define MAXCOMMAND 2000
#define MINTHRUST 1050

// Points milieu commandes moteur autour de 1500
#define MIN_THROTTLE 1000
#define MAX_THROTTLE 1900
#define MIN_YAW 1100
#define MAX_YAW 1900
#define MIN_ROLL 1100
#define MAX_ROLL 1900
#define MIN_PITCH 1100
#define MAX_PITCH 1900

#define MIN_AUX1 1000
#define MAX_AUX1 2000
#define MIN_AUX2 1000
#define MAX_AUX2 2000

//----------------------------
// Coefficients PID
float ROLL_PITCH_P = 1.4;
float ROLL_PITCH_I = 0.8;
float ROLL_PITCH_D = 4.0;
float ROLL_PITCH_P_GUARD = 500;
float ROLL_PITCH_I_GUARD = 200;

float YAW_P = 1.4; //6.0;
float YAW_I = 0.8; //0.1;
float YAW_D = 4.0; //0.0;
float YAW_P_GUARD = 500;
float YAW_I_GUARD = 200;

/////////////////////////////////////////////////////////
byte etat = 0; // variable d'état du drone

#define ARRET   0   // ARRET équivaut à 0
#define ARME    1   // ARME équivaut à 1
#define MARCHE  2   // MARCHE équivaut à 2

bool stableMode = 0;

double targetThrottle, targetYaw, targetPitch, targetRoll;
double outputRollStable, outputPitchStable, outputYawStable;
double outputRoll, outputPitch, outputYaw;

bool signalPresent;

int throttleValue = 0;

uint32_t armTime;
unsigned long currentTime = 0;
unsigned long lastDethrottleTime = 0;

/////////////////////////////////////////////////////////
