void recept()
{

  static char minu = Wire.read();
  static char sec = Wire.read();
  Wire.requestFrom(I2C_SLAVE1_BOMBE, 20);
  char c = Wire.read();
  switch (c)
  {
    case -1 :
      {
        Serial.println("???");
        delay(1000);
      } break;
    case 1 :
      {
        minu = Wire.read();
        sec = Wire.read();
        matrix.print(minu * 100 + sec, DEC);
        matrix.writeDisplay();
      } break;
    case 3 :
      {
        win(minu, sec);
      } break;
    case 4 :
      effet_mess_stress();
    default :
      {
        Serial.println("sec");
        String s = "";
        while (Wire.available())
          s += (char)Wire.read();
        mess(s.c_str());
      }

  }
}
