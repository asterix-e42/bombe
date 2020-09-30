

char aff_mess = 1;
String str = "";

void mess(const char *s)
{
  Serial.println(s);
  message(s, "", "", "");
}

void message(const char *s0, const char *s1, const char *s2, const char *s3)
{
  //lcd.clear();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(s0);
  lcd.setCursor(0, 1);
  lcd.print(s1);
  lcd.setCursor(0, 2);
  lcd.print(s2);
  lcd.setCursor(0, 3);
  lcd.print(s3);
  aff_mess = 0;
}
