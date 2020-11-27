

#include <MFRC522.h> // RFID
MFRC522 rfid(SS_PIN, RST_PIN);

// Tableau contentent l'ID


void identification()
{
  MFRC522 rfid(SS_PIN, RST_PIN);

  struct  s_mastermind master;
  master.password = "2204";

  byte nuidPICC[4];

  // Init MFRC522
  rfid.PCD_Init();


  mess("kit de desarmorsage    identification   Vj9.d4163uf4.42     badge requis");

  while (!rfid_ident(nuidPICC));
  while (code_test(&master));


}


#define CARTE1 0xC6, 0x19, 0x1C, 0x1B 
#define CARTE2 0x8F, 0x85, 0xBE, 0x29
#define CARTE3 0xC6, 0xD0, 0x24, 0x1B

#define CARTE CARTE3
boolean rfid_ident(byte *nuidPICC)
{
  if (!rfid.PICC_IsNewCardPresent())
    return 0;

  if ( !rfid.PICC_ReadCardSerial())
    return 0;

  byte nuidrem[4] = {CARTE2};
#define rap(i) nuidrem[i] == rfid.uid.uidByte[i]
  if ((rap(0) && rap(1) && rap(2) && rap(3)))
    return 1;
#undef rap
  byte nuidrem2[4] = {CARTE3};
#define rap(i) nuidrem2[i] == rfid.uid.uidByte[i]
  if ((rap(0) && rap(1) && rap(2) && rap(3)))
    return 1;
#undef rap

  mess("user unknown");
  return 0;
}

boolean code_test(struct  s_mastermind *master)
{
  byte test_clavier = 0;

  message("Bonjour Mr S.Morse", "veuillez saisir code", "* : reset", "# : valider");

  while (1)
  {

    while (test_clavier)
      test_clavier = (lecture() != 'p');
    char flag_clavier = mastermind_attente_clavier(master);
    if (!flag_clavier)
      test_clavier = 1;
    else if (flag_clavier == 1)
    {
      effet_clear();
      if (master->code == master->password) // quand appui sur la touche # comparaison pour vérifier sur le code rempli par les joeurs est faux
      {
        mess("   identification         reussie");
        delay(2000);
        return (false);
      }
      else
      {
        master->code = "";
        mess("identification fail");
        delay(5000);
        return (true);
      }
    }

  }
  return (true);
}
