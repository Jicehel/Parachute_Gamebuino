
/*------------------------------------------
Parachute remake for Makebuino
v0.3 by Jicehel Date:05/08/017
Based on Nintendo Game & Watch Parachute
Thanks to Luis Dominguez - LADBSoft.com
for the examples with “Firebuino”
--------------------------------------------
Last changes:
- Add full graphics
- Add Helico animation
- Add Paratroopers animation
- Add Misses and swimmer animation
- Add score
--------------------------------------------
To do:
- Change and adjust speed quickest when the score growth             
------------------------------------------*/
#include <Gamebuino.h>
#include <EEPROM.h>
#include <SPI.h>
Gamebuino gb;
 
int  score;
long  highscore;
short misses;
short manage_joystick;
short BoatPosition;
short moveTick;
short spawnDelay;
short spawnCount;
short Nb_Parachutes_launched;
short Position_Parachute[10];
short Colonne_Para;
short Position_Para;
short shark_anim;
short helico_anim;
short vitesse_pales;
byte  gameState;
byte  speedmax;
short x;
int temp;

//  const int JoyY_pin = A5; // Analog pin connected to joystick's Y axis output (not used in parachute)
const int JoyX_pin = A4;   // Analog pin connected to joystick's X axis output
int JoyX_pos = 0; // Variable to store position of joystick's Y axis
 
const byte barque[] PROGMEM = { 16,7,
B00111000,B00000000,B01011100,B00000000,B01111000,B00000000,B00110000,B00000000,B11011100,B00000000,B11111111,B11111111,B11001111,B11111110,};
 
const byte requin_m[] PROGMEM = { 16,6,
B00000000,B00111000,B11000001,B11111000,B11110110,B11010000,B11111111,B10000000,B01111110,B00110000,B11111111,B11110000,};
 
const byte aileron[] PROGMEM = { 8,8,
B00010000,B01110000,B01110000,B11110000,B00000000,B00000000,B00000000,B00000000,};
 
const byte titleScreenBitmap[] PROGMEM = {80,32,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x0,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x0,0x0,0x0,
0x1,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x0,0x0,0x0,0x3,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x0,0x0,0x0,0x3,0xE0,0x0,0x0,0x1,0xFF,0xF0,0x0,0x0,0x0,0x3,0xE0,0x0,0x4F,0x9,0xFD,0xF0,0x0,0x0,0x0,
0x3,0xE0,0x0,0x3F,0x80,0xAB,0xF0,0x0,0x0,0x0,0x3,0xE1,0xE0,0xE7,0xC2,0xFF,0xF0,0x0,0x0,0x0,0x3,0xFF,0xF1,0xFF,0xE1,0x29,0xF0,0x0,0x0,0x0,0x3,0xFF,0x81,0x89,0xE3,0xFD,0xF0,0x0,0x0,0x0,
0x3,0xFF,0x81,0xC9,0xE1,0xFF,0xF0,0x0,0x0,0x0,0x3,0xFF,0x81,0x4E,0x20,0xF9,0xF0,0x0,0x0,0x0,0x3,0xF6,0xE0,0x36,0x50,0x67,0xF0,0x0,0x0,0x0,0x3,0xF0,0x0,0x1F,0xA0,0x27,0xF0,0x0,0x0,0x0,
0x3,0xFC,0x0,0x7D,0x0,0xF,0xF0,0x0,0x0,0x0,0x3,0xFC,0x0,0x37,0x0,0x1F,0xF0,0x0,0x0,0x0,0x3,0xF8,0x7B,0x22,0x1,0xFF,0xF0,0x0,0x0,0x0,0x3,0xF8,0x3,0xF0,0x0,0x7,0xF0,0x0,0x0,0x0,
0x3,0xFF,0x80,0x80,0x0,0x7,0xF0,0x0,0x0,0x0,0x3,0xF8,0x0,0x0,0x19,0x87,0xF0,0x0,0x0,0x0,0x3,0xE0,0x0,0x1,0xF2,0x87,0xF0,0x0,0x0,0x0,0x3,0xF0,0x72,0x80,0x78,0x1,0xF0,0x0,0x0,0x0,
0x3,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x0,0x0,0x0,0x3,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x0,0x0,0x0,0x1,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x0,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,};
 
const byte Helice[] PROGMEM = {8,1,B11111000,};
 
const byte Helico[] PROGMEM = {24,10,
B00000100,B00000000,B00000000,B00000100,B00000100,B00000000,B00001110,B00000100,B00000000,B00011111,B00011111,
B00000000,B00011111,B11111001,B10000000,B00011110,B10101000,B11000000,B00001110,B10101111,B11000000,B00000111,
B11111111,B10000000,B00000011,B00000110,B00000000,B00000011,B00000110,B00000000,};
 
