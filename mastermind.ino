
#include "mallette.h"

char  lecture()
{
  int key;

  key = CapaTouch.keyPad();

  if (key <= -1)
    return ('p');
  if (key < 10)
    return (key + '0');
  if (key == 11)
    return ('*');
  if (key == 12)
    return ('#');
}

int mastermind_attente_clavier(struct  s_mastermind *master)
{
  char buf = lecture();

  //  Serial.println("sdf");
  if ((isdigit(buf)) && ((master->code).length() < master->max_lenght))//(master->password).length()))
    // récupere l'information du clavier et la charge dans "code" + limite de taille pour code
  {

    matrix.writeDigitRaw(0, SEGMENT_MAP_DIGIT[3]);
    master->code += buf ;
    effet_clear();
    for (int j = 0; j < master->code.length(); j++)

      matrix.writeDigitRaw(j < 2 ? j : j + 1, ~SEGMENT_MAP_DIGIT[master->code.c_str()[j] - '0']);

    matrix.writeDisplay();
    return 0;
  }

  if (buf == '*')
    // quand appui sur la touche * = remise à zéro du code rentré par les joueurs
  {
    master->code = "";
    effet_clear();
  }


  if (buf == '#')// && ((master->code).length() == (master->password).length()))
    // quand appui sur la touche # comparaison pour vérifier sur le code rempli par les joeurs est vrai
  {
    return (1);//confirmation incomming
  }
  return (2); // on attend toujours;
}

void masterm(struct  s_mastermind *master, byte *cbp, byte *cmp) // dertermine nb correct
{
  int8_t tmppass; //nb char in pass
  int8_t tmpcode; //nb char in code
  for (char i = '0'; i <= '9'; i++)
  {
    tmppass = 0; //nb char in pass
    tmpcode = 0; //nb char in code
    for (int8_t j = 0; j < 4; j++)
    {
      if (master->password[j] == i)
      {
        if (master->code[j] == i)
          (*cbp)++;
        else
          tmppass++;
      }
      else if (master->code[j] == i)
        tmpcode++;
    }
    (*cmp) += MIN(tmppass, tmpcode);

  }
}


void mastermind_bad_code(struct  s_mastermind *master)
{
  byte cbp = 0; // chiffre qui donne le nombre de Charactère Bien Placé
  byte cmp = 0; // chiffre qui donne le nombre de Charactère Mal Placé

  masterm(master, &cbp, &cmp);

  message("     est non corect:", "valeur mal placer ", "valeur bien placer ", "");
  lcd.setCursor(0, 0);
  lcd.print(master->code);
  master->code = "";
  lcd.setCursor(19, 1);
  lcd.print(cmp);
  lcd.setCursor(19, 2);
  lcd.print(cbp);

}

void reusite_mastermind()
{
  mess("win");
}

boolean mastermind_test(struct  s_mastermind *master)
{
  static byte test_clavier = 0;

  //  lcd.setCursor(0, 3);
  //  lcd.print("rentrez code:");
  while (test_clavier)
    test_clavier = (lecture() != 'p');
  char flag_clavier = mastermind_attente_clavier(master);
  if (!flag_clavier)
    test_clavier = 1;
  else if (flag_clavier == 1 && master->code.length() == 4)
  {

    if (master->code == master->password) // quand appui sur la touche # comparaison pour vérifier sur le code rempli par les joeurs est faux
    {
      reusite_mastermind();
      return (false);
    }
    else
    {
      mastermind_bad_code(master);
    }
  }
  return (true);
}
