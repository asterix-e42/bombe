const char *game5_reponse[] = {
  NULL,
  "f", //195
  "x", //390
  "l",//586
  "h",//781
  "q",//976
  "v",//1171
  "y",//1366
  "j",//1.562
  "z",//1.757
  "p",//1.952

//  "read", //195
//  "brain", //390
//  "mind",//586
//  "break",//781
//  "boat",//976
//  "rain",//1171
//  "mean",//1366
//  "make",//1.562
//  "right",//1.757
//  "bath",//1.952
};
game5::game5()
{
#ifdef aff_debug
  Serial.println("game5");
#endif
  reset();
}

void game5::reset()
{
  game::reset();
  setafftime(1);
  while (!game5_reponse[reponse = random(0, sizeof(game5_reponse))]);
  //reponse = 1;
#ifdef aff_debug
  Serial.print("reponse game5 : ");
  Serial.print(reponse);
#endif
}

typedef struct morseleter
{
  uint8_t l1: 2, l2: 2, l3: 2, l4: 2;
} ML;

typedef union  morsel {
  uint8_t l1: 2, l2: 2, l3: 2, l4: 2;
  uint8_t leter;
} Ml;

const ML Malpha[26] = {
  {.l1 = 1, .l2 = 3, .l3 = 0, .l4 = 0},   //A
  {3, 1, 1, 1}, //B
  {3, 1, 3, 1}, //C
  {3, 1, 1, 0},  //D
  {1, 0, 0, 0},    //E
  {1, 1, 3, 1}, //F
  {3, 3, 1, 0},  //G
  {1, 1, 1, 1}, //H
  {1, 1, 0, 0},   //I
  {1, 3, 3, 3}, //J
  {3, 1, 3, 0},  //K
  {1, 3, 1, 1}, //L
  {3, 3, 0, 0},   //M
  {3, 1, 0, 0},   //N
  {3, 3, 3, 0},  //O
  {1, 3, 3, 1}, //P
  {3, 3, 1, 3}, //Q
  {1, 3, 1, 0},  //R
  {1, 1, 1, 0},  //S
  {3, 0, 0, 0},    //T
  {1, 1, 3, 0},  //U
  {1, 1, 1, 3}, //V
  {1, 3, 3, 0},  //W
  {3, 1, 1, 3}, //X
  {3, 1, 3, 3}, //Y
  {3, 3, 1, 1} //Z
};

const int timemorse = 30;

int game5::launch()
{
  static long unsigned int timetmp = millis();
  int win = 0;
  const char *tab = game5_reponse[reponse];
  static int defiltab = 0;
  static int defilmorse = 0;

  float sensorValue = (analogRead(A0) >> 4) - 20;;

  if (sensorValue < 0)
    MFS.write("up");
  else if (sensorValue > 15)
    MFS.write("down");
  else
    MFS.write(sensorValue / 5.123, 3);
  if (MFS.getButton() == 3)
  {
    if (sensorValue == reponse)
      return (0);
    reset();
    MFS.beep(10);
    return 2;
  }
  if (timetmp < millis())//calcul timing
  {
    int t = tab[defiltab];
    //leter
    if (t == 0x20) //space
    {
      timetmp = millis() + timemorse * 50;
      defiltab++;
    }
    else if (!t) // fin des mots
    {
      timetmp = millis() + timemorse * 80;
      defiltab = 0;
      defilmorse = 0;
    }
    else if (!('a' <= t && t <= 'z')) // no alpha
      exit(1);
    else
    {
      t = ( (  ((Ml*)&Malpha[t - 'a'])->leter >> (defilmorse << 1) ) & 3) * timemorse; /// ARRRGGG!!
      if (!t)
      {
        defilmorse = 0;
        defiltab++;
        timetmp = millis() + timemorse * 20;
      }
      else
      {
        timetmp = millis() + (t + timemorse) * 10;
        MFS.beep(t);
        defilmorse++;
      }
    }
  }
  return 1;
}
