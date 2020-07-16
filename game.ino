
//class game
//{
//  protected :
//    int gamepart;
//    int result;
//    int (*func)(void *);
//
//  public :

game::game(void)
{

#ifdef aff_debug
  Serial.println("init game");
#endif
  reset();
}

game::~game()
{}

void game::reset()
{
  MFS.writeLeds(LED_ALL, OFF);
  MFS.write("");
  result = 0;
  setafftime(1);
}

int game::launch(void)
{}

void game::mauvais(void)
{
  butt->reset();//initbuton();
  //    MFS.beep(10,    // beep for x*10 milliseconds
  //           1,    // silent for x*10 milliseconds
  //           4,    // repeat above cycle x times
  //           1,    // loop x times
  //           10    // wait x*10 milliseconds between loop
  //          );
}
//};
