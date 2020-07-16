#include "bombe.h"
#include <stdlib.h>

potentiometre::potentiometre()
{
#ifdef aff_debug
  Serial.println("init potentiometre");
#endif
  reset();
  tab = (typeof(tab))malloc(sizeof(tab) * 10);
}


potentiometre::~potentiometre()
{
  free(tab);
  tab = 0;
}

void potentiometre::reset()
{
#ifdef aff_debug
  Serial.println("reset potentiometre");
#endif
  flagpot = 0;
  pot = get_pot_pin();
}

void potentiometre::set_tab(void *tab)
{
  int i;
  char *tableau = (char *)tab;
  for (i = 0; tableau[i] != 0 ; i++)
  {
    *(this->tab + i) = *(tableau + i);
  }
  *(this->tab + i) = *(tableau + i);
}

int potentiometre::get_pot()
{
  return (pot);
}

int potentiometre::get_pot_pin()
{
  return (analogRead(POT_PIN));
}

int potentiometre::readsequence()
{
  if (tab[flagpot] == 0)
    return (0);
  if (tab[flagpot] < 0)
  {
    //       Serial.print("potentionmetre");
    //       Serial.print(get_pot() + tab[flagpot]);
    //        Serial.print(" _ ");
    //        Serial.println(get_pot_pin());
    if (get_pot() + tab[flagpot] > get_pot_pin())
    {
      ++flagpot;
      pot = get_pot_pin();
    }
    else if (get_pot() + 50 < get_pot_pin())
      return 2;
  }
  else
  {
    if (get_pot() + tab[flagpot] < get_pot_pin())
    {
      ++flagpot;
      pot = get_pot_pin();
    }
    else if (get_pot() - 50 > get_pot_pin())
      return 2;
  }
  return (1);
}
