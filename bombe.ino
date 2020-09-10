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

#define speedd 200
#define SPEEDSEC 1000
int countDownTime = SPEEDSEC;

uint8_t cables = 0;
void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library
  Serial.begin(9600);
  randomSeed(analogRead(0));

  MFS.beep(10,    // beep for x*10 milliseconds
           5,    // silent for x*10 milliseconds
           4,    // repeat above cycle x times
           2,    // loop x times
           50    // wait x*10 milliseconds between loop
          );
  MFS.blinkLeds(LED_ALL, ON); //flag clignote
  MFS.writeLeds(LED_ALL, ON); // on/off

  delay(1500);
  MFS.blinkLeds(LED_ALL, OFF);
  MFS.writeLeds(LED_ALL, OFF);

  while (analogRead(A4) < 5); //detection presense(capteur)

  for (int i = 0; i < 3; i++)
  {
    uint8_t n;
    do
    {
      n = random(0, 8);
    } while ((cables >> n) & 1);
    cables ^= (1 << n);
  }
}

void game_over()
{

  MFS.beep(50, 10, 10);  // beep 3 times, 500 milliseconds on / 500 off
  MFS.wait(100000);
  exit(0);
}


void change_a4(int numberofgame)
{
  char flag = 0;
#define aff_debug
#ifdef aff_debug
  Serial.print("numberofgame : ");
  Serial.println(numberofgame);
#endif
  while (1)
  {
    if (timer())
      MFS.beep(2);
    switch (numberofgame)
    {

#define f(x, y)               \
  analogWrite(A4, 700);       \
  if(y && y && y)             \
  {                           \
    analogWrite(A4, 0);       \
    if (x && x && x)          \
    {                         \
      if (flag)               \
        return;               \
      else                    \
        break;                \
    }                         \
  }                           \
  delay(1000);                \
  flag = 1;                   \
  break;


      case 0:
        f(analogRead(A4) < 5, analogRead(A4) < 15)
      case 87:
        f(500 < analogRead(A4) && analogRead(A4) < 980, analogRead(A4) > 1000)
      case 127:
        f(analogRead(A4) > 1000, analogRead(A4) > 1000)
    }
  }
}

void decroche_fil(char numberofgame)//arg1= flag
{
  static uint8_t flag_ac = 0;


#ifdef aff_debug
  Serial.print("numberofgame : ");
  Serial.println((int)numberofgame);
#endif

  if (!numberofgame)
    exit(1);//erreur
  while (1)
  {
    timer();

    if ((analogRead(A5) / 1000 ^ (flag_ac & 1)) && (digitalRead(9) ^ ((flag_ac >> 1) & 1)) && (digitalRead(6) ^ ((flag_ac >> 2) & 1)) && (digitalRead(5) ^ ((flag_ac >> 3) & 1))) //etat init
      continue;
    else if ((flag_ac ^= numberofgame) && (analogRead(A5) / 1000 ^ (flag_ac & 1)) && (digitalRead(9) ^ ((flag_ac >> 1) & 1)) && (digitalRead(6) ^ ((flag_ac >> 2) & 1)) && (digitalRead(5) ^ ((flag_ac >> 3) & 1))) //cést bon
    {
      break; //quit to next game
    }
    else //erreur
    {
      Serial.println("erreur");
      flag_ac ^= numberofgame;//reset
      countDownTime = speedd;//speed timer
      MFS.beep(1);
    }
  }
  countDownTime = SPEEDSEC;
}

const int order_cable_1[12] = {
  0, 0, 0, 0, 87, 87, 87, 87, 127, 127, 127, 127
};
const char order_cable_led[12] = {
  0x44, 0x46, 0x55, 0x15, 0x0c, 0x19, 0x23, 0x23, 0x19, 0x1b, 0x1f, 0x4f
};
const char order_cable_2[5] = {
  8, 1, 2, 4, 0
};
const char order_cable_led_2[5] = {
  0x66, 0x57, 0x4d, 0x2f, 0
};

void  next_pin()
{
  static char choose_cable = 0;
  static char choose_rand = ~0;

#ifdef aff_debug
  Serial.print("next_pin : ");
  Serial.println((int)choose_cable);
  //delay(5);
#endif
  if (choose_cable == 8 || cables >> choose_cable & 1)
  {
    int order;
#ifdef aff_debug
    Serial.println("decroche");
#endif
    while (1)
    {
      order = random(0, 4);
      if ((choose_rand >> order) & 1)
      {
        choose_rand ^= 1 << order;
        break;
      }
    }
    MFS.blinkLeds(order_cable_led_2[order] >> 4 & 15, ON);
    all_lednum(order_cable_led_2[order] & 15);
    decroche_fil(order_cable_2[order]);
  }
  else
  {
    int rng = random(0, 12);
#ifdef aff_debug
    Serial.print("rgn :");
    Serial.println(rng);
#endif
    MFS.blinkLeds(order_cable_led[rng] >> 4 & 15, ON);
    all_lednum(order_cable_led[rng] & 15);
    change_a4(order_cable_1[rng]);
  }
  ++choose_cable;
  MFS.blinkLeds(LED_ALL, OFF);
}

game *select_game(char num_of_game)
{
  switch (num_of_game)
  {
    case 0:
      return (new game1());
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
      return (new game0());
    default:  //si on est la c'est win
      Serial.println("win!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      delay(10000);
      exit(0);
  }
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
    //next_pin();
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
