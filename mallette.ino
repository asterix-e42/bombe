
//---- Librairie pour ecran LED---
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
Adafruit_7segment matrix = Adafruit_7segment();
#include <HT16K33.h>
#include <LiquidCrystal_I2C.h>
#include <mpr121.h>
#include "mallette.h"
#include <SPI.h> // SPI

#define SS_PIN 10
#define RST_PIN 9
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

# define I2C_SLAVE1_BOMBE 1

const byte SEGMENT_MAP_DIGIT[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};

struct  s_mastermind master;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//  Wire.begin();
  // Init SPI bus
  SPI.begin();

  randomSeed(analogRead(A0));
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  CapaTouch.begin();
  matrix.begin(0x70);
  
  identification();

  master.password = random(1000, 9999); // définition du code de manière full aléatoire

  message("deconnecter les ", "deux fils blanc,", "le noir et le jaune", "# pour continuer");
  while (!Wire.requestFrom(I2C_SLAVE1_BOMBE, 1))
  {
    effet_LED_2();
    if ((lecture() == '*'))
      message("deconnecter les ", "deux fils blanc,", "le noir et le jaune", "# pour continuer");
    if ((lecture() == '#'))
      message("brancher les trois", "fils de la mallette", "a ceux disponible", "sur la bombe");
    delay(100);
  }

  effet_message();
  message("saisir code systeme", "manuellement", "* : reset", "# : valider");
  while (mastermind_test(&master));
  matrix.writeDisplay();
}

void win(int8_t minu, int8_t sec)
{
  message("la bombe a ete", "désamorcee", "temps restant :", "    min  sec");

  lcd.setCursor(2, 3);
  lcd.print(minu);
  lcd.setCursor(7, 3);
  lcd.print(sec);
  while (1) {
    effet_LED_5();
    delay(50);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
  recept();
}
