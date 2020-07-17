
game7::game7()
{
#ifdef aff_debug
  Serial.println("game7");
#endif
  butt = new button();
  reset();
}

game7::~game7()
{
#ifdef aff_debug
  Serial.println("destr game7");
#endif
  delete butt;
  butt = NULL;
}

void game7::reset()
{
#ifdef aff_debug
  Serial.println("game7 reset");
#endif
  game::reset();
  setafftime(0);
  number = random();
  order = 0;
  next();
}

void game7::next()
{
  char affiche = (number >> (order << 2)) & 255;
  char cas = (affiche >> 6) & 3;
  int but;
  switch (order)
  {
    case 0:
      if (cas == 0)
        but = 2;
      else if (cas == 1)
        but = ((number >> (4 + order * 4)) & 3) + 1;
      else if (cas == 2)
        but = ((number >> (0 + order * 4)) & 3) + 1;
      else
        but = 2;
      break;

    case 1:
      if (cas == 0)
        but = 3;
      else if (cas == 1)
        but = 2;
      else if (cas == 2)
        but = ((number >> (2 + order * 4 )) & 3) + 1;
      else
        but = ((number >> (4 + order * 4 )) & 3) + 1;
      break;

    case 2:
      if (cas == 0)
        but = 2;
      else if (cas == 1)
        but = ((number >> (4 + order * 4 )) & 3) + 1;
      else if (cas == 2)
        but = ((number >> (2 + order * 4 )) & 3) + 1;
      else
        but = 4;
      break;

    case 3:
      if (cas == 0)
        but = ((number >> (2 + order * 4 )) & 3) + 1;
      else if (cas == 1)
        but = 3;
      else if (cas == 2)
        but = 1;
      else
        but = ((number >> (0 + order * 4 )) & 3) + 1;
      break;
  }
  but = (but == 4) ? order + 1 : but;
  but = (but >= 4) ? 1 : but;
  set_but(but);
  ++order;

  MFS.setdisplay(SEGMENT_MAP_DIGIT[((affiche >> 6) & 3) + 1], 0);
  MFS.setdisplay(SEGMENT_MAP_DIGIT[((affiche >> 4) & 3) + 1], 1);
  MFS.setdisplay(SEGMENT_MAP_DIGIT[((affiche >> 2) & 3) + 1], 2);
  MFS.setdisplay(SEGMENT_MAP_DIGIT[(affiche & 3) + 1], 3);

  int nbled = 8;
  if (order > 1)
    nbled |= 4;
  if (order > 2)
    nbled |= 2;
  if (order > 3)
    nbled |= 1;
  all_lednum(nbled);
}

void game7::set_but(int mem)
{
#ifdef aff_debug
  Serial.println(mem);
#endif
  struct buton but[2];
  but[1] = {0, 0, 0};
  but[0] = {mem, 1, 1};
  butt->reset();
  butt->set_tab(but);
}

int game7::launch()
{
  int win = butt->readsequence();
  if (win > 1)
  {
    MFS.beep(1);
  }
  else if (win  == 0)
  {
    if (order == 4)
      return 0;
    else
      next();
  }
  return 1;
}
