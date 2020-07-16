#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>
#include "bombe.h"

#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8
button *butt = NULL;
potentiometre *poten = NULL;
led *led_ = NULL;
digit *digt;


#define aff_debug 0
int countDownTime = 1000;

void setup() {
  // put your setup code here, to run once:

  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library
  Serial.begin(9600);
  randomSeed(analogRead(5));

  MFS.beep(10,    // beep for x*10 milliseconds
           5,    // silent for x*10 milliseconds
           4,    // repeat above cycle x times
           2,    // loop x times
           50    // wait x*10 milliseconds between loop
          );
  //
  //  affichetime();
  MFS.blinkLeds(LED_ALL, ON); //flag clignote
  MFS.writeLeds(LED_ALL, ON); // on/off

  delay(1500);
  MFS.blinkLeds(LED_ALL, OFF);
  MFS.writeLeds(LED_ALL, OFF);

  while (analogRead(A4) < 5); //detection presense(capteur)
}


const int order_cable_1[7] = {
  0, 0, 1016, 1016, 0, 1016, 0
};
const char order_cable_led[9] = {
  0x13, 6, 12, 10, 9, 15, 3, 5, 8
};

void game_over()
{

  MFS.beep(50, 10, 10);  // beep 3 times, 500 milliseconds on / 500 off
  MFS.wait(100000);
  exit(0);
}


void  next_pin(int numberofgame)
{

  MFS.blinkLeds(order_cable_led[numberofgame]>>4 & 15, ON);
  all_lednum(order_cable_led[numberofgame] & 15);
  int count_time = 20;
  char inv = 0;

  if (numberofgame < 5)
  {
    while (count_time != 3)
    {
      timer();
      if ((order_cable_1[numberofgame] & 1008) == (analogRead(A4) & 1008))
      {

#ifdef aff_debug
        Serial.print(numberofgame);
        Serial.print(" : ");
        Serial.print(analogRead(A4) & 1008);
        Serial.print(" : ");
        Serial.print(order_cable_1[numberofgame] & 1008);
        delay(100);
#endif
        analogWrite(A4, inv ? 500 : 0);
        inv = !inv;
        if (count_time < 40)
          ++count_time;
        continue ;
      }

      count_time = 0;
      //      analogWrite(A4, 0);
    }
  }
  else
  {
#define speedd 200
    while (count_time)
    {
      switch (numberofgame)
      {
        case 5:
          if (analogRead(A5) != 0 && digitalRead(9) != 0 && digitalRead(6) == 0 && digitalRead(5) != 0)
            count_time = 0;
          else if ((analogRead(A5) != 0) && (digitalRead(9) != 0) && (digitalRead(6) != 0) && (digitalRead(5) != 0))
            countDownTime = 1000;
          else
          {
            countDownTime = speedd;
            MFS.beep(1);
          }
          break;
        case 6:
          all_lednum(3);
          if (analogRead(A5) != 0 && digitalRead(9) == 0 && digitalRead(6) == 0 && digitalRead(5) != 0)
            count_time = 0;
          else if ((analogRead(A5) != 0) && (digitalRead(9) != 0) && (digitalRead(6) == 0) && (digitalRead(5) != 0))
            countDownTime = 1000;
          else
          {
            countDownTime = speedd;
            MFS.beep(1);
          }
          break;
        case 7:
          all_lednum(3);
          if ((analogRead(A5) == 0) && (digitalRead(9) == 0) && (digitalRead(6) == 0) && (digitalRead(5) != 0))
          {
            delay(100000);
            exit;                                                       //win
          }
          else if ((analogRead(A5) != 0) && (digitalRead(9) == 0) && (digitalRead(6) == 0) && (digitalRead(5) != 0))
            countDownTime = 1000;
          else
          {
            game_over();
          }
          break;
      }
      timer();
    }
  }
  MFS.blinkLeds(LED_ALL, OFF);
}


game *select_game(char num_of_game)
{
  switch (num_of_game)
  {
    case 0:
      return (new game0());
    case 1:
      return (new game1());
    case 2:
      return (new game2());
    case 3:
      return (new game3());
    case 4:
      return (new game4());
    case 5:
      return (new game5());
    case 6:
      return (new game6());
    case 7:
      return (new game7());
    default:
      return (NULL);
  }
}

void  loop() {

  //MFS.write((((((analogRead(A5) / 100) + digitalRead(9)) * 10) + digitalRead(6)) * 10) + digitalRead(5));
  //MFS.write(analogRead(A4));

  static game *game_l = NULL;
  static char win = 0;
  static char numberofgame = 0;
  //WriteInDigit();

  if (!win)
  {

    setafftime(1);
    if (game_l)
    {

    MFS.beep(10,    // beep for x*10 milliseconds
             5,    // silent for x*10 milliseconds
             4,    // repeat above cycle x times
             2,    // loop x times
             50    // wait x*10 milliseconds between loop
            );
#ifdef aff_debug
      Serial.println("win the game and start another");
      Serial.print("delete game : game");
      Serial.println((int)numberofgame);
#endif
      delete game_l;
#ifdef aff_debug
      Serial.println("ok");
#endif
      game_l = NULL;
    numberofgame += 1;//random(0, 8); //rand
    }
    next_pin(numberofgame);
    MFS.beep(10,    // beep for x*10 milliseconds
             5,    // silent for x*10 milliseconds
             4,    // repeat above cycle x times
             2,    // loop x times
             50    // wait x*10 milliseconds between loop
            );
    //numberofgame = 1;
    Serial.print("new game : game");
    Serial.println((int)numberofgame);
    Serial.println("");
    game_l = select_game(5);//numberofgame);

    //init();
  }
  win = game_l->launch();
  timer();

  //  int sensorValue1 = analogRead(A5);
  //  int sensorValue2 = digitalRead(5);
  //  Serial.println(sensorValue2);
}
