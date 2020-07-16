


game3::game3()
{

#ifdef aff_debug
  Serial.println("game3");
#endif
  reset();
}

void game3::reset()
{
  game::reset();

  timetmp = millis();
  timetmp2 = millis();

  game3_hz = random(500, 900);
  while (game3_hz - 80 <  poten->get_pot_pin() && game3_hz + 80 >  poten->get_pot_pin())
    game3_hz = random(500, 900);
  poten = new potentiometre();
}

game3::~game3(void)
{
  delete poten;
  poten = NULL;
}

int game3::launch()
{
  int pot = poten->get_pot_pin();
  pot = (game3_hz - pot) * 4;

  if (timetmp2 + 1000 + pot < millis())
  {
    MFS.beep(10);
    timetmp2 = millis();
  }

  if (timetmp + 500 < millis())
  {
    all_lednum(0);
  }
  if (timetmp + 1000 < millis())
  {
    all_lednum(1);
    timetmp += 1000;
  }

#ifdef aff_debug
  Serial.println(poten->get_pot());
  Serial.println(poten->get_pot_pin());
  Serial.println(pot);
#endif
  if (-20 < pot && pot < 20)
    return (0);
  return 1;
}
