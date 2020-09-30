#include "bombe.h"
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

#ifndef easy_mode
void game0::seqbut0(struct buton *but) //ramdom hard
{
  int i = 0;
  char but_tmp[3] = {0};
  byte again = 0;
  while (i < 7 || again)
  {
    byte tmp;

    do
    {
      tmp = random(1, 4);
    } while (but_tmp[tmp - 1] > 0);

    but_tmp[tmp - 1] = random(1, 5);
    //    if (random(0, 6) == 4)
    //      but[i++] = {(tmp == 3)? 1 : tmp + 1, random(0, 4), 0};
    byte flag = random(again, 3);
    again = !flag;
    but[i] = {tmp, but_tmp[tmp - 1] - 1, flag};


    but_tmp[0] -= flag;
    but_tmp[1] -= flag;
    but_tmp[2] -= flag;
    while (but_tmp[0] > 0 && but_tmp[1] > 0 && but_tmp[2] > 0)
    {
      --but_tmp[0];
      --but_tmp[1];
      --but_tmp[2];
      but[i].flag++;
    }
    ++i;
  }
  but[i - 1].iteration = 2;
  but[i - 1].flag = 3;
  but[i] = {0, 0, 0};
  //  for (int y = -1; y != i; y++)
  //  {
  //    Serial.print(but[y].number);
  //    Serial.print(" ");
  //    Serial.print(but[y].iteration);
  //    Serial.print(" ");
  //    Serial.print(but[y].flag);
  //    Serial.println(" ");
  //  }
}
#else
void game0::seqbut0(struct buton *but)
{
  int i = 0;
  while (i < 12)
  {
    byte tmp = random(1, 4);
    if (random(0, 5) == 4)
      but[i++] = {(tmp == 3) ? 1 : tmp + 1, 0, 0};
    but[i] = {tmp, 0, 1};
    ++i;
  }
  but[i] = {0, 0, 0};
}
#endif

game0::game0(void)
{
  butt = new button();
  reset();
  
  state = 2;
  sendmess = TETRIS;
#ifdef aff_debug
  Serial.println("init game0");
#endif
}

void game0::reset()
{
  game::reset();
  setafftime(0);
  butt->reset();
  seqbut0(but);
  butt->set_tab(but);
  digittmp_init(but);
  butt->reset();
  result = 1;
  butt->set_flag(2);
}

game0::~game0(void)
{
  delete butt;
  butt = NULL;
}

void game0::mauvais(void)
{
  MFS.beep(10,    // beep for x*10 milliseconds
           1,    // silent for x*10 milliseconds
           4,    // repeat above cycle x times
           1,    // loop x times
           10    // wait x*10 milliseconds between loop
          );
  MFS.wait(500);
}

int game0::launch()
{
  if (!result)
    return 0;//win game
  result = butt->readsequencet();//(*func)();
  if (result > 1 || result < -1)
  {
    mauvais();
    reset();
  }
  else if (result < 0)
  {
    digittmp(butt->get_flag(), butt->get_readtab(), butt->get_tab());
  }
  return (result);
}


void digittmp_init(struct buton *but)
{
  for (int i = 0; i < 3; i++)
    dig[i] = dig_buf[i] = 0;

  int i = -1;
  do
  {
    ++i;
    dig[but[i].number - 1] = 1;
    dig_buf[but[i].number - 1] = but[i].iteration;
  } while (!but[i].flag);
}

void digittmp(char flag, int readtab, struct buton *but)
{
  nextgit(0);
  nextgit(1);
  nextgit(2);

  while (flag < 2)
  {
    if (!(but[readtab]).number)
      break;
    flag += (but[readtab]).flag;
    ++readtab;
  }
  if (flag == 2 && (but[readtab]).number)
  {
    int i = readtab - 1;
    do {
      ++i;
      dig[but[i].number - 1] |= 1;
      dig_buf[but[i].number - 1] = but[i].iteration;
    } while (!but[i].flag);
  }
  MFS.setdisplay(~dig[0], 0);
  MFS.setdisplay(~dig[1], 1);
  MFS.setdisplay(~dig[2], 2);
}
