//
//
//
//game5::game5()
//{
//  Serial.println("game5");
//  reset(0);
//}
//
//void game5::reset(int i)
//{
//  game::reset(0);
//  setafftime(1);
//  (void) i;
//}
//
//void game5::next()
//{
//  ;
//}
//
//int game5::launch()
//{
//  static long unsigned int timetmp = millis();
//  int win = 0;
//  win = butt->readsequence();
//  if (!win)
//  {
//    Serial.println("next 4");
//    next();
//  }
//  else if (win > 1)
//  {
//    MFS.beep(1);
//    nbled = 0;
//  }
//  else if (win  < 0)
//  {
//    nbwin++;
//    if (nbwin == 5)
//      {
//        nbled = (nbled << 1) + 1;
//        nbwin = 0;
//      }
//  }
//  if (nbled >= 31)
//    return (0);
//
//  if (timetmp + 2000 < millis())//calcul timing
//  {
//    timetmp = millis();
//    nbled = nbled >> 1;
//  }
//all_lednum(nbled);
//  return 1;
//}
