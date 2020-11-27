#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

/*

  For more information and help, please visit https://www.cohesivecomputing.co.uk/hackatronics/arduino-multi-function-shield/part-3/

  All our hackatronics projects are free for personal use, and there are many more
  in the pipeline. If you find our projects helpful or useful, please consider making
  a small donation to our hackatronics fund using the donate buttons on our web pages.
  Thank you.

*/
bool writetime = 1;
enum CountDownModeValues
{
  COUNTING_STOPPED,
  COUNTING
};

byte countDownMode = COUNTING_STOPPED;


void affichetime() {
  writetime = !writetime;
}

void setafftime(bool t) {
  writetime = t;
}

int timer() {
  static long unsigned int timetmp = millis();

  if (timetmp + countDownTime < millis())
  {
    timetmp += countDownTime;
    seconds--;

    if (seconds < 0 && minutes > 0)
    {
      seconds = 59;
      minutes--;
    }

    if (minutes == 0 && seconds == 0)
    {
      game_over();
    }
    if (writetime)
    {
      MFS.write(minutes * 100 + seconds);
    }
    return 1;
  }
  return 0;
}
