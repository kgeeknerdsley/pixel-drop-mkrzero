#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BEEPER 8
#define CNT_IN A1

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define BOX_HEIGHT 58

#define PELLET_NUM 6 //change to increase pellet number, sorta temporary

int adcval;
bool soundPlaying;
int score;
uint8_t strikeCount;
int cycles;
uint8_t pelletsOut;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 'testsprite', 10x5px
const unsigned char box [] PROGMEM = {
  0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0
};

// 'testsprite', 128x64px
const unsigned char splash [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0x1f, 0x7c, 0xf7, 0xfb, 0xe0, 0x7f, 0x07, 0xf0, 0x3f, 0x1f, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0x9f, 0x7e, 0xf7, 0xfb, 0xe0, 0x7f, 0xc7, 0xfc, 0x7f, 0x9f, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xdf, 0x3f, 0xe7, 0xfb, 0xe0, 0x7f, 0xe7, 0xfe, 0xff, 0x9f, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0xdf, 0x1f, 0xc7, 0xfb, 0xe0, 0x3f, 0xf7, 0xfe, 0xff, 0xdf, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0xdf, 0x1f, 0xc7, 0xfb, 0xe0, 0x3f, 0xf7, 0xfe, 0xff, 0xdf, 0xf8, 0x00, 0x00, 
  0x00, 0x00, 0x7d, 0xdf, 0x0f, 0x87, 0xc3, 0xe0, 0x3e, 0xf7, 0xde, 0xf3, 0xdf, 0x78, 0x00, 0x00, 
  0x00, 0x00, 0x79, 0xdf, 0x0f, 0x87, 0xfb, 0xe0, 0x3e, 0x77, 0xde, 0xf3, 0xdf, 0x70, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0xdf, 0x0f, 0x87, 0xf3, 0xe0, 0x3e, 0x77, 0xfc, 0xe3, 0xdf, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0x9f, 0x0f, 0xc7, 0xc3, 0xe0, 0x3c, 0xf7, 0xf8, 0xf3, 0xdf, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x7c, 0x1f, 0x1f, 0xc7, 0xc3, 0xe0, 0x3f, 0xe7, 0xf8, 0xff, 0x9f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x78, 0x1f, 0x1d, 0xe7, 0xfb, 0xfc, 0x3f, 0xc7, 0xfc, 0x7f, 0x1f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x78, 0x1f, 0x39, 0xe7, 0xf3, 0xf8, 0x3f, 0x87, 0xdc, 0x3e, 0x1f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x60, 0x00, 0x22, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x31, 0x00, 0x04, 0x00, 0x60, 0x00, 0x26, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x31, 0x00, 0x0c, 0x00, 0x60, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x33, 0x3c, 0x7c, 0xf0, 0x7c, 0x88, 0x28, 0x7b, 0x3c, 0xf8, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3b, 0x82, 0x45, 0x98, 0x66, 0x98, 0x38, 0x89, 0x24, 0x88, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x2a, 0x9e, 0x85, 0xf8, 0x62, 0xd0, 0x3c, 0xfd, 0x64, 0x88, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x2c, 0xa2, 0xc5, 0x00, 0x62, 0x50, 0x24, 0x80, 0xc4, 0x88, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x6c, 0xa2, 0x4d, 0x80, 0x66, 0x60, 0x26, 0x80, 0xc4, 0x88, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x44, 0xbe, 0x7c, 0xf0, 0x7c, 0x20, 0x23, 0x78, 0x84, 0x88, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int pelletPos[PELLET_NUM][2] = {
  {random(20,114),random(0,10)},
  {random(20,114),random(0,10)},
  {random(20,114),random(0,10)},
  {random(20,114),random(0,10)},
  {random(20,114),random(0,10)},
  {random(20,114),random(0,10)}
  };

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  pinMode(7, OUTPUT);
  pinMode(CNT_IN, INPUT);

  digitalWrite(7, HIGH);

  display.clearDisplay();
  display.drawBitmap(0,0,splash,128,64,SSD1306_WHITE);
  display.display();
  splashSong();

  delay(1000);
  

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  
  display.clearDisplay();
  //display.display();

  //splash screen somewhere here

  adcval = 0;
  soundPlaying = false;
  score = 0;
  strikeCount = 0;
  pelletsOut = 1;
  cycles = 0;

  randomSeed(analogRead(A2)); //uses analog noise to get new seed
  
  startupSong();
}

