//Author : Naval Bhandari 
//Email  : navs92@hotmail.com
//Left and right to move player, "A" button to shoot, now kill the invaders! 
#include <DigiPixel.h>
#include <avr/pgmspace.h>

// leave the following line uncommented for use with a Digispark
//DigiPixel digiPixel(3,0,5,2,1);  // LED Latch/Button Shift !load pin, LED/Button clock pin, LED Data Pin, LED Output Enable pin, Buttons data pin)

// leave the following line uncommented for use with an Arduino
DigiPixel digiPixel(5,2,6,4,3);  // LED Latch/Button Shift !load pin, LED/Button clock pin, LED Data Pin, LED Output Enable pin, Buttons data pin)

//Make the game harder/easier
#define playerSpeedSave 10; //Lower is faster
#define bulletSpeedSave 6;
#define invaderSpeedSave 150;

//Player starting pos
byte playerX = 4;
byte playerSpeed = playerSpeedSave; 
byte bulletSpeed = bulletSpeedSave;
byte invaderSpeed = invaderSpeedSave;
boolean bulletShot = false;
byte bulletX = 0; 
byte bulletY = 0;
//To display score at end of game
byte numberTable[30]PROGMEM = {
  0b01111110, 0b01000010, 0b01111110, 0b00100010, 0b01111110, 0b00000010, 0b01001110, 0b01001010, 0b01111010, 0b01000010, 0b01001010, 0b01111110, 0b01111000, 0b00001000, 0b01111110, 0b01111010, 0b01001010, 0b01001110, 0b01111110, 0b01001010, 0b01001110, 0b01000000, 0b01000000, 0b01111110, 0b01111110, 0b01001010, 0b01111110, 0b01111010, 0b01001010, 0b01111110};
boolean gameOver = false;
byte killed = 0;
//DO NOT CHANGE FROM 4
byte invadersX[4];
//How many rows of invaders 
const byte invaderRows = 4; 
byte invadersY[invaderRows];
boolean invadersDead[invaderRows*4];
byte invaders = invaderRows*4;

void initGame()
{
  //Initiliases the positions and shiz of the invaders and resets some stuff. 
  gameOver = false;
  for (int row = 0; row < invaderRows; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      invadersX[col] = col*2;
      invadersDead[(row*4)+col] = false;
    }
    invadersY[row] = (row*2)+7;
  }
}
void setup()
{
  digiPixel.setPixel(playerX, 0, 1);
  initGame();
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
  playerCheck();
  bulletCheck();
  invaderCheck();
  checkAllDead();
  if (gameOver)
  {
    //Reset invader
    showDeath();
    initGame();
    killed = 0;
  }
}

void saveGraphics(){
  digiPixel.clearScreen();
  digiPixel.setPixel(playerX, 0,1);
  if (bulletShot)
  {
    digiPixel.setPixel(bulletX, bulletY, 2);
  }
  for (int row=0; row < invaderRows; row++)
  {
    for (int col=0; col < 4; col++)
    {
      if (!invadersDead[(row*4)+col])
      {
        if (invadersY[row] < 8)
        {
          digiPixel.setPixel(invadersX[col], invadersY[row], 3);
        }
      }
    }
  }
}


void showDeath()
{
  //Nicked from SnakePixel
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

void playerCheck()
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
}

void bulletCheck()
{
  if (bulletShot)
  {
    bulletSpeed --;
    if (bulletSpeed == 0)
    {
      if (bulletY < 8)
      {
        for (int row = 0; row < invaderRows; row++)
        {
          for (int col = 0; col < 4; col++)
          {
            if (bulletX == invadersX[col] && bulletY == invadersY[row] && invadersDead[(row*4)+col] == false)
            {
              invadersDead[(row*4)+col] = true;
              bulletShot = false;
              killed ++;
            }       
          }
        }
        bulletY++;
      }
      else 
      {
        bulletShot = false;
      }
      bulletSpeed = bulletSpeedSave;
    }
  } 
}

void invaderCheck()
{
  invaderSpeed --;
  if (invaderSpeed == 0)
  {
    for (int row = 0; row < invaderRows; row++)
    {
      for (int col = 0; col < 4; col++)
      {
        if (row < 1)
        {
          if (invadersX[col] % 2 == 0)
          {
            invadersX[col] ++;
          }
          else
          {
            invadersX[col] --;
          }
        }
        if (invadersY[row] < 2 && invadersDead[(row*4)+col] == false)
        {
          gameOver = true;
        }
      }
      if (invadersY[row] > 1)
      {
        invadersY[row] --;
      } 
    }
    invaderSpeed = invaderSpeedSave;
  }
} 

void checkAllDead()
{
  boolean allDead = true;
  for (int i=0; i < invaders; i++)
  {
    if (invadersDead[i] == false)
    {
      allDead = false;
      break;
    }
  }
  if (allDead)
  {
    initGame();
  }
}

