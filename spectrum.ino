


/*
  Written by FischiMc and SupaStefe

  This sketch uses a 10x10 RGB LED-Matrix as a spectrum analyzer
  It uses a FTT Library to analyze an audio signal connected to the
  pin A7 of an Arduino nano. Everytime a column gets higher than
  10 pixels the color of each column changes.
*/


#define LIN_OUT 1
#define FFT_N 256         //set to 256 point fft

#include <FFT.h>
#include <math.h>         //include library for mathematic funcions
#include <Dimmer.h>

Dimmer blue(12);
Dimmer green(11);
Dimmer yellow(10);
Dimmer red(9);


//unsigned char grenzen[11] = {0,3,5,7,9,11,13,16,24,32,69};          //borders of the frequency areas
//unsigned char borders[5] = {25, 40, 69, 69, 100};
//unsigned char borders[5] = {16,24, 32, 69, 80};
unsigned char borders[5] = {7, 13, 24, 32, 69};

void setup() {

  //pinMode(3, OUTPUT);
  //pinMode(5, OUTPUT);
 //pinMode(6, OUTPUT);
  //pinMode(9, OUTPUT);
  Serial.begin(115200);                                             //use the serial port

  blue.begin();
  green.begin();
  yellow.begin();
  red.begin();

  blue.set(100);
  delay(100);
  blue.set(0);
  red.set(100);
  delay(100);
  red.set(0);
  green.set(100);
  delay(100);
  green.set(0);
  yellow.set(100);
  delay(100);
  yellow.set(0);
  
  TIMSK0 = 0;                                                       //turn off timer0 for lower jitter
  ADCSRA = 0xe5;                                                    //set the adc to free running mode
  ADMUX = 0x40;                                                      //use pin A0
  DIDR0 = 0x01;                                                     //turn off the digital input for
}

void setLeds(unsigned char W) {
  if (W < 100)
    analogWrite(9, map(constrain(W, 0, 100), 0, 100, 0, 255));
  if (W < 150 && W > 100)
    analogWrite(6, map(constrain(W, 100, 150), 100, 150, 0, 255));
  if (W < 200 && W > 150)
    analogWrite(5, map(constrain(W, 150, 200), 150, 200, 0, 255));
  if (W < 255 && W > 150)
    analogWrite(3, map(constrain(W, 200, 255), 200, 255, 0, 255));
}

void loop() {
  while (1) {                                                       //reduces jitter
    cli();                                                          //UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) {                            //save 256 samples
      while (!(ADCSRA & 0x10));                                     //wait for adc to be ready
      ADCSRA = 0xf5;                                                //restart adc
      byte m = ADCL;                                                //fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m;                                         //form into an int
      k -= 0x0200;                                                  //form into a signed int
      k <<= 6;                                                      //form into a 16b signed int
      fft_input[i] = k;                                             //put real data into even bins
    }

    fft_window();                                                   // window the data for better frequency response
    fft_reorder();                                                  // reorder the data before doing the fft
    fft_run();                                                      // process the data in the fft
    fft_mag_lin();                                                  // take the output of the fft
    sei();

    fft_lin_out[0] = 0;
    fft_lin_out[1] = 0;


    for (unsigned char i = 0; i < 4; i++) {

      unsigned char maxW = 0;
      for (unsigned char x = borders[i]; x < borders[i + 1]; x++) {
        if ((unsigned char)fft_lin_out[x] > maxW) {
          maxW = (unsigned char)fft_lin_out[x];
        }

      }
      unsigned char maxWa = 0;
      unsigned char dim=0;



      //setBalken(i, maxW);
      //Serial.print(i);
      //Serial.print(": ");
      //Serial.print(maxW);
      //setLeds(maxW);

      if (i == 0) {
        maxW = constrain(maxW, 235, 255);
        maxWa = map(maxW, 235, 255, 0, 255);
        if (maxW < 245 ) {
          maxWa = 0;
        }
        maxWa = constrain(maxWa, 0, 255);
        //Serial.print(" Adjusted maxW: ");
        //Serial.println(maxWa);
       // analogWrite(3, maxWa);
       dim =map(maxWa, 235, 255, 0, 100);
       if (maxWa ==0) dim=0;
       green.set(dim);
      }
      else if (i == 1) {
        maxW = constrain(maxW, 220, 255);
        maxWa = map(maxW, 220, 255, 0, 255);
        if (maxW < 235 )
          maxWa = 0;
        maxWa = constrain(maxWa, 0, 255);
        //Serial.print(" Adjusted maxW: ");
        //Serial.println(maxWa);
        //analogWrite(5, maxWa);
        dim = map(maxWa, 220, 255, 0, 100);
        if (maxWa ==0) dim=0;
        blue.set(dim);
        //blue.set(map(maxWa, 0, 255, 0, 100));

      } else if (i == 2) {
        maxW = constrain(maxW, 80, 200);
        maxWa = map(maxW, 80, 200, 0, 255);
        //Serial.print(" Adjusted maxW: ");
        //Serial.println(maxWa);
        //analogWrite(6, maxWa);
       dim =map(maxWa, 80  , 255, 0, 100);
       if (maxWa ==0) dim=0;
       red.set(dim);
        //red.set(map(maxWa, 0, 255, 0, 100));
      } else if (i == 3) {
        maxW = constrain(maxW, 80, 255);
        maxWa = map(maxW, 80, 255, 0, 255);
        if (maxW > 245)
          maxWa = 0;
        maxWa = constrain(maxWa, 0, 255);
        //Serial.print(" Adjusted maxW: ");
        //Serial.println(maxWa);
        //analogWrite(9, maxWa);
        dim =map(maxWa, 80, 245, 0, 100);
       if (maxWa ==0) dim=0;
       yellow.set(dim);
        //yellow.set(map(maxWa, 100, 240, 0, 100));
      }


    }
    //Serial.println("");
    TIMSK0 = 1;
    //FastLED.show();
    TIMSK0 = 0;
  }
}
