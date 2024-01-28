//##########################################################
// Gestion led rouge, clignotement
//  - paramètre d'entrée: uint32_t frequence
//  - pas de valeur retournée
//##########################################################
void ledRBlink(uint32_t frequence)
{
  if ((millis() - RledTimer) >= frequence)
  {
    RledTimer = millis();
    RblinkState = !RblinkState;
    digitalWrite(LEDR_PIN, RblinkState);
  }
}

//##########################################################
// Gestion led verte, clignotement
//  - paramètre d'entrée: uint32_t frequence
//  - pas de valeur retournée
//##########################################################
void ledVBlink(uint32_t frequence)
{
  if ((millis() - VledTimer) >= frequence)
  {
    VledTimer = millis();
    VblinkState = !VblinkState;
    digitalWrite(LEDV_PIN, VblinkState);
  }
}

//##########################################################
int readsernum()
{
  int num;
  char numStr[3];
  numStr[0] = Serial.read();
  numStr[1] = Serial.read();
  return atol(numStr);
}

//##########################################################
//   Make sure that given value is not over min_value/max_value range.
//
//   @param float value     : The value to convert
//   @param float min_value : The min value
//   @param float max_value : The max value
//
//   @return float
//##########################################################
float minMax(float value, float min_value, float max_value) {
  if (value > max_value) value = max_value;
  else if (value < min_value) value = min_value;

  return value;
}

//##########################################################
//  Process RadioCommande
//  Changement de l'état du drone
//  - pas de paramètre d'entrée
//  - aucune valeur retournée
//##########################################################
void processRC()
{
  // Quand le stick de gauche est positionné dans le coin inférieur droit
  if (etat == ARRET && Throttle.getScaledValue() < 1050 && Yaw.getScaledValue() < 1050)
  {
    etat = ARME;
  }

  // Quand le stick de gauche revient au centre
  if (etat == ARME && Yaw.getScaledValue() >= 1450 && Throttle.getScaledValue() < 1050)
  {
    etat = MARCHE;
    // Ré-initialise les variables du PID pour éviter un démarrage chaotique
    controleurSetup();
  }

  // Quand le stick de gauche est positionné dans le coin inférieur gauche
  if (etat == MARCHE && Yaw.getScaledValue() >= 1950 && Throttle.getScaledValue() < 1050)
  {
    etat = ARRET;
  }

  // Regarde si le signal radio est present
  signalPresent = checkRemote();

  //  Codes de signalisation lumineuse
  if (etat == ARRET)
  {
    ledVBlink(750); // Led verte clignote lentement
    ledRBlink(750); // Led rouge clignote lentement
  }
  if (etat == MARCHE)
  {
    ledVBlink(100); // Led verte clignote rapidement
    ledRBlink(100); // Led rouge clignote rapidement
  }
}

