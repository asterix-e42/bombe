#include "bombe.h"
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

void game1::seqbut0(struct buton *but)
{
  but[0] = {2, 1, 2};
  but[1] = {1, 2, 1};
  but[2] = {3, 2, 3};
  but[3] = {2, 1, 2};
  but[4] = {0, 0, 0};
}
void game1::seqled0(s_led *led)
{
  led[0] = {LED_1, 1};
  led[1] = {LED_2, 1};
  led[2] = {LED_3 | LED_4, 2};
  led[3] = {LED_3, 1};
  led[4] = {0, 2};
  led[5] = {0, 0};
}

void game1::seqbut1(struct buton *but)
{
  but[0] = {2, 1, 2};
  but[1] = {3, 4, 1};
  but[2] = {1, 1, 2};
  but[3] = {1, 2, 3};
  but[4] = {0, 0, 0};
}
void game1::seqled1(s_led *led)
{
  led[0] = {LED_1, 1};
  led[1] = {LED_2, 1};
  led[2] = {LED_3 | LED_4, 1};
  led[3] = {LED_3, 1};
  led[4] = {LED_3 | LED_4, 1};
  led[5] = {0, 2};
  led[6] = {0, 0};
}
void game1::seqbut2(struct buton *but)
{
  but[0] = {3, 1, 2};
  but[1] = {2, 1, 2};
  but[2] = {1, 2, 1};
  but[3] = {3, 2, 3};
  but[4] = {0, 0, 0};
}
void game1::seqled2(s_led *led)
{
  led[0] = {LED_2, 1};
  led[1] = {LED_1, 1};
  led[2] = {LED_3 | LED_4, 2};
  led[3] = {LED_3, 1};
  led[4] = {0, 2};
  led[5] = {0, 0};
}

void game1::seqbut3(struct buton *but)
{
  but[0] = {1, 2, 1};
  but[1] = {3, 3, 2};
  but[2] = {1, 2, 3};
  but[3] = {3, 1, 2};
  but[4] = {0, 0, 0};
}
void game1::seqled3(s_led *led)
{
  led[0] = {LED_3 | LED_4, 1};
  led[1] = {LED_1, 1};
  led[2] = {LED_2, 1};
  led[3] = {LED_3 | LED_4, 1};
  led[4] = {LED_3, 1};
  led[5] = {0, 2};
  led[6] = {0, 0};
}

void game1::seqbut4(struct buton *but)
{
  but[0] = {3, 1, 2};
  but[1] = {2, 1, 2};
  but[2] = {3, 3, 1};
  but[3] = {1, 1, 3};
  but[4] = {0, 0, 0};
}
void game1::seqled4(s_led *led)
{
  led[0] = {LED_1, 1};
  led[1] = {LED_3, 1};
  led[2] = {LED_3 | LED_4, 2};
  led[3] = {LED_3, 1};
  led[4] = {0, 2};
  led[5] = {0, 0};
}
void game1::seqbut5(struct buton *but)
{
  but[0] = {1, 4, 1};
  but[1] = {3, 1, 2};
  but[2] = {3, 2, 3};
  but[3] = {2, 1, 2};
  but[4] = {0, 0, 0};
}
void game1::seqled5(s_led *led)
{
  led[0] = {LED_1, 1};
  led[1] = {LED_3 | LED_4, 1};
  led[2] = {LED_2, 1};
  led[3] = {LED_3 | LED_4, 1};
  led[4] = {LED_2, 1};
  led[5] = {0, 2};
  led[6] = {0, 0};
}

void game1::seqpot0(char *pot)
{
  pot[0] = -50;
  pot[1] = 50;
  pot[2] = 0;
}

void game1::mauvais(void)
{

  poten->reset();
  butt->reset();//initbuton();
  MFS.beep(10,    // beep for x*10 milliseconds
           1,    // silent for x*10 milliseconds
           4,    // repeat above cycle x times
           1,    // loop x times
           10    // wait x*10 milliseconds between loop
          );
}

game1::game1(void)
{

  led_ = new led();
  poten = new potentiometre();
  butt = new button();
  reset();
  state = 3;
  sendmess = SOYER_ATT;
#ifdef aff_debug
  Serial.println("init game1");
#endif
}

game1::~game1(void)
{
  delete poten;
  poten = NULL;
  delete butt;
  butt = NULL;
  delete led_;
  led_ = NULL;
}

void game1::reset()
{
  game::reset();
  void *tab;
  switch (random(0, 6))
  {
    case 0:
      seqled0(tal);
      seqbut0(but);
      tab = but;
      func = butt;
      break;
    case 1:
      seqled1(tal);
      seqbut1(but);
      tab = but;
      func = butt;
      break;
    case 2:
      seqled2(tal);
      seqbut2(but);
      tab = but;
      func = butt;
      break;
    case 3:
      seqled3(tal);
      seqbut3(but);
      tab = but;
      func = butt;
      break;
    case 4:
      seqled4(tal);
      seqbut4(but);
      tab = but;
      func = butt;
      break;
    case 5:
      seqled5(tal);
      seqbut5(but);
      tab = but;
      func = butt;
      break;
  }

  led_->set_tab(tal);
  func->reset();
  func->set_tab(tab);
  result = 1;
}

int game1::launch()
{
  if (!result)
    return 0;
  led_->readsequence();
  result = func->readsequence();
#ifdef aff_debug
  Serial.print("result : ");
  Serial.println(result);
#endif
  if (result > 1)
    mauvais();
  return (result);
}
