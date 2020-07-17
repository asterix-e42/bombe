#include "bombe.h"

button::button()
{
#ifdef aff_debug
  Serial.println("init button");
#endif
  reset();
  tab = (typeof(tab))malloc(sizeof(tab) * 10);
}

button::~button()
{
  free(tab);
  tab = 0;
}

void button::set_tab(void *tab)
{
  int i;
  struct buton *tableau = (struct buton *)tab;

  for (i = 0; tableau[i].number != 0 ; i++)
  {
    *(this->tab + i) = *(tableau + i);
  }
  *(this->tab + i) = *(tableau + i);
}

void button::reset()
{
  readtab = 0;
  timetmpbut = millis();
  flagbut = 0;
  jump = 0;
  presser = 0;
}

char button::get_readtab()
{
  return readtab;
}

int button::get_flag()
{
  return flagbut;
}

void button::set_flag(int f)
{
  flagbut = f;
}

struct buton * button::get_tab()
{
  return tab;
}

int button::cyclebuton(bool win)
{
  if (!flagbut)
  {
    if (win)
      return (presser);//win si tout est relacher
    return (2); //flagbut 0
  }
  --flagbut;
  --timebut[0];
  --timebut[1];
  --timebut[2];
  return (-1);
}

void button::appuibuton(int btn, struct buton *tab)
{
  flagbut = tab[readtab + jump].flag;
  timebut[btn - 1] = ((btn == tab[readtab].number) ? tab[readtab].iteration : ((btn == tab[readtab + 1].number) ? tab[readtab + 1].iteration : tab[readtab + 2].iteration));
  if (tab[readtab + jump].flag)
  {
    readtab += 1 + jump;
    jump = 0;
  }
  else
    ++jump;
  presser ^= 1 << btn;
}

//timer
int button::readsequencet()
{
  byte btn = MFS.getButton();

  if (btn & BUTTON_SHORT_RELEASE_IND) //bonton relacher
  {
    if (timebut[(btn & 3) - 1] || jump) //bonton ok
    {
      return (2);
    }
    else if (presser & 1 << (btn & 3))
      presser ^= 1 << (btn & 3);
  }
  else if (btn && !(btn >> 6))
  {
    if (flagbut)
    {
      return (2);
    }
    else if ((btn == tab[readtab].number) || (tab[readtab].flag == 0 && (btn == tab[readtab + 1].number || !tab[readtab + 1].flag)))//bon bouton
    {
      appuibuton(btn, tab);
    }
    else
      return (2);
  }

  if (timetmpbut + 1000 < millis())//calcul timing
  {
    timetmpbut += 1000;
    MFS.beep(5);
    return (cyclebuton(!tab[readtab].number));
  }
  return (1);
}

//non timer
int button::readsequence()
{
  byte btn = MFS.getButton();

  //  MFS.write((readtab + 10) * 10 + flagbut);

  if (btn & BUTTON_SHORT_RELEASE_IND) //bonton relacher
  {
    if (timebut[(btn & 3) - 1] || jump) //bonton ok
      return (2);
    else
    {
      timebut[(btn & 3) - 1] = -1;
      if (presser & 1 << (btn & 3))
        presser ^= 1 << (btn & 3);
      return cyclebuton(!tab[readtab].number);
    }
  }
  else if (btn && !(btn >> 6))
  {
    if (flagbut)
      return (2);
    else if ((btn == tab[readtab].number) || (tab[readtab].flag == 0 && (btn == tab[readtab + 1].number || (tab[readtab + 1].number && !tab[readtab + 1].flag))))//bon bouton
    {
      appuibuton(btn, tab);
      return cyclebuton(!tab[readtab].number);
    }
    else
      return (2);
  }
  return (!!(tab[readtab].number + presser)); // 1 mais si pb
}
