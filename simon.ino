#include "bombe.h"
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

game2::~game2(void)
{
  delete butt;
  butt = NULL;
  delete digt;
  digt = NULL;
}

game2::game2()
{
#ifdef aff_debug
  Serial.println("game2");
#endif
  butt = new button();
  digt = new digit();
  reset();
}

void game2::seqdigitr(byte *tal, int number)
{
  tal[number] = 0;
  while (number)
  {
    --number;
    tal[number] = random(1, 4);//rand simon
  }
}

void game2::mauvais()
{
  digt->reset();
  butt->reset();
  flag = 1;
  result = 1;
}

void game2::seqbutfromdigit(struct buton *but, byte *mem, char modif)
{
  int i = 0;
  while (mem[i])
  {
    but[i] = {((mem[i] - 9 + ((modif * modif == 4) ? (modif / 2) * i : modif)) % 3) + 3, 1, 2};
#ifdef aff_debug
    Serial.println("begin");
    Serial.println(mem[i]);
    Serial.println((int)modif);
    Serial.println((modif * modif == 4) ? (modif / 2) * i : modif);
    Serial.println( but[i].number);
    Serial.println("end");
#endif
    ++i;
  }

  but[i - 1].flag = 1;
  but[i] = {0, 0, 0};
}

void game2::reset()
{
  char modif = 0;//random(-1, 2);
  game::reset();
  setafftime(0);
  byte tal[10];
  struct buton but[10];

  seqdigitr(tal, 5);
  MFS.write("");
  //MFS.setdisplay(SEGMENT_MAP_DIGIT[modif + 3], 1);
  seqbutfromdigit(but, tal, modif);
  butt->set_tab(but);
  digt->set_tab(tal);
  mauvais();
#ifdef aff_debug
  Serial.println("game 2 reset");
#endif
}
int game2::launch()
{

  if (!result)
  {
#ifdef aff_debug
    Serial.println("win2");
#endif
    return 0;
  }
  if (flag)
  {
    flag = digt->readsequence();
    MFS.getButton();
  }
  else
    result = butt->readsequence();//(*fun)(data, mauvais2);
  if (result > 1)
  {
    mauvais();
    return (2);
  }
  return (1);
}
