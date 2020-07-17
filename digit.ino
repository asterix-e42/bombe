#include "bombe.h"
static char readtabdigit;
static long unsigned int timetmpdigit;

const byte SEGMENT_MAP_ALPHA[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
                                  0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x80, 0x76, 0x0E, 0x38,
                                  0x73, 0x67, 0x3E, 0x5C, 0x63, 0x08, 0x40, 0x48, 0x53, 0x00
                                 };

const byte SEGMENT_MAP_DIGIT[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};

digit::digit()
{
#ifdef aff_debug
  Serial.println("init digit");
#endif
  reset();
  tab = (typeof(tab))malloc(sizeof(tab) * 10);
}

digit::~digit()
{
  free(tab);
  tab = 0;
}

void digit::reset()
{
  readtabdigit = 0;
  timetmpdigit = millis();
}

void digit::set_tab(void *tab)
{
  int i;
  byte *tableau = (byte *)tab;
  for (i = 0; tableau[i] != 0 ; i++)
    *(this->tab + i) = *(tableau + i);
  *(this->tab + i) = *(tableau + i);
}

int digit::readsequence()
{
  if (timetmpdigit + 1000 < millis())
  {
#ifdef aff_debug
    Serial.println(timetmpdigit);
#endif
    timetmpdigit += 1000;
    readtabdigit += 1;//reset
    if (!(tab[readtabdigit]))
    {
      MFS.setdisplay(~0x40, 3);
      readtabdigit = 0;
      return 0;
    }
  }
  MFS.setdisplay(SEGMENT_MAP_DIGIT[tab[readtabdigit]], 3);
  if (timetmpdigit + 900 < millis())
  {
    MFS.setdisplay(~0, 3);
  }
  return 1;
}

char dig[3];
char dig_buf[3];
void nextgit(int select)
{
  char tmp = 0;
  tmp |= (0x40 & dig[select]) >> 3;
  tmp |= (1 & dig[select]) << 6;
  tmp |= (0x22 & dig[select]) ? 0x14 : 0;
  if (dig_buf[select])
  {
    --dig_buf[select];
    tmp |= 0x23;
  }
  dig[select] = tmp;
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
  MFS.setdisplay(~dig[0], 1);
  MFS.setdisplay(~dig[1], 2);
  MFS.setdisplay(~dig[2], 3);
}
