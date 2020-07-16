#include "bombe.h"
#include <stdlib.h>

led::led()
{
#ifdef aff_debug
  Serial.println("init led");
#endif
  reset();
  tab = (typeof(tab))malloc(sizeof(tab) * 10);
}


void led::reset()
{
  readtabled = -1;
  iterationled = 0;
  timetmpled = millis();
}

led::~led()
{
  free(tab);
  tab = 0;
}

void led::set_tab(void *tab)
{
  int i;
  struct s_led *tableau = (struct s_led *)tab;
  for (i = 0; tableau[i].iteration != 0 ; i++)
    *(this->tab + i) = *(tableau + i);
  *(this->tab + i) = *(tableau + i);
}

int led::readsequence()
{
  if (readtabled == -1)
  {
    //Serial.println("reset_led");
    readtabled = 0;
    iterationled = tab[0].iteration;
  }

  if (timetmpled + 1000 < millis())
  {
    timetmpled += 1000;
    iterationled -= 1;
    if (!iterationled)
    {
      //Serial.println("next_led");
      readtabled += 1;//reset
      if (!(tab[readtabled]).iteration)
        readtabled = 0;
      iterationled = tab[readtabled].iteration;
    }
  }
  all_lednum((this->tab)[readtabled].number);
  return 0;
}


void all_lednum(char led)
{
  //Serial.println((int)led);
  MFS.writeLeds(LED_ALL, OFF);
  MFS.writeLeds(led, ON);
}