void loop() {
  //handle input changes
  adcval = analogRead(CNT_IN);

  if(adcval == 0) {
    adcval = 1;
  }
  adcval = adcval / 8;
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

  //paddle update
  if(adcval < 2) {
    display.drawBitmap(0,BOX_HEIGHT,box,10,5,SSD1306_WHITE);
  } else if(adcval > 114) {
    display.drawBitmap(114,BOX_HEIGHT,box,10,5,SSD1306_WHITE);
  } else {
    display.drawBitmap(adcval,BOX_HEIGHT,box,10,5,SSD1306_WHITE);
  }

  if(cycles == 29) { //300 cycles have passed, 300*100 = 30000 / 1000 = 30s
    if(pelletsOut != PELLET_NUM) {
      pelletsOut++;
    cycles = 0;
    }
  }
  
  updatePellets(adcval,pelletsOut);
  
  drawStrikes();
  
  if(strikeCount == 4) {
    gameOver();
  }

  cycles++;

  

  //for loop that iterates over array of Pellets
  //as time progresses, for loop bounds expand to give more pellets
  //iterates over each pellet, updating position and checking score

  display.display(); //only do this when all drawing is done

//  if(adcval < 20) {
//    sound(50);
//  } else if(adcval > 100) {
//    sound(500);
//  } else if(adcval < 518 && adcval > 508) {
//    sound(225);
//  } else {
//    stopSound();
//  }
  
  delay(50);
}

//pelletPos[pellet][0] = X coordinate
//pelletPos[pellet][1] = Y coordinate
void updatePellets(int paddlePos, int pelletsOut) {
  for(int pellet = 0; pellet < pelletsOut; pellet++) {//for each pellet
    
    if(pelletPos[pellet][1] >= BOX_HEIGHT || pelletPos[pellet][1] >= BOX_HEIGHT + 5) { //this is when the pellet would contact the box
      
      if(pelletPos[pellet][0] >= paddlePos && pelletPos[pellet][0] < paddlePos + 10) { //if pellet x is greater than box's left hand side and less than box's right hand side, we made it in
        score++; //increase score
        pelletPos[pellet][0] = random(20,114); //reset x position somewhere new
        pelletPos[pellet][1] = random(0,10); //reset y to top
        display.drawPixel(pelletPos[pellet][0],pelletPos[pellet][1],SSD1306_WHITE); //draw new pellet position
        sound(500, 100);
        //delay(100*1.3);
//        Serial.print(pelletPos[pellet][0]);
//        Serial.print(",");
//        Serial.println(pelletPos[pellet][1]);
      }
    } 
      
    if(pelletPos[pellet][1] > 68) { //if we missed, add a strike
      strikeCount++;
      pelletPos[pellet][0] = random(20,114);
      pelletPos[pellet][1] = random(0,10);
      sound(100, 100);
        
    }

    //pellet will always be dropping
    pelletPos[pellet][1] += 2; //change this for faster drop
    display.drawPixel(pelletPos[pellet][0],pelletPos[pellet][1],SSD1306_WHITE);
  }
}

void drawStrikes() {
  display.setCursor(80, 0);
  for(int i = 0; i < strikeCount; i++) {
    display.print("X ");
  }
}

void gameOver() {
  display.clearDisplay();
  
  display.println("Game over!");
  display.print("Final Score: ");
  display.println(score);
  display.println();
  display.println("To restart, press RST on board");

  display.display();

  sound(100, 2000);

  while(1);
}

void startupSong() {
  for(int i = 0; i < 2; i++) {
    tone(BEEPER, 100, 200);
    delay(200 * 1.3);
    tone(BEEPER, 300, 200);
    delay(200*1.3);
  }
}

void splashSong() {
  sound(100, 200);
  delay(200 * 1.3);
  sound(200, 100);
  delay(100 * 1.3);
  sound(500, 100);
  delay(100 * 1.3);
}

//OVERLOADED SOUND FUNCTIONS
void sound(int frequency, int duration) {
  if(!soundPlaying) {
    soundPlaying = true;
    tone(BEEPER, frequency, duration);
    soundPlaying = false;
  }
}

void sound(int frequency) {
  if(!soundPlaying) {
    soundPlaying = true;
    tone(BEEPER, frequency);
  }
}

void stopSound() {
  noTone(BEEPER);
  soundPlaying = false;
}
