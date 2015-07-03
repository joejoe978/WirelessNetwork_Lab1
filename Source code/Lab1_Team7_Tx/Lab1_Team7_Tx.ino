/*
  Wireless Networking Fundamentals and Applications - Lab1
  Team 7
  VLC
  Yen Su,MH Yang,WC Lin,Joe Haung,Jack Kao
*/

#define BASEFREQ 11450 
#define DELTA 20 
#define INIT_HERZ 350 //350Hz for 0000, 650Hz  for 11111
#define DELAY 33

char ledR, ledL, tmp;

void setup() {
	Serial.begin(9600);
	
	// use Timer1 for left LED
	pinMode(11, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(9, OUTPUT);
	TCCR1A = _BV(COM1A0) | _BV(COM1B0) | _BV(COM1C0);
	TCCR1B = _BV(WGM12)  | _BV(CS10);

        // Timer3 for right LED
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(3, OUTPUT);
	TCCR3A = _BV(COM3A0) | _BV(COM3B0) | _BV(COM3C0);
	TCCR3B = _BV(WGM32)  | _BV(CS30);

	OCR1A = BASEFREQ;
	OCR1B = 0;
	OCR1C = 0;

        OCR3A = OCR1A;
	OCR3B = 0;
	OCR3C = 0;
}

/*
  Input frequency to calculate OCRnA
*/
int calOCRnA(int hertz) {
  int result; 
  result = 16000000 / 2 / hertz;
  return result;
}

void loop(){
	if(Serial.available() > 0){
		char incomingByte = Serial.read();
                tmp = incomingByte;
                        
                //Divide into two 4bits
                ledL = tmp >> 4;
                ledR = tmp;

                //Mask
                ledL &= 0x0F;
                ledR &= 0x0F;

                OCR1A = calOCRnA(INIT_HERZ + ledL * DELTA);
                OCR3A = calOCRnA(INIT_HERZ + ledR * DELTA);

                //Magic delay=>33.5ms
                delay(DELAY);
                delayMicroseconds(500);
	}
	OCR1A = BASEFREQ;
        OCR3A = OCR1A;
}

