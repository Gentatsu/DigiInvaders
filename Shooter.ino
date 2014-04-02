//Author : Naval Bhandari 
//Email  : navs92@hotmail.com
//Left and right to move player, "A" button to shoot, now kill the invaders! 
#include <DigiPixel.h>
#include <avr/pgmspace.h>

// leave the following line uncommented for use with a Digispark
//DigiPixel digiPixel(3,0,5,2,1);  // LED Latch/Button Shift !load pin, LED/Button clock pin, LED Data Pin, LED Output Enable pin, Buttons data pin)

// leave the following line uncommented for use with an Arduino
DigiPixel digiPixel(5,2,6,4,3);  // LED Latch/Button Shift !load pin, LED/Button clock pin, LED Data Pin, LED Output Enable pin, Buttons data pin)

#define playerSpeedSave 10; //Lower is faster
#define bulletSpeedSave 6;
#define invaderSpeedSave 80;


byte playerX = 4;
byte playerSpeed = playerSpeedSave; 
byte bulletSpeed = bulletSpeedSave;
byte invaderSpeed = invaderSpeedSave;
boolean bulletShot = false;
byte bulletX = 0; 
byte bulletY = 0;
byte invaderX = random(0,7); 
byte invaderY= 7;
boolean invaderDead = false;
boolean gameOver = false;
byte killed = 0;

byte numberTable[30]PROGMEM = {0b01111110, 0b01000010, 0b01111110, 0b00100010, 0b01111110, 0b00000010, 0b01001110, 0b01001010, 0b01111010, 0b01000010, 0b01001010, 0b01111110, 0b01111000, 0b00001000, 0b01111110, 0b01111010, 0b01001010, 0b01001110, 0b01111110, 0b01001010, 0b01001110, 0b01000000, 0b01000000, 0b01111110, 0b01111110, 0b01001010, 0b01111110, 0b01111010, 0b01001010, 0b01111110};


void setup()
{
  digiPixel.setPixel(playerX, 0, 1);
}

void loop()
{
  check();
  saveGraphics();
  digiPixel.saveButtonStates(); 
  digiPixel.drawScreen(); 
}

void check()
{
  playerSpeed --;
  if (playerSpeed == 0)
  {
    if (digiPixel.buttonAPressed)
    {
      if (!bulletShot)
      {
        bulletShot = true;
        bulletX = playerX;
        bulletY = 1;
      }
    }
    if (digiPixel.buttonLeftPressed && playerX > 0)
    {
      playerX--;
    }
    else if (digiPixel.buttonRightPressed && playerX < 7)
    {
      playerX++;
    }
    playerSpeed = playerSpeedSave;
  }
  if (bulletShot)
  {
    bulletSpeed --;
    if (bulletSpeed == 0)
    {
      if (bulletY < 7)
      {
        bulletY++;
      }
      else 
      {
        bulletShot = false;
      }
      bulletSpeed = bulletSpeedSave;
    }
  } 
  if (!invaderDead)
  {
    invaderSpeed --;
    if (invaderSpeed == 0)
    {
      if (invaderX % 2 == 0)
      {
        invaderX ++;
      }
      else
      {
        invaderX --;
      }
      if (invaderY > 1)
      {
        invaderY --;
      }
      else
      {
        gameOver = true;
      }
      invaderSpeed = invaderSpeedSave;
    }
    if (bulletX == invaderX && bulletY == invaderY)
      {
        invaderDead = true;
        bulletShot = false;
        gameOver = true;
        killed ++;
      } 
  }
  if (gameOver)
  {
    //Reset invader
    invaderX = random(0,7); 
    invaderY= 7;
    invaderDead = false;
    gameOver = false;
  }
}

void saveGraphics(){
  digiPixel.clearScreen();
  digiPixel.setPixel(playerX, 0,1);
  if (bulletShot)
  {
    digiPixel.setPixel(bulletX, bulletY, 2);
  }
  if (!invaderDead)
  {
    digiPixel.setPixel(invaderX, invaderY, 3);
  }
}


void showDeath()
{
    digiPixel.clearScreen();
    digiPixel.drawScreen();

    int scoreTens = killed / 10;
    int scoreOnes = killed - (scoreTens * 10);

    for (int index = 0; index < 3; index++)
    {
      digiPixel.bufferBlue[index] = pgm_read_dword(&numberTable[index + (3 * scoreTens)]);
      digiPixel.bufferBlue[index + 4] = pgm_read_dword(&numberTable[index + (3 * scoreOnes)]);
    }
    
   for (int j = 0; j < 100; ++j)
    {
      digiPixel.drawScreen();
    }
    
    delay(1000);
  }

