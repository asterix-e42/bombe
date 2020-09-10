game4::game4()
{
#ifdef aff_debug
  Serial.println("game4");
#endif
  butt = new button();
  reset();
}

game4::~game4()
{
  delete butt;
  butt = NULL;
}

void game4::seqbutfromdigit(struct buton *but, byte mem, byte number)
{
  but[number] = {0, 0, 0};
  but[--number] = {mem, 1, 1};
  while (number)
  {
    number--;
    but[number] = {mem, 1, 2};
  }
}

void game4::reset()
{
  game::reset();
  setafftime(0);
  nbwin = 0;
  nbled = 0;
  next();
}

void game4::next()
{
  static byte number_tap;
  static byte number_but;
  struct buton but[10];

  number_tap = random(2, 5);
  number_but = random(1, 4);
  seqbutfromdigit(but, number_but, number_tap);

  butt->reset();
  butt->set_tab(but);
  MFS.write("");
  MFS.wait(50);
  MFS.setdisplay(SEGMENT_MAP_DIGIT[number_tap], number_but);
}

int game4::launch()
{
  static long unsigned int timetmp = millis();
  int win = 0;
  win = butt->readsequence();
  if (!win)
  {
#ifdef aff_debug
    Serial.println("next game4");
#endif
    next();
  }
  else if (win > 1)
  {
    MFS.beep(1);
    nbled = 0;
  }
  else if (win  < 0)
  {
    nbwin++;
    if (nbwin == 5)
    {
      nbled = (nbled << 1) + 1;
      nbwin = 0;
    }
  }
  if (nbled >= 31)
    return (0);

  if (timetmp + 1500 < millis())//calcul timing
  {
    timetmp = millis();
    nbled = nbled >> 1;
  }
  all_lednum(nbled);
  return 1;
}
