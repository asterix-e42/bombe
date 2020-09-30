

void effet_LED_1()
{
  uint16_t affichage;

  affichage = 123;
  int t = affichage;
  while (affichage)
  {

    Serial.print(affichage);
    Serial.print(" ");
    Serial.print(t - affichage);
    t = affichage;
    Serial.print(" ");
    Serial.println(affichage + 1111);
    if ((affichage += 1111) > 7000)
      affichage = 0;// je comprend pas 0000, qui est sense etre 0 en octal
    Serial.print(affichage);
    matrix.print(affichage, DEC);
    matrix.writeDisplay();
    delay(300);
  }
}


uint8_t counter = 0;
void effet_LED_2()
{
  // paint one LED per row. The HT16K33 internal memory looks like
  // a 8x16 bit matrix (8 rows, 16 columns)
  for (uint8_t i = 0; i < 8; i++) {
    // draw a diagonal row of pixels
    matrix.displaybuffer[i] = _BV((counter + i) % 16) | _BV((counter + i + 8) % 16)  ;
  }
  // write the changes we just made to the display
  matrix.writeDisplay();
  delay(100);

  counter++;
  if (counter >= 16) counter = 0;
}


void effet_LED_3()
{
  static int8_t i = 0;
  i++;
  if (i == 8)
    i = 0;
  matrix.writeDigitRaw(0, 1 << i);
  matrix.writeDigitRaw(1, 1 << i + 1);
  matrix.writeDigitRaw(3, 1 << i + 2);
  matrix.writeDigitRaw(4, 1 << i + 3);
  matrix.writeDisplay();
}


void effet_LED_4()
{
  static int8_t i = 0;
  i++;
  if (i == 0x100)
    i = 0;
  matrix.writeDigitRaw(0, i);
  matrix.writeDigitRaw(1, i + 1);
  matrix.writeDigitRaw(3, i + 2);
  matrix.writeDigitRaw(4, i + 3);
  matrix.writeDisplay();
}


void effet_LED_5()
{
  matrix.writeDigitRaw(0, rand());
  matrix.writeDigitRaw(1, rand());
  matrix.writeDigitRaw(3, rand());
  matrix.writeDigitRaw(4, rand());
  matrix.writeDisplay();
}

void effet_clear()
{  
  matrix.writeDigitRaw(0, 0);
  matrix.writeDigitRaw(1, 0);
  matrix.writeDigitRaw(3, 0);
  matrix.writeDigitRaw(4, 0);
  matrix.writeDisplay();
}

void   effet_message()
{
  int i;
  mess("bombe detecte");
  for (i = 0; i < 30; i++)
  {
    effet_LED_3();
    delay(100);
  }
  mess("scan peripherique :");
  for (i = 0; i < 20; i++)
  {
    effet_LED_3();
    delay(100);
  }
  lcd.setCursor(0, 1);
  lcd.print("protocole HTCPCP");
  for (i = 0; i < 20; i++)
  {
    effet_LED_3();
    delay(100);
  }
  lcd.setCursor(0, 2);
  lcd.print("protocole i2c");

  for (i = 0; i < 20; i++)
  {
    effet_LED_3();
    delay(100);
  }
  message("i2c detecte", "tentative intrusion", "", "");
  for (i = 0; i < 30; i++)
  {
    effet_LED_3();
    delay(100);
  }
  message("echec d'intrusion", "erreur 418", "", "");

  for (i = 0; i < 31; i++)
  {
    effet_LED_3();
    delay(100);
  }
  message("saisir code systeme", "* : reset", "# : valider", "");
effet_clear();
}