const byte Noye_1[] PROGMEM = {16,5,B01000100,B01000000,B00001110,B00000000,B10101010,B10000000,B00011111,B00000000,B00001110,B00000000,};
 
const byte Noye_2[] PROGMEM = {16,5,B00000000,B01000000,B01001110,B00000000,B00001101,B01000000,B00111111,B00000000,B00001110,B00000000,};
 
const byte Para_1[] PROGMEM = {8,8,B00000100,B00100111,B00110011,B10011111,B11011100,B01111111,B00110010,B01100000,};
 
const byte Nageur[] PROGMEM = {8,5,B01000000,B11101110,B11111111,B00011110,B01111100,};
 
const byte Parra_m[] PROGMEM = {8,5,B00001100,B10110110,B10000110,B11111100,B00111100,};
 
const byte Para_2[] PROGMEM = {16,11,
B00000000,B00011000,B00000000,B00111000,B00011100,B01110000,B00111100,B11100000,B10011101,B11000000,B01101111,
B00000000,B00011110,B00000000,B01111111,B00000000,B11011001,B10000000,B00110000,B00000000,B01100000,B00000000,};
 
const byte Para_3[] PROGMEM = {16,15,
B00000001,B10000000,B00000011,B10000000,B00000111,B10000000,B00001101,B00000000,B00011011,B00000000,B00011110,
B00000000,B00000100,B00000000,B00111000,B00000000,B00111100,B00000000,B00111000,B00000000,B10011011,B00000000,
B01111100,B00000000,B00110000,B00000000,B01010000,B00000000,B11011000,B00000000,};
 
const byte Para_4[] PROGMEM = {16,17,
B00001111,B11100000,B00111011,B11111000,B01110011,B11011100,B11100111,B11001110,B11111111,B11001110,B11100100,
B11111110,B10000100,B01001110,B01000100,B01000010,B00100100,B11000100,B00010111,B11001000,B00001101,B11010000,
B00000111,B11100000,B00000001,B10010000,B00000001,B11110000,B00011011,B10000000,B00001100,B11100000,B00000000,B00110000,};
 
const byte Para_5[] PROGMEM = {24,18,
B00000011,B11100000,B00000000,B00011111,B11111000,B00000000,B00111001,B11111110,B00000000,B01110001,B11111111,
B00000000,B01110011,B11111011,B10000000,B11111111,B11111001,B10000000,B11000111,B01111001,B11000000,B10000010,
B00011111,B11000000,B01000010,B00001011,B11000000,B00100011,B00001000,B11000000,B00010111,B10010000,B01000000,
B00001011,B10010001,B10000000,B00100101,B10111110,B00000000,B00110111,B01100000,B00000000,B00011111,B11100000,
B00000000,B00100011,B00000000,B00000000,B00111111,B11110000,B00000000,B00110000,B01100000,B00000000,};
 
const byte Miss[] PROGMEM = {8,5,B11000000,B11100100,B11111100,B01111100,B11001100,};
 
#define NUM_SPRITES 6
const byte* shark[NUM_SPRITES] = {
  aileron, //0
  aileron, //1
  aileron, //2
  aileron, //3
  aileron, //4
  requin_m //5
};
 
const byte subBackgroundBitmap[] PROGMEM = {88,48,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00010000,
B11101001,B10001000,B10110101,B01111011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B10110101,B01010101,B01110101,B01010010,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B11011101,B10011101,B00111101,B01010011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B10010101,B01010101,B01110101,B01010010,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B10010101,B01010100,B10110101,B11010011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00011010,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B01101110,B10100111,B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B11111111,B11111111,B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B11111111,B11111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B01111111,B11111101,B01110100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B11111111,B01001000,B00010000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000100,B10110000,
B10011100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000001,B11001101,B10110000,
B00011000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00011110,B11111111,B11110000,
B00111000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B11111111,B11110000,
B00110000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000001,B11111111,B11111111,B11110000,
B01110000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000010,B01111111,B01111111,B11100000,
B01100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01000100,B11111011,B10110000,
B01100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000001,B11111001,B11000000,
B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B10101000,B11000000,
B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00100000,B11100000,
B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11110000,
B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11110000,
B11110000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000111,B11110000,
B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00011111,B11110000,
B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00111111,B11110000,
B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111111,B11110000,
B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111111,B11110000,
B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111111,B11110000,
B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000111,B11110000,
B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000001,B11110000,
B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000001,B11110000,
B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11110000,
B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11110000,
B11111000,B00000000,B00000000,B00000000,B00000000,B00111000,B00000000,B00000000,B00000000,B00000000,B01110000,
B11110000,B11110011,B00011110,B01100001,B11111001,B11111100,B11001111,B11100000,B00010000,B00100000,B11110000,
B00000000,B00000100,B00000000,B00000000,B00000000,B01000000,B00000000,B00110000,B00000000,B00000000,B00000000,
B11100000,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00001110,B00000000,B00000000,B00000000,B00000000,B00001000,B00000000,B00000011,B11110001,B11000011,B11100000,
B11110001,B11100000,B00000000,B00000000,B00000000,B00001000,B00000000,B00000000,B00001111,B00111111,B00000000,
B00001110,B00000000,B00000000,B01111100,B00001111,B00111110,B00000000,B00000000,B00000000,B11000000,B00000000,
B11000000,B01111100,B00000000,B00000000,B00000000,B00001000,B00000000,B00000000,B00000000,B00000001,B11110000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00001000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,
B00000000,B00000001,B11111001,B11100001,B11110011,B11000111,B11111100,B00001111,B10000000,B00000000,B00000000,
B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00010000,};
 
 
void initGame() {
  gb.pickRandomSeed();
  misses = 0;
  score = 0;
  BoatPosition = 1;
  spawnDelay = 4; // Délai avant autorisation d'envoi de parachute
  shark_anim = 0; 
  speedmax = 25;   // Nombre de cycles avant mise à jour de la boucle
  helico_anim = 0;
  vitesse_pales = 5;
  moveTick = speedmax;
  spawnCount = spawnDelay;
  manage_joystick = 0;    
}
 
