
#ifndef bombe
#define bombe

struct buton {
  byte number;//quel bouton
  byte iteration;//temps
  byte flag;//temps
};

struct s_led {
  byte number;//quelle led
  byte iteration;//temps
};
void initbuton(void);
void affichetime(void);
int timer();
void initled(void);
int readpotseq(int *);
void pot_init();
int readsequencebut(void *tab);
int (*fun)(void *, void(*err)(void));
void readsequenceled(struct s_led *tab);
void all_lednum(char led);

class seq
{
  public :
    virtual void reset();
    virtual int readsequence();
    virtual void set_tab(void *tab);
};

class digit : public seq
{
  protected :
    char readtabled;
    char iterationled;
    long unsigned int timetmpled;
    byte *tab;
  public :
    digit();
    ~digit();

    void reset();
    void set_tab(void *tab);
    int readsequence();
};

class led : public seq
{
  protected :
    char readtabled;
    char iterationled;
    long unsigned int timetmpled;
    struct s_led *tab;
  public :
    led();
    ~led();

    void reset();
    void set_tab(void *tab);
    int readsequence();
};

class button : public seq
{
  protected :
    char readtab;
    long unsigned int timetmpbut;
    byte flagbut;
    byte jump;
    char timebut[3];
    byte presser;
    int cyclebuton(bool win);
    void appuibuton(int btn, struct buton *tab);
    struct buton *tab;
  public :
    struct buton *get_tab();
    char get_readtab();
    int get_flag();
    void set_flag(int);
    button();
    ~button();
    void reset();
    void set_tab(void *tab);
    //timer
    int readsequencet();

    //non timer
    int readsequence();
};

class potentiometre : public seq
{
  protected :
    int pot;
    byte flagpot;
    char *tab;

  public :

    potentiometre();
    ~potentiometre();
    void reset();
    void set_tab(void *tab);
    int get_pot();
    int get_pot_pin();
    int readsequence();
};

class game
{
  protected :
    int result;
    seq *func;
  public :
    game();//init
    ~game();//init
    virtual void reset();
    virtual void mauvais();
    virtual int launch(void);
};

class game0 : public game //game sequence led -> time button
{
  protected :
    void seqbut0(struct buton *but);
    void mauvais();
    struct buton but[10];
    byte pot[25];

  public :
    game0();
    ~game0();
    void reset();
    int launch(void);
};

class game1 : public game //game sequence led -> sequence button
{
  protected :
    void seqbut0(struct buton *but);
    void seqled0(s_led *led);
    void seqbut1(struct buton *but);
    void seqled1(s_led *led);
    void seqbut2(struct buton *but);
    void seqled2(s_led *led);
    void seqbut3(struct buton *but);
    void seqled3(s_led *led);
    void seqbut4(struct buton *but);
    void seqled4(s_led *led);
    void seqbut5(struct buton *but);
    void seqled5(s_led *led);
    void seqpot0(char *pot);

    struct s_led tal[10];
    struct buton but[10];
    char pot[10];
    void mauvais();

  public :
    game1();
    ~game1();
    void reset();
    int launch(void);
};

class game2 : public game
{
  protected :
    int flag;
    int gamepart;
    int result;
    void seqbutfromdigit(struct buton *but, byte *mem, char mod);
    void mauvais();
    void seqdigitr(byte *tal, int number);
  public :
    game2();
    ~game2();
    void reset();
    int launch(void);
};

class game3 : public game
{
  protected :
    long unsigned int timetmp;
    long unsigned int timetmp2;
    int game3_hz;
  public :
    game3();
    ~game3();
    void reset();
    int launch(void);
};

class game4 : public game
{
  protected :
    void seqbutfromdigit(struct buton *but, byte mem, byte number);
    void next();
    int nbwin;
    int nbled;

  public :
    game4();
    ~game4();
    void reset();
    int launch(void);
};

class game5 : public game
{
  protected :
    int reponse;

  public :
    game5();
    ~game5();
    void reset();
    int launch(void);
};

class game6 : public game
{
  protected :
    uint8_t reponse;
    uint8_t led_on;
    int flag;

  public :
    game6();
    ~game6();
    void reset();
    int launch(void);
};

class game7 : public game
{
  protected :
    void next();
    void set_but(int mem);
    int  number;
    char  order;

  public :
    game7();
    ~game7();
    void reset();
    int launch(void);
};

#endif
