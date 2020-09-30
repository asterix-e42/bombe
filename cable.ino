
void decroche_fil(char numberofgame)//arg1= flag
{
  static uint8_t flag_ac = 0;


#ifdef aff_debug
  Serial.print("numberofgame : ");
  Serial.println((int)numberofgame);
#endif

  if (!numberofgame)
    exit(1);//erreur
  while (1)
  {
    if (timer())
      MFS.beep(2);
      
    if ((digitalRead(9) ^ ((flag_ac >> 1) & 1)) && (digitalRead(6) ^ ((flag_ac >> 2) & 1)) && (digitalRead(5) ^ ((flag_ac >> 3) & 1))) //etat init
      continue;
    else if ((flag_ac ^= numberofgame) && (digitalRead(9) ^ ((flag_ac >> 1) & 1)) && (digitalRead(6) ^ ((flag_ac >> 2) & 1)) && (digitalRead(5) ^ ((flag_ac >> 3) & 1))) //cést bon
    {
      break; //quit to next game
    }
    else //erreur
    {
      Serial.println("erreur");
      flag_ac ^= numberofgame;//reset
      countDownTime = speedd;//speed timer
      MFS.beep(1);
    }
  }
  countDownTime = SPEEDSEC;
}

const char order_cable_led_3[16] = {
  0x66, 0x88, 0x47, 0x55, 0x15,      0x4d, 0x0c, 0x19, 0x23, 0x46,      0x2f, 0x57, 0x1b, 0x1f, 0x4f
};
const char order_cable_2[5] = {
  8, 2, 4, 0, 1
};

void  next_pin()
{
  static char choose_cable = 0;
  static char choose_rand = ~0;

  state = 2;
  sendmess = "decablage page12    ";
#ifdef aff_debug
  Serial.print("next_pin : ");
  Serial.println((int)choose_cable);
  //delay(5);
#endif
  if (choose_cable == 9 || cables >> choose_cable & 1) // end case
  {
    int order;
#ifdef aff_debug
    Serial.println("decroche");
#endif
    while (1)
    {
      order = random(0, 3);
      if ((choose_rand >> order) & 1)
      {
        choose_rand ^= 1 << order;
        break;
      }
    }
    char t = random(0, 5);
    MFS.blinkLeds(order_cable_led_3[order * 5 + t] >> 4 & 15, ON);
    all_lednum(order_cable_led_3[order * 5 + t] & 15);
    decroche_fil(order_cable_2[order]);
  }
  ++choose_cable;
  MFS.blinkLeds(LED_ALL, OFF);
}