void setup() {
  gb.begin();
  initGame();
  //  gb.display.setFont(font5x7); //change the font to the large one
  gb.titleScreen(F("Parachute"), titleScreenBitmap);
  gb.pickRandomSeed(); //pick a different random seed each time for games to be different
  gb.battery.show = false;
 
  //############################################################
  //  pinMode(JoyY_pin, INPUT); //Configures pin on microcontroller
  pinMode(JoyX_pin, INPUT);
  //############################################################
 
  initGame();
}
 
void anim_shark() {
  switch (shark_anim) {
      case 0 :
        break;
      case 1 : 
        gb.display.drawBitmap(62 , 38, shark[shark_anim - 1]);
        break;  
      case 2 : 
        gb.display.drawBitmap(46 , 38, shark[shark_anim - 1]);
        break;
      case 3 :
        gb.display.drawBitmap(32 , 38, shark[shark_anim - 1]);
        break;
      case 4 :
        gb.display.drawBitmap(5 , 42, shark[shark_anim - 1],0,FLIPH);
        break;
      case 5 :
        gb.display.drawBitmap(24 , 42, shark[shark_anim - 1],0,FLIPH);
        break;
      case 6 :
        gb.display.drawBitmap(48 , 40, shark[shark_anim - 1]);
        break;
      case 7: 
        // Traiter la fin de l’animation du requin
        shark_anim = 0;
        break;
  }
    if (moveTick <= 0)
      if ((shark_anim > 0) || ((shark_anim == 0) && (random(50) < 5))){
         shark_anim++;
         // gb.sound.playTick(); 
      }
}
 
void anim_helico() {
  vitesse_pales--;
  if (vitesse_pales == 0) {
   switch(helico_anim) {
     case 0:
        gb.display.drawBitmap(65 , 0, Helice);
        gb.display.drawBitmap(71 , 0, Helice);
        break;
      case 1 : 
        gb.display.drawBitmap(65 , 0, Helice);
        gb.display.drawBitmap(71 , 0, Helice);
        gb.display.drawBitmap(73 , 1, Helice);
        gb.display.drawBitmap(79 , 1, Helice);
        break;
      case 2 : 
        gb.display.drawBitmap(73 , 1, Helice);
        gb.display.drawBitmap(79 , 1, Helice);
        break;
      case 3 : 
        helico_anim = 0;
        break;
  }
  helico_anim++;  
  vitesse_pales = 5;
  }
}
 
void anim_para() {
  int   compteur;
  for (compteur = 1 ; compteur <= Nb_Parachutes_launched ; compteur++)  {
          Colonne_Para = Position_Parachute[compteur] / 10;
          Position_Para = Position_Parachute[compteur] - Colonne_Para*10;
          Dessine_Para(Colonne_Para,Position_Para, compteur);
          if (moveTick==0) {
          if ((Position_Para >= 4+Colonne_Para) && (Colonne_Para <=3) ) Test_Barque(Colonne_Para,compteur);
          else Position_Parachute[compteur]++ ;
          }
    }  
}
 
