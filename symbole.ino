game6::game6()
{
#ifdef aff_debug
  Serial.println("game6");
#endif
  reset();
}

void game6::reset()
{
  game::reset();
  setafftime(0);
  reponse = random(0, 256);
  led_on = 0;
  flag = 0;

  byte tmp = ~search_sbox(reponse);
  MFS.setdisplay(~0x30, 1);
  MFS.setdisplay(tmp, 2);
  MFS.setdisplay(~6, 3);
  MFS.setdisplay(~0, 0);
}

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

byte search_sbox(byte search) {
  uint8_t p = 1, q = 1;

  /* loop invariant: p * q == 1 in the Galois field */
  if (search == 0)
    return 0x63;
  do {
    p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
    q ^= q << 1;
    q ^= q << 2;
    q ^= q << 4;
    q ^= q & 0x80 ? 0x09 : 0;
  } while (p != search);
  return (q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4) ^ 0x63);
}

int game6::launch()
{
  byte btn = MFS.getButton();
  if (btn == 1 && led_on)
    --led_on;
  else if (btn == 3 && (led_on & 15) != 15)
    ++led_on;
  else if (btn == 2)
  {
    if (flag)
    {
#ifdef aff_debug
      Serial.println((int)reponse);
      Serial.println((int)led_on);
#endif
      if  (reponse == led_on)
        return 0;
      MFS.beep(1);
      reset();
      return 2;
    }
    else
      led_on <<= 4;
    flag = !flag;
  }

  all_lednum(led_on);
  return 1;
}