//##########################################################
void moniteurDebug (void)
{
#ifdef DEBUG  // parser part
  if (Serial.available())
  {
    char ch = Serial.read();
    // Perform ACC calibration
    if (ch == 'A')
    {
      Serial.println("\nFaire l'étalonnage Acc");
      //      calibreAccelGyro();
      //ACC_Store();
      Serial.println("Étalonnage Acc Terminé\n");
    }
    else if (ch == 'M')
    {
      Serial.println("\nFaire l'étalonnage Mag");
      //      calibreMag();
      //ACC_Store();
      Serial.println("Étalonnage Mag Terminé\n");
    }
    else if (ch == 'R')
    {
      Serial.print("\nPID Accro :  ");
      Serial.print(ROLL_PITCH_P);   Serial.print("  ");
      Serial.print(ROLL_PITCH_I);   Serial.print("  ");
      Serial.print(ROLL_PITCH_D);   Serial.println();
      Serial.print("PID Stable : ");
      Serial.print(YAW_P);   Serial.print("  ");
      Serial.print(YAW_I);   Serial.print("  ");
      Serial.print(YAW_D);   Serial.println("\n");
    }
    else if (ch == 'D')
    {
      Serial.println("Charge le PID par défaut");
      ROLL_PITCH_P = 1.4;
      ROLL_PITCH_I = 0.8;
      ROLL_PITCH_D = -4.0;
      ROLL_PITCH_P_GUARD = 500;
      ROLL_PITCH_I_GUARD = 200;

      YAW_P = 6.0;
      YAW_I = 0.1;
      YAW_D = 0.0;

      controleurSetup();
      //PID_Store();
    }
// *** Voir Proto-b_25-12-2024 ***
//    else if (ch == 'W')
//    {
//      char ch =   Serial.read();
//      char n =   Serial.available();
//      if (n == 2)
//      {
//        n = Serial.read();
//        if (ch == 'p')
//        {
//          if (n == '+') ROLL_PITCH_P += 0.01;
//          if (n == '-') ROLL_PITCH_P -= 0.01;
//          Serial.print("pid P = ");
//          Serial.println(ROLL_PITCH_P);
//        }
//        else if (ch == 'i')
//        {
//          n = Serial.read();
//          if (ch == 'i')
//          {
//            if (n == '+') ROLL_PITCH_I += 0.01;
//            if (n == '-') ROLL_PITCH_I -= 0.01;
//            Serial.print("pid I = ");
//            Serial.println(ROLL_PITCH_I);
//          }
//          else if (ch == 'd')
//          {
//            n = Serial.read();
//            if (ch == 'd')
//            {
//              if (n == '+') ROLL_PITCH_D += 0.01;
//              if (n == '-') ROLL_PITCH_D -= 0.01;
//              Serial.print("pid D = ");
//              Serial.println(ROLL_PITCH_D);
//            }
//            else if (ch == 'P')
//            {
//              n = Serial.read();
//              if (ch == 'p')
//              {
//                if (n == '+') YAW_P += 0.01;
//                if (n == '-') YAW_P -= 0.01;
//                Serial.print("yaw P = ");
//                Serial.println(YAW_P);
//              }
//              else if (ch == 'I')
//              {
//                n = Serial.read();
//                if (ch == 'i')
//                {
//                  if (n == '+') YAW_I += 0.01;
//                  if (n == '-') YAW_I -= 0.01;
//                  Serial.print("yaw I = ");
//                  Serial.println(YAW_I);
//                }
//                else if (ch == 'D')
//                {
//                  n = Serial.read();
//                  if (ch == 'd')
//                  {
//                    if (n == '+') YAW_D += 0.01;
//                    if (n == '-') YAW_D -= 0.01;
//                    Serial.print("yaw D = ");
//                    Serial.println(YAW_D);
//                  }
//                  else   Serial.println("Commande inconnue");
//                }
//              }
//            }
//          }
//        }
//      }
//      else if (ch == 'S')
//      {
//        //          PID_Store();
//        Serial.println("\nPID sauvé en EEPROM");
//        controleurSetup();
//      }
//      else
//      {
//        Serial.println("\nMauvais format d'entrée");
//        Serial.println("Wp±, Wi±, Wd± - modifie PID stable, exemple: Wd+");
//        Serial.println("WP±, WI±, WD± - modifie PID accro , exemple: WD-\n");
//      }
//    }

    else if (ch >= '0' && ch <= '9') debugvalue = ch - '0';
    if (ch == 'h') // Affichage de l'aide
    {
      Serial.println("A - Etalonnage Acc");
      Serial.println("M - Etalonnage Mag");
      Serial.println("D - Ecrit le PID par défaut");
      Serial.println("R - Valeurs PID actuel");
      Serial.println("Wpxx, Wixx, Wdxx - Ecrit PID accro");
      Serial.println("WPxx, WIxx, WDxx - Ecrit PID stable");
      Serial.println("WS - Sauve PID en EEPROM");
      Serial.println("\nAffiche les données:");
      Serial.println("0 - off");
      Serial.println("1 - Valeurs Gyro");
      Serial.println("2 - Valeurs Acc");
      Serial.println("3 - Valeurs Angle");
      Serial.println("4 - Valeurs RC");
      Serial.println("5 - Etat radio");
      Serial.println("6 - Valeurs Moteurs");
    }
  }

  if      (debugvalue == 1)   Serial.printf("%4d %4d %4d \n", gx, gy, gz);
  else if (debugvalue == 2)   Serial.printf("%2d %2d %2d \n", yprAngle[0], yprAngle[1], yprAngle[2]);
//  else if (debugvalue == 3)   Serial.printf("%2d %2d %2d \n", Roll, Pitch, Yaw);
  else if (debugvalue == 4)   Serial.printf("%4d %4d %4d %4d \n", Throttle.getScaledValue(), Yaw.getScaledValue(), Roll.getScaledValue(), Pitch.getScaledValue());
  else if (debugvalue == 5)
  {
    Serial.print(signalPresent ? "Radio connectée\t" : "Radio NON connectée\t");
    if (etat == 0)   Serial.print(F("Arret\t"));
    if (etat == 1)   Serial.print(F("Armé\t"));
    if (etat == 2)   Serial.print(F("Marche\t"));
    Serial.print(stableMode ? "Mode stable\t" : "Mode manuel\t");
    Serial.println(camMode ? "Cam On" : "Cam Off");
  }
  else if (debugvalue == 6)
  {
    Serial.print("AvG,"); Serial.print("ArD,"); Serial.print("AvD,"); Serial.println("ArG");
    Serial.printf("%4d %4d %4d %4d \n", frontLeftValue, rearRightValue, frontRightValue, rearLeftValue);
  }
  else if (debugvalue == 7)
  {
    Serial.print("Roll,"); Serial.print("Pitch,"); Serial.println("Yaw");
    Serial.printf("%4d %4d %4d \n", outputRoll, outputPitch, outputYaw);
  }
  //  else if (debugvalue == 8) Serial.println(altitude,2);
  //  else if (debugvalue == 9) Serial.println(mpu.temperature(),2);

#endif
}

//##########################################################
