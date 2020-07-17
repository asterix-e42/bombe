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
{}
