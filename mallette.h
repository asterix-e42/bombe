
#ifndef H__mallette__H
#define H__mallette__H

#define MIN(x, y) (y < x)? y : x
typedef struct  s_mastermind
{
  String code;
  String password; //le bon password sera défini après par la machine
  int    max_lenght = 4;
}       t_mastermind;

int mastermind_attente_clavier(struct  s_mastermind *master);
#endif
