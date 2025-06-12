// Program VTv3.11 - autor: Samuel Balazs Marec 2023
// Vysledkova Tabula pre sportove ucely (futbal) konkretne na zapis aktualneho skore pre divakov

// Zadefinujem si kniznice a premenne ktore pouzivam v programe
#include <FastLED.h>
#include <Bounce2.h>
#define POCET_LEDIEK_NA_CISLICU 61
#define POCET_VSETKYCH_LED 122
#define DATA_PIN_L 2
#define DATA_PIN_P 3

// Unikatna matica binarnych cisiel pre LED pas 1-19 v zavislosti od postupnosti LEDiek v LED pase
const uint64_t digits[20] = {
  0b0000000001111111111111111111111111111111111111111000000000000000, // 0
  0b0000000000000001111111111111100000000000000000000000000000000000, // 1
  0b0001111111111111111111000000011111111111110000000000000000000000, // 2
  0b0001111111111111111111111111111111100000000000000000000000000000, // 3
  0b0001111110000001111111111111100000000000001111111000000000000000, // 4
  0b0001111111111110000000111111111111100000001111111000000000000000, // 5
  0b0001111111111110000000111111111111111111111111111000000000000000, // 6
  0b0000000001111111111111111111100000000000000000000000000000000000, // 7
  0b0001111111111111111111111111111111111111111111111000000000000000, // 8
  0b0001111111111111111111111111111111100000001111111000000000000000, // 9
  0b0000000001111111111111111111111111111111111111111111111111111111, // 10
  0b0000000000000001111111111111100000000000000000000111111111111111, // 11
  0b0001111111111111111111000000011111111111110000000111111111111111, // 12
  0b0001111111111111111111111111111111100000000000000111111111111111, // 13
  0b0001111110000001111111111111100000000000001111111111111111111111, // 14
  0b0001111111111110000000111111111111100000001111111111111111111111, // 15
  0b0001111111111110000000111111111111111111111111111111111111111111, // 16
  0b0000000001111111111111111111100000000000000000000111111111111111, // 17
  0b0001111111111111111111111111111111111111111111111111111111111111, // 18
  0b0001111111111111111111111111111111100000001111111111111111111111, // 19
};
// Vstupne/ovladacie piny pre Arduino Nano
const byte PlusPinL = 6;
const byte ResetPinL = 5;
const byte MinusPinL = 4;
const byte PlusPinP = 9;
const byte ResetPinP = 8;
const byte MinusPinP = 7;

// Definujeme si "integer" hodnotu pre zobrazenie skore
int SkoreL;
int SkoreP;

// Definicia Ledpasu (WS2812B) podla poziadaviek kniznice (FastLED.h)
CRGB leds[POCET_VSETKYCH_LED];

// Definicia tlacidiel podla kniznice (Bounce2.h)
Bounce2::Button StlacPlusL = Bounce2::Button();
Bounce2::Button StlacMinusL = Bounce2::Button();
Bounce2::Button StlacResetL = Bounce2::Button();
Bounce2::Button StlacPlusP = Bounce2::Button();
Bounce2::Button StlacMinusP = Bounce2::Button();
Bounce2::Button StlacResetP = Bounce2::Button();

// Udavam funkcie zobrazovania jednotlivych lediek v LED pase
void nastavCislo(int display, int val, CHSV colour) 
{
	for (int i=0; i<POCET_LEDIEK_NA_CISLICU; i++)
  {
		colour.v = bitRead(digits [val], i) * 255;
		leds[display*POCET_LEDIEK_NA_CISLICU +i] = colour;
	}
}
// Nastavenie programu pre (VTv3.0)
void setup ()
{
  // Nastavime hodnoty pravej a lavej cislice na hodnotu 0
  SkoreL = 0;
  SkoreP = 0;
  // Inicializujeme si LED pas
  FastLED.addLeds<WS2812B, 2>(leds, 0, 61), GRB>(leds, POCET_VSETKYCH_LED);
  FastLED.addLeds<WS2812B, 3>(leds, 61, 122), GRB>(leds, POCET_VSETKYCH_LED);
  // Nakonfigurujeme funkciu tlacidiel
	StlacPlusL.attach(PlusPinL, INPUT_PULLUP);
	StlacResetL.attach(ResetPinL, INPUT_PULLUP);
  StlacMinusL.attach(MinusPinL, INPUT_PULLUP);
  StlacPlusP.attach(PlusPinP, INPUT_PULLUP);
	StlacResetP.attach(ResetPinP, INPUT_PULLUP);
  StlacMinusP.attach(MinusPinP, INPUT_PULLUP);
}
// Samotny program pre (VTv3.0)
void loop() {
	// Kontrolujem stlacenie ovladacich tlacidiel
	StlacPlusL.update();
	StlacMinusL.update();
  StlacResetL.update();
  StlacPlusP.update();
	StlacMinusP.update();
  StlacResetP.update();
	
	// Funkcia pre Cislo Lave
  if(StlacPlusL.pressed())
   {
     SkoreL++; // pridam 1 na lavu cislicu
     if (SkoreL >= 20) SkoreL = 19;
     delay(400);
   }
  else if (StlacMinusL.pressed())
   {
     SkoreL--; // odcitam 1 z lavej cislice
     if (SkoreL <= -1) SkoreL = 0;
     delay(400);
   }
  else if (StlacResetL.pressed()) // resetujem lavu cislicu na 0
   {
     SkoreL = 0;
     delay(400);
   }
   // Funkcia pre Cislo Prave
  if(StlacPlusP.pressed())
   {
     SkoreP++;  // pridam 1 na pravu cislicu
     if (SkoreP >= 20) SkoreP = 19;
     delay(400);
   }
  else if (StlacMinusP.pressed())
   {
     SkoreP--;  // odcitam 1 z pravej cislice
     if (SkoreP <= -1) SkoreP = 0;
     delay(400);
   }
  else if (StlacResetP.pressed()) // resetujem pravu cislicu na 0
   {
     SkoreP = 0;
     delay(400);
   }   
  // Nastavenie hodnoty aj farby cisiel
  nastavCislo(0, SkoreL, CHSV(0, 255, 255)); // poslem na pin 2
  nastavCislo(1, SkoreP, CHSV(0, 255, 255)); // poslem na pin 3
  // Zobraz aktualizovane udaje na LED pase
	FastLED.show();
	delay(20);
}





// koniec programu