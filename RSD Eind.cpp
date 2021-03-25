#include "mbed.h"
// serial setup
Serial pc(USBTX, USBRX);
// buttons
InterruptIn mode(D13, PullUp);
InterruptIn startstop(D5, PullUp);
// potmeter
AnalogIn pot(A7);
// cny70
AnalogIn cnyIn(A0);
// pwm H-brug
PwmOut speed(D2);
// DirPins
DigitalOut dir1(D3);
DigitalOut dir2(D4);
// variables
int wittelijn = 0;
int cny = cnyIn;
int achteruitcount;
int startpressed = 0;
int modepressed = 0;
float f;

void vooruit() {
  dir1 = 1;
  dir2 = 0;
}

void achteruit() {
  dir1 = 0;
  dir2 = 1;
}

void stop() {
  dir1 = 0;
  dir2 = 0;
}

void pwmAdjust() {
  while (modepressed == 1) {
    int potprev = f;
    f = pot.read();
    if (f != potprev) {
      int x = f * 100;
      pc.printf("Duty cycle: %2.0f%%\n", x);
      speed = f;
    }
    if(mode == 1){
        modepressed = 0;
    }
  }
}

void startstoppressed() {
  if (startpressed == 0) {
    startpressed = 1;
  }
  if (startpressed == 1) {
    stop;
  }
}

void modebtnpressed() { modepressed = 1; }

int main() {
  speed.period(1.3f);
  startstop.rise(startstoppressed);
  mode.rise(modebtnpressed);
  while (1) {
    if (startpressed == 1) {
      // cny switch case------
      switch (cny) {
      case 0:
        wittelijn = 1;
      default:
        wittelijn = 0;
      }
      //---------------------
      if (wittelijn == 1) {
        if (achteruitcount < 4) {
          achteruit;
          achteruitcount++;
          wait_us(1 * 10 ^ (9));
        } else {
          achteruitcount = 0;
        }
      } else if (wittelijn == 0 && dir1 == 0) {
        vooruit;
      }
    }
  }
}
//test