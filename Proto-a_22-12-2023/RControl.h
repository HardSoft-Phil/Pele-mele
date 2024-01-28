//==================================================================
//  Application de la bibliothèque RC_Lib à l'origine pour Libellule
//  à base d'atmega328 adaptée  à l'ESP32
//==================================================================
#ifndef _RadioControl_h
#define _RadioControl_h

#include "RC_Lib.h"

//RC Interupts
RC_Lib Throttle(THROTTLE_PIN, MIN_THROTTLE, MAX_THROTTLE, THROTTLE_TRIM);
RC_Lib Yaw(YAW_PIN, MIN_YAW, MAX_YAW, YAW_TRIM);
RC_Lib Roll(ROLL_PIN, MIN_ROLL, MAX_ROLL, ROLL_TRIM);
RC_Lib Pitch(PITCH_PIN, MIN_PITCH, MAX_PITCH, PITCH_TRIM);
RC_Lib Aux1(AUX1_PIN, MIN_AUX1, MAX_AUX1, 0);
RC_Lib Aux2(AUX2_PIN, MIN_AUX2, MAX_AUX2, 0);

//#####################################################
void IRAM_ATTR calcThrottle() 
{
  Throttle.calcPin();
}

//#####################################################
void IRAM_ATTR calcYaw() 
{
  Yaw.calcPin();
}

//#####################################################
void IRAM_ATTR calcRoll() 
{
  Roll.calcPin();
}

//#####################################################
void IRAM_ATTR calcPitch() 
{
  Pitch.calcPin();
}

//#####################################################
void IRAM_ATTR calcAux1() 
{
  Aux1.calcPin();
}

//#####################################################
void IRAM_ATTR calcAux2() 
{
  Aux2.calcPin();
}

//#####################################################
void RCSetup() {
  pinMode(ROLL_PIN, INPUT_PULLDOWN);
  pinMode(PITCH_PIN, INPUT_PULLDOWN);
  pinMode(THROTTLE_PIN, INPUT_PULLDOWN);
  pinMode(YAW_PIN, INPUT_PULLDOWN);
  pinMode(AUX1_PIN, INPUT_PULLDOWN);
  pinMode(AUX2_PIN, INPUT_PULLDOWN);
  
  attachInterrupt(ROLL_PIN, calcRoll, CHANGE);
  attachInterrupt(PITCH_PIN, calcPitch, CHANGE);
  attachInterrupt(THROTTLE_PIN, calcThrottle, CHANGE);
  attachInterrupt(YAW_PIN, calcYaw, CHANGE);
  attachInterrupt(AUX1_PIN, calcAux1, CHANGE);
  attachInterrupt(AUX2_PIN, calcAux2, CHANGE);
};

//#####################################################
void RCLoop() {
  // Vérifie si on reçoit des données de l'émetteur
  Roll.copyValue();
  Pitch.copyValue();
  Throttle.copyValue();
  Yaw.copyValue();
  Aux1.copyValue();
  Aux2.copyValue();
}

//#####################################################
// Vérifie s'il y a une connection avec l'émetteur
bool checkRemote()
{
  if (Throttle.getValue() < 950) // On teste juste la réception de la vitesse
    return false;
  else return true;
}
//{
//  uint32_t now = micros();
//  return now - Throttle.getSignalTime() < SIGNAL_TIMEOUT
//      && now - Yaw.getSignalTime() < SIGNAL_TIMEOUT 
//      && now - Roll.getSignalTime() < SIGNAL_TIMEOUT 
//      && now - Pitch.getSignalTime() < SIGNAL_TIMEOUT;
//}

#endif // _RadioControl_h

//#####################################################
