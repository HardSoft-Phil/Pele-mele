// ################################################################
// ####         Programme contrôleur de vol prototype          ####
// ####     Version pour ESP32 sans moteurs pour le moment     ####
// ################################################################
// ####---------- Configuration Carte QuaTri-2_ESP ------------####
// ####--------------------------------------------------------####
// #### * Carte DOIT ESP32 DEVKIT V1                           ####
// #### * module GY 9250:                                      ####
// ####   - accéléromètre / gyroscope (MPU9250)                ####
// ####   - magnétomètre (AK8963)                              ####
// #### * 4 sorties PWM pour moteur ou servo                   ####
// ################################################################
//  Base: Quatri-Servo-29-11-2023 
//        (algorithme PID:https://github.com/davidkim9/Tricopter)
//------------------------------------------------------------------
//  22/12/2023:
//  -> Valeurs radio brutes sinon inréglable sur le terrain.
//      Ajustement avec les trims de la radiocommande
//  -> Mode stable ok, mode accro pas bon
//------------------------------------------------------------------

#include <Wire.h>
//#include <EEPROM.h>
//#include <ESP32Servo.h>

// Ne pas changer l'ordre des includes locaux
#include "Params.h"   // Paramètres globaux
#include "MPU_9250.h" // Sans interruptions
#include "RControl.h" // Gestion radiocommande
#include "Process.h"  // Le controleur proprement dit

//#######################################################
//####                     SETUP()                   ####
//#######################################################
char *version = "Proto-version du 22/12/2023";
//=======================================================
void setup()
{
  Serial.begin(115200); Serial.println();
  delay(250);
  Serial.print("\nVersion : "); Serial.println(version); Serial.println();

  Wire.begin();
  Wire.setClock(400000); // Horloge I2C à 400kHz

  //  initMoteurs(); // Critique sinon les moteurs ne démarrent pas tous !

  pinMode(LEDV_PIN, OUTPUT);
  pinMode(LEDR_PIN, OUTPUT);
  pinMode(CAM_PIN, OUTPUT);

  digitalWrite(LEDV_PIN, HIGH);
  digitalWrite(LEDR_PIN, HIGH);
  digitalWrite(CAM_PIN, LOW);

  RCSetup();      // Configuration Radio-Commande

  gyroSetup();    // Initialisation du MPU-9250

  controleurSetup();

  etat = ARRET;

  stableMode = true; // Mode stable par défaut
}

//####################################################
//####                    LOOP()                  ####
//####################################################
void loop()
{
  currentTime = micros();

  RCLoop();         // Scrutation radio-commande
  processRC();      // Gère l'état du drone
 
  MPULoop();        // Acquisition gyroscope

  controllerLoop(); // Calculs de stabilité

  moniteurDebug();  // Affichage (option pour debug)

  motorsLoop();     // Pilotage des moteurs
}

//####################################################
