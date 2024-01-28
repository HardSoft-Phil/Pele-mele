//#####################################################
// void motorsLoop()
// Boucle de gestion des moteurs
//  - pas de paramètres d'entrée
//  - pas de valeur retournée
//#####################################################
void motorsLoop()
{
  if (etat == MARCHE)
  {
    if (signalPresent)
    {
      throttleValue = Throttle.getScaledValue();
    } else {
      // Décrémente la vitesse toutes les 100 ms
      if (currentTime - lastDethrottleTime > 100000)
      {
      throttleValue = throttleValue - 1;
        lastDethrottleTime - currentTime;
        // Arrete les moteurs mais reste armé
        if (throttleValue == 1000) 
        {
          etat = ARME;
          armTime = currentTime;
        }
      }
    }
    //---------------------------
    // mixage des commandes PID
    //---------------------------
    frontLeftValue  = throttleValue + outputRoll - outputPitch + outputYaw;
    rearRightValue  = throttleValue - outputRoll + outputPitch - outputYaw;
    frontRightValue = throttleValue - outputRoll + outputPitch - outputYaw;
    rearLeftValue   = throttleValue + outputRoll - outputPitch + outputYaw;

    //Make sure the motors dont turn off in flight and PWM size isnt greater than 2000
    frontLeftValue  = constrain(frontLeftValue, MINTHRUST, MAXCOMMAND);
    frontRightValue = constrain(frontRightValue, MINTHRUST, MAXCOMMAND);
    rearRightValue  = constrain(rearRightValue, MINTHRUST, MAXCOMMAND);
    rearLeftValue   = constrain(frontRightValue, MINTHRUST, MAXCOMMAND);
  }
}
//#####################################################
