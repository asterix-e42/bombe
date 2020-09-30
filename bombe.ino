#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>
#include "bombe.h"

#define easy_mode

#define DELAY_BEEP  "recalibrage page8   "
#define MEMORY      "order page ??"
#define MORSE       "morse page10 "
#define SIMON       "memory page7"
#define SOYER_ATT   "soyez attentif p6   "
#define SYMBOLE     "ou est charlie p11  "
#define TAPETATAUPE "tape ta taupe page9 "
#define TETRIS      "tetris page5        "

# define I2C_SLAVE1_BOMBE 1
#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8
button *butt = NULL;
potentiometre *poten = NULL;
led *led_ = NULL;
digit *digt;

#define easy_mode
//#define aff_debug

#define speedd 200
#define SPEEDSEC 1000
int countDownTime = SPEEDSEC;

char state = 0;
String sendmess = "sendmess";


char seconds = 5;
char minutes = 40;


uint8_t cables = 0;
void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library
  Serial.begin(9600);
  Wire.begin(I2C_SLAVE1_BOMBE);
  Wire.onRequest(requestI2C);
  randomSeed(analogRead(A0));

  MFS.beep(10,    // beep for x*10 milliseconds
           5,    // silent for x*10 milliseconds
           4,    // repeat above cycle x times
           2,    // loop x times
           50    // wait x*10 milliseconds between loop
          );
  MFS.blinkLeds(LED_ALL, ON); //flag clignote
  MFS.writeLeds(LED_ALL, ON); // on/off

  delay(15000);
  MFS.blinkLeds(LED_ALL, OFF);
  MFS.writeLeds(LED_ALL, OFF);

//while(1)
//    Serial.println(analogRead(A4));
  while (analogRead(A4) < 30) //detection presense(capteur)

    Serial.println(analogRead(A4));

  for (int i = 0; i < 2; i++)
  {
    uint8_t n;
    do
    {
      n = random(1, 8);
    } while ((cables >> n) & 1);
    cables ^= (1 << n);
  }

  while (!state)
  {
    if (timer())
      MFS.beep(2);
  }

}

void requestI2C() {
  Wire.write(state);
  switch (state)
  {
    case (1):

      Wire.write(minutes);
      Wire.write(seconds);

      Wire.write("       ");
      break;
    default :
      Serial.println("ichbinda");
      Wire.write(sendmess.c_str());
      Wire.write("                  ");
  }
  state = 1;

}

void game_over()
{

  MFS.beep(50, 10, 10);  // beep 3 times, 500 milliseconds on / 500 off
  MFS.wait(100000);
  exit(0);
}



game *select_game(char num_of_game)
{
  switch (num_of_game)
  {
    case 0:
      return (new game0());
    case 88:
      return (new game0());
    case 2:
      return (new game1());
    case 3:
      return (new game5());
    case 4:
      return (new game3());
    case 5:
      return (new game4());
    case 6:
      return (new game2());
    case 7:
      return (new game6());
    case 8:
      return (new game1());
      //    case 9:
      return (new game6());
    case 10:
      return (new game5());
    case 11:
      return (new game2());
    case 12:
      return (new game3());
    case 13:
      return (new game4());
    case 14:
      return (new game0());
    default:  //si on est la c'est win
      win();
  }
}

void win()
{
//    setafftime(1);
  MFS.beep(300);
  state = 3;
  sendmess = "win";
  while(!timer());
  delay(100000);
  exit(0);
}

void  loop() {

  static game *game_l = NULL;
  static char win = 0;
  static char numberofgame = 0;

  if (!win)
  {

#ifdef aff_debug
    Serial.print("game num= ");
    Serial.println((int)numberofgame);
#endif
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
      game_l = NULL;
      numberofgame += 1;//random(0, 8); //rand
    }
    next_pin();
    MFS.beep(10,    // beep for x*10 milliseconds
             5,    // silent for x*10 milliseconds
             4,    // repeat above cycle x times
             2,    // loop x times
             50    // wait x*10 milliseconds between loop
            );
    delay(300);
    //numberofgame = 1;
    Serial.print("new game : game");
    Serial.println((int)numberofgame);
    Serial.println("");
    game_l = select_game(numberofgame);

    //init();
  }
  win = game_l->launch();
  timer();
}