void Dessine_Para(short ColX, short PosY, short Parachute) {
  int count;
  // Dessine le para en fonction de sa colonne (1 à gauche et 3 à droite) et de sa position
  if (ColX < 4) {
    switch(PosY) {
      case 1:
        gb.display.drawBitmap(72 - 6*ColX  , 9 - 3*ColX, Para_1);
        break;
      case 2:
        gb.display.drawBitmap(67 - 7*ColX  , 11 - 3*ColX, Para_2);
        break;
      case 3:
        gb.display.drawBitmap(56 - 6*ColX  , 14 - 5*ColX, Para_3);
        break;
      case 4:
        gb.display.drawBitmap(54 - 8*ColX  , 15 - (ColX-1)*4, Para_4);
        break; 
      case 5:
        gb.display.drawBitmap(62 - 16*ColX  , 12 + (3 - ColX)*3, Para_5);
        break; 
      case 6:
        if (ColX==2) gb.display.drawBitmap(29 , 17, Para_5);
        else gb.display.drawBitmap(16 , 1é, Para_5) ;
        break;
      case 7:
        gb.display.drawBitmap(13 , 17, Para_5);
        break;
      case 8:
        break;
    }
  } else {
    switch(PosY) {
      case 1:
        gb.display.drawBitmap(48 , 39, Noye_1);
        break;
      case 2:
        gb.display.drawBitmap(30 , 39, Noye_2);
        break;
      case 3:
        gb.display.drawBitmap(14, 39, Noye_2,0,FLIPH);
        break;
      case 4:
        gb.display.drawBitmap(21, 43, Nageur);
        break; 
      case 5:
        gb.display.drawBitmap(37, 43, Nageur);
        break; 
      case 6:
        gb.display.drawBitmap(62, 43, Parra_m);
        break;
      case 7:
        for (count = Parachute; count < Nb_Parachutes_launched ; count++) {
          Position_Parachute[count] = Position_Parachute[count+1];
        }
        Nb_Parachutes_launched-- ;
        break;
    }
  }
}
 
void Test_Barque(int colonne, int para_courant) {
int count;
// Test si la barque est sous le para => score ou passe le para en mode nageur (colonne théorique: 4) ...
  if (colonne == 4 - BoatPosition ) {
    score ++;
    for (count = para_courant; count < Nb_Parachutes_launched ; count++) {
       Position_Parachute[count] = Position_Parachute[count+1];
    }
    Nb_Parachutes_launched --;
  } else {
     misses++;
     Position_Parachute[para_courant] = 4*10 + colonne;  
     shark_anim = colonne;
     moveTick = 0;
     if (Nb_Parachutes_launched > 1) {
       for (count = 2; count <= Nb_Parachutes_launched ; count++) Position_Parachute[count] = 0;
       Nb_Parachutes_launched == 1;
     }
     spawnCount = spawnDelay;
  }
}
 
void loop() {
  if (gb.update()) {
    // Gestion du Joystick                              // Comments
    if(analogRead(JoyX_pin) <= 600)       JoyX_pos = 1; // Joystick is in left position
    else if(analogRead(JoyX_pin)>= 700)   JoyX_pos = 2; // Joystick is in right position 
         else JoyX_pos = 0;                            // Joystick is inside dead spot at or near its home positon

    //pause the game if C is pressed
    if(gb.buttons.pressed(BTN_C)){
     gb.titleScreen(F("Parachute"), titleScreenBitmap);
      gb.battery.show = false;
      // gb.display.fontSize = 2;
    }
 
    gb.display.drawBitmap(0, 0, subBackgroundBitmap);
    
    // Animation de la barque
    if(gb.buttons.pressed(BTN_LEFT) || gb.buttons.pressed(BTN_A) || (JoyX_pos == 1 && manage_joystick== 1)){
        if (BoatPosition > 1) BoatPosition--;
    }
    if(gb.buttons.pressed(BTN_RIGHT) || gb.buttons.pressed(BTN_B) || (JoyX_pos == 2 && manage_joystick== 1)){
        if (BoatPosition < 3) BoatPosition++;
    }
    if (BoatPosition < 1) BoatPosition = 1;
    if (BoatPosition > 3) BoatPosition = 3;
    gb.display.drawBitmap(BoatPosition * 16 - 8 , 30, barque);

    if (moveTick > 0) moveTick --;
    else {
          spawnCount--;
    if ((spawnCount < 1) && (random(6 - (score / 200))< 2)) {
      temp = int(random(3))+1;
      Nb_Parachutes_launched++;
      Position_Parachute[Nb_Parachutes_launched] = temp*10+1;
      spawnCount = spawnDelay - int(score / 100); 
    }
       moveTick = speedmax - (score / 100);
    }
    gb.display.drawBitmap(65 , 0, Helico);

    anim_shark();  // Animation du requin
    anim_para();   // Animation du parachutiste
    anim_helico(); // Animtion de l'hélicoptère
    gb.display.setColor(BLACK,WHITE);
    gb.display.cursorX = 0;
    gb.display.cursorY = 0;
    gb.display.print("Score :    ");
    gb.display.cursorX = 30;
    gb.display.println(score);
    // Suit une variable pour debuggage
    // gb.display.print("         ");
    // gb.display.cursorX = 30;
    // gb.display.println(Nb_Parachutes_launched);
  }  
}