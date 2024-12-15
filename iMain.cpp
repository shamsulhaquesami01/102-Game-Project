/*
CAR  RACING MANIA
Made by SAMI
 */

// declaration of header files

#include "iGraphics.h"
#include <ctime>
#include <cstring>
#include <windows.h>
#include <mmsystem.h>

// all pages
void drawhomepage();
void drawstartpage();
void drawins();
void draweasypage();
void drawgameover();
void increaseScore();
void updateCoinPosition(int &coinX, int &coinY);
void resetGame();
void drawscorepage();
void drawLives();
void drawmediumpage();
void drawaboutpage();
void drawhardpage();

// all handlers

void startclickhandler();
void aboutclickhandler();
void insclickhandler();
void scoreclickhandler();
void backclickhandler();
void easyclickhandler();
void mediumclickhandler();
void hardclickhandler();
void aboutclickhandler();

// time
void displayText(int a);
void startTimer();
int displayTime = 0;

// global variables
int startclick = 0;
int insclick = 0;
int homepage = 1;
int startpage = 0;
int instructions = 0;
int gameover = 0;
int scorepage = 0;
int easypage = 0;
int mediumpage = 0;
int hardpage = 0;
int aboutpage = 0;

// coin variables
int c1y = 50;
int c2y = 300;
int c3y = 500;
int c1x = 300;
int c2x = 500;
int c3x = 700;
int c1speed = 0;
int c2speed = 0;
int c3speed = 0;
int coinscore = 0;
bool musicon = true;

// player and enemy
int playerX = 470, playerY = 0;
int x = 400, y = 601;
int a = 600, b = 601;
int c = 550, d = 601;

//live count
int lives = 0;    
int maxLives = 0; 


int lifeX = 900, lifeY = 550;

// holes
int hole1X, hole1Y, hole2X, hole2Y;      
bool hole1Hit = false, hole2Hit = false; 
int holeCollisionCount = 0;              

// Define the sound files
#define BGM_SOUND "sound1.wav"
#define ACCELERATION_SOUND "sound2.wav"
#define COLLISION_SOUND "sound3.wav"

const char *currentSound = nullptr;
bool isAccelerationSoundPlaying = false;
bool isBgmPlaying = true;

//score printing
int nextMilestone = 1000; 
bool showMilestoneMessage = false;  
int milestoneDisplayTime = 0;  

//pause
bool isPaused = false; 


// Function related to playing sound
void playSound(const char *soundFile, bool loop)
{
    if (currentSound == soundFile)
    {
        return; // If the sound is already playing, do nothing
    }

    // Stop any currently playing sound
    PlaySound(NULL, 0, 0);

    // Play the new sound
    PlaySound(TEXT(soundFile), NULL, loop ? SND_ASYNC | SND_LOOP : SND_ASYNC);

    // Update the current sound
    currentSound = soundFile;
}

void stopSound()
{
    PlaySound(NULL, 0, 0);
    currentSound = nullptr;
}

void startGame()
{
    playSound(BGM_SOUND, true); // Play background music in a loop
}
void playAccelerationSound()
{
    // Play the acceleration sound only if it is not already playing
    if (!isAccelerationSoundPlaying)
    {
        PlaySound(TEXT("sound2.wav"), NULL, SND_LOOP | SND_ASYNC); // Loop the sound for continuous play
        isAccelerationSoundPlaying = true;
    }
}

void stopAccelerationSound()
{
    // Stop the acceleration sound when it is no longer needed
    PlaySound(NULL, 0, 0);
    isAccelerationSoundPlaying = false;
}

void playCollisionSound()
{
    PlaySound(TEXT("sound3.wav"), NULL, SND_ASYNC);
}
void handleGameOverMusic()
{
    if (homepage == 2)
    {
        playSound(BGM_SOUND, true); // Restart background music
    }
}

void displayText(int a)
{
    if (displayTime > 0)
    {
        char msg[50]; // Declare a string to hold the formatted text
        sprintf(msg, "Score: %d", a);
        iText(400, 300, msg, GLUT_BITMAP_HELVETICA_18);
        displayTime -= 100; // Decrease time (for each 100ms the timer is called)
    }
}

void startTimer()
{
    displayTime = 3000; // Set the time to display text
}

void initializeHoles()
{
    hole1X = 200 + rand() % 600;
    hole1Y = 700; // Off-screen
    hole2X = 200 + rand() % 600;
    hole2Y = 900; // Off-screen
    hole1Hit = false;
    hole2Hit = false;
    holeCollisionCount = 0;
}

void iMouseMove(int mx, int my)
{
    // printf("x = %d, y= %d\n",mx,my);
}
bool coin1Hit = false, coin2Hit = false, coin3Hit = false;

bool isTooClose(int newPos, int existingPos, int minSeparation)
{
    return abs(newPos - existingPos) < minSeparation;
}

// coin collsion detection

void coincollision()
{
    // Check collision with Coin 1
    if ((playerX + 60 >= c1x && playerX <= c1x + 30) &&
        (playerY + 110 >= c1y && playerY <= c1y + 30) &&
        !coin1Hit)
    {
        coinscore += 100;
        coin1Hit = true;
        updateCoinPosition(c1x, c1y);
    }

    // Check collision with Coin 2
    if ((playerX + 60 >= c2x && playerX <= c2x + 30) &&
        (playerY + 110 >= c2y && playerY <= c2y + 30) &&
        !coin2Hit)
    {
        coinscore += 100;
        coin2Hit = true;
        updateCoinPosition(c2x, c2y);
    }

    // Check collision with Coin 3
    if ((playerX + 60 >= c3x && playerX <= c3x + 30) &&
        (playerY + 110 >= c3y && playerY <= c3y + 30) &&
        !coin3Hit)
    {
        coinscore += 100;
        coin3Hit = true;
        updateCoinPosition(c3x, c3y);
    }
}

void updateCoinPosition(int &coinX, int &coinY)
{
    coinX = -100;
    coinY = -100;
}
int counter = 0;         
bool v1Collided = false; 
bool v2Collided = false; 
bool v3Collided = false; 
bool collisionDetected = false;

// car collision detection

void collision()
{
    collisionDetected = false;

    // Check for collisions
    if ((playerX + 50 >= x && playerX <= x + 50) && (playerY + 90 >= y && playerY <= y + 90))
    {
        if (!v1Collided)
        {
            lives--; // Reduce lives
            counter++;
            v1Collided = true;
            stopAccelerationSound(); // Stop acceleration sound
            playCollisionSound();
        }
    }
    else
    {
        v1Collided = false;
    }

    if ((playerX + 50 >= a && playerX <= a + 50) && (playerY + 90 >= b && playerY <= b + 90))
    {
        if (!v2Collided)
        {
            lives--; // Reduce lives
            counter++;
            v2Collided = true;
            stopAccelerationSound(); // Stop acceleration sound
            playCollisionSound();
        }
    }
    else
    {
        v2Collided = false;
    }

    if ((playerX + 50 >= c && playerX <= c + 50) && (playerY + 90 >= d && playerY <= d + 90))
    {
        if (!v3Collided)
        {
            lives--; // Reduce lives
            counter++;
            v3Collided = true;
            stopAccelerationSound(); // Stop acceleration sound
            playCollisionSound();
        }
    }
    else
    {
        v3Collided = false;
    }

    // End the game if lives reach zero
    if (lives <= 0)
    {
        easypage = 0;
        mediumpage = 0;
        hardpage = 0;
        homepage = 2; // Game over page
        counter = 0;
        lives = maxLives; // Reset lives for the next game
    }
}

// background rendering

int totalimg = 30;
int inc = 20;
char backimgpath[200][250] =
    {"pictures2//tile30.bmp", "pictures2//tile29.bmp", "pictures2//tile28.bmp", "pictures2//tile27.bmp", "pictures2//tile26.bmp", "pictures2//tile25.bmp", "pictures2//tile24.bmp", "pictures2//tile23.bmp", "pictures2//tile22.bmp", "pictures2//tile21.bmp", "pictures2//tile20.bmp", "pictures2//tile19.bmp", "pictures2//tile18.bmp", "pictures2//tile17.bmp", "pictures2//tile16.bmp", "pictures2//tile15.bmp", "pictures2//tile14.bmp", "pictures2//tile13.bmp", "pictures2//tile12.bmp", "pictures2//tile11.bmp", "pictures2//tile10.bmp", "pictures2//tile09.bmp", "pictures2//tile08.bmp", "pictures2//tile07.bmp", "pictures2//tile06.bmp", "pictures2//tile05.bmp", "pictures2//tile04.bmp", "pictures2//tile03.bmp", "pictures2//tile02.bmp", "pictures2//tile01.bmp"};
int imgposition[100];

void initialization()
{
    int i, j;
    for (i = 0, j = 0; i < totalimg; i++)
    {
        imgposition[i] = j;
        j += inc;
    }
}

void moveback()
{
    if (isPaused) return; 
    for (int i = 0; i < totalimg; i++)
        imgposition[i] -= inc;
    for (int i = 0; i < totalimg; i++)
    {
        if (imgposition[i] < 0)
            imgposition[i] = 600 - inc;
    }
}

void render()
{
    for (int i = 0; i < totalimg; i++)
    {
        iShowBMP2(0, imgposition[i], backimgpath[i], 0);
    }
}

// leaderboard using file reading and writing

void newhighscore();
void readscore();
int len = 0;
char str1[100];
bool newscore = true;

struct hscore
{
    char name[30];
    int score = 0;

} high_score[5];

void readscore()
{
    FILE *fp = fopen("Score.txt", "a+");
    if (!fp)
    {
        printf("Error: Could not open Score.txt\n");
        return;
    }

   
    for (int i = 0; i < 5; i++)
    {
        strcpy(high_score[i].name, "---");
        high_score[i].score = 0;
    }

   
    for (int i = 0; i < 5; i++)
    {
        if (fscanf(fp, "%s %d", high_score[i].name, &high_score[i].score) != 2)
        {
            break;
        }
    }

    fclose(fp);
}

void newhighscore()
{
    FILE *fp = fopen("Score.txt", "r+");
    if (!fp)
    {
        printf("Error: Could not open Score.txt for reading.\n");
        return;
    }

   
    for (int i = 0; i < 5; i++)
    {
        if (fscanf(fp, "%s %d", high_score[i].name, &high_score[i].score) != 2)
        {
            
            strcpy(high_score[i].name, "---");
            high_score[i].score = 0;
        }
    }
    fclose(fp);

    
    int insertionIndex = -1;
    for (int i = 0; i < 5; i++)
    {
        if (coinscore > high_score[i].score)
        {
            insertionIndex = i;
            break;
        }
    }

    if (insertionIndex != -1)
    {

        for (int i = 4; i > insertionIndex; i--)
        {
            high_score[i] = high_score[i - 1];
        }

       
        strcpy(high_score[insertionIndex].name, str1);
        high_score[insertionIndex].score = coinscore;
    }

   
    fp = fopen("Score.txt", "w");
    if (!fp)
    {
        printf("Error: Could not open Score.txt for writing.\n");
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        fprintf(fp, "%s %d\n", high_score[i].name, high_score[i].score);
    }
    fclose(fp);

   
    newscore = false;
}

// taking name input from player
void showchar()
{
    iSetColor(255, 255, 255);
    iText(400, 500, "Enter Your Name: ", GLUT_BITMAP_HELVETICA_18);
    iRectangle(495, 450, 160, 30);
    iText(500, 460, str1, GLUT_BITMAP_HELVETICA_18);
}

void takeinput(unsigned key)
{
    if (key == '\r')
    {
        homepage = 1;
        newhighscore();
    }
    else if (key == '\b')
    {
        if (len <= 0)
        {
            len = 0;
        }
        else
            len--;
        str1[len] = '\0';
    }
    else
    {
        str1[len] = key;
        len++;
        if (len > 15)
        {
            len = 15;
        }
        str1[len] = '\0';
    }
}

// in built functions
void iDraw()
{
    iClear();
    if (homepage == 1)
    {
        drawhomepage();
    }
    else if (startpage == 1)
    {
        drawstartpage();
    }
    else if (instructions == 1)
    {
        drawins();
    }
   
    else if (scorepage == 1)
    {
        drawscorepage();
    }
    else if (easypage == 1)
    {
        draweasypage();
    }
    else if (mediumpage == 1)
    {
        drawmediumpage();
    }
    else if (hardpage == 1)
    {
        drawhardpage();
    }
    else if (aboutpage == 1)
    {
        drawaboutpage();
    }

    else if (homepage == 2)
    {
        drawgameover();
        showchar();
    }
    if (showMilestoneMessage) {
        iSetColor(0,0,0);
        iFilledRectangle(180, 530, 520, 50);
        iSetColor(255, 255, 255);
        char message[100];
        sprintf(message, "Congratulations! You have completed %d points!", nextMilestone - 1000);
        iText(200, 550, message, GLUT_BITMAP_TIMES_ROMAN_24);

        // Reduce display time
        milestoneDisplayTime -= 100;
        if (milestoneDisplayTime <= 0) {
            showMilestoneMessage = false;  // Stop displaying the message
        }
    }
}

void iMouse(int button, int state, int mx, int my)
{
    printf("x = %d, y= %d\n", mx, my);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
    
        if (homepage == 1 && (mx >= 334 && mx <= 422) && (my >= 47 && my <= 86))
        {
            startclickhandler();
        }
        else if (homepage == 1 && (mx >= 463 && mx <= 573) && (my >= 46 && my <= 92))
        {
            insclickhandler();
        }
        else if (homepage == 1 && (mx >= 775 && mx <= 825) && (my >= 45 && my <= 97))
        {
            scoreclickhandler();
        }
         else if (homepage == 1 && (mx >= 623 && mx <= 695) && (my >= 45 && my <= 97))
        {
            exit(0);
        }
        else if (homepage == 1 && (mx >= 210 && mx <= 265) && (my >= 45 && my <= 97))
        {
            aboutclickhandler();
        }
        else if ((instructions == 1 || scorepage == 1 || startpage == 1 || aboutpage == 1) && (mx >= 0 && mx <= 90) && (my >= 542 && my <= 595))
        {
            backclickhandler();
        }
        else if (startpage == 1 && (mx >= 470 && mx <= 570) && (my >= 65 && my <= 165))
        {
            easyclickhandler();
        }
        else if (startpage == 1 && (mx >= 624 && mx <= 740) && (my >= 65 && my <= 165))
        {
            mediumclickhandler();
        }
        else if (startpage == 1 && (mx >= 795 && mx <= 900) && (my >= 65 && my <= 165))
        {
            hardclickhandler();
        }
    }
}

void iKeyboard(unsigned char key)
{
    if (key == 'p' || key == 'P') {
        isPaused = !isPaused; // Toggle pause state
    }
    if(!isPaused){
    if (homepage == 2)
    {
        takeinput(key);
    }
    if (key == '\q')
    {
        exit(0);
    }
    }
}

void iSpecialKeyboard(unsigned char key)
{
    if (key == GLUT_KEY_RIGHT)
    {
        playerX += 40;
        if (playerX > 755)
            playerX = 755;

        // Play acceleration sound when moving right
        playAccelerationSound();
    }

    if (key == GLUT_KEY_LEFT)
    {
        playerX -= 40;
        if (playerX < 210)
            playerX = 210;

        // Play acceleration sound when moving left
        playAccelerationSound();
    }

    if (key == GLUT_KEY_UP)
    {
        playerY += 20;
        if (playerY > 520)
            playerY = 520;

        // Play acceleration sound when moving up
        playAccelerationSound();
    }

    if (key == GLUT_KEY_DOWN)
    {
        playerY -= 20;
        if (playerY < 0)
            playerY = 0;

        // Play acceleration sound when moving down
        playAccelerationSound();
    }

    // Stop acceleration sound when no key is pressed
    if (key != GLUT_KEY_RIGHT && key != GLUT_KEY_LEFT && key != GLUT_KEY_UP && key != GLUT_KEY_DOWN)
    {
        stopAccelerationSound();
    }
    if (key == GLUT_KEY_F2)
    {
        if (isBgmPlaying)
        {
            // Stop BGM
            PlaySound(0, 0, 0);
            isBgmPlaying = false;
        }
        else
        {
            // Start BGM again
            PlaySound(TEXT("sound1.wav"), NULL, SND_LOOP | SND_ASYNC);
            isBgmPlaying = true;
        }
    }
}

// main pages
void drawhomepage()
{
    iSetColor(128, 128, 0);
    iShowBMP(0, 0, "pictures//homepage.bmp");
    iSetColor(255, 255, 255);
    iFilledCircle(800, 70, 32, 100);
    iSetColor(236, 185, 20);
    iFilledCircle(800, 70, 30, 100);
    iSetColor(255, 255, 255);
    iText(776, 63, "Score", GLUT_BITMAP_HELVETICA_18);
    iSetColor(255, 255, 255);
    iFilledCircle(240, 70, 32, 100);
    iSetColor(236, 185, 20);
    iFilledCircle(240, 70, 30, 100);
    iSetColor(255, 255, 255);
    iText(214, 63, "About", GLUT_BITMAP_HELVETICA_18);
}

void drawstartpage()
{
    iShowBMP2(0, 555, "back2.bmp", 0);
    iShowBMP2(0, 0, "new_cars\\startpage.bmp", 0);
}

void drawins()
{
  iShowBMP2(0,0,"new_cars\\instructions.bmp",0);
}
void drawaboutpage()
{
 iShowBMP2(0,0,"new_cars\\ABOUT_ME.bmp",0);
}

void draweasypage()
{

    iShowBMP2(0, 0, "pictures2\\road2.bmp", 0);
    render();
    
     iShowBMP2(hole1X, hole1Y, "hole.bmp", 0);
    iShowBMP2(hole2X, hole2Y, "hole.bmp", 0);
    iShowBMP2(x, y, "new_cars//gari5.bmp", 0);
    iShowBMP2(a, b, "new_cars//gari3_ulta.bmp", 0);
    iShowBMP2(c, d, "new_cars//gari6_ulta.bmp", 0);
    drawLives();

    // Render holes
   iShowBMP2(playerX, playerY, "new_cars//gari4.bmp", 0);

//pause logic
    if (isPaused) {
        iSetColor(0, 0, 0);
        iFilledRectangle(359,282,300,60);
        iSetColor(255, 255, 255);
        iText(368, 305, "Game Paused. Press 'P' to Resume.", GLUT_BITMAP_HELVETICA_18);
        return; 
    }

    // Move holes
    hole1Y -= 7; // Adjust speed based on difficulty
    if (hole1Y <= 0)
    {
        hole1Y = 600;
        hole1X = 200 + rand() % 600;
        hole1Hit = false; // Reset collision flag
    }

    hole2Y -= 7.5; // Adjust speed based on difficulty
    if (hole2Y <= 0)
    {
        hole2Y = 600;
        hole2X = 200 + rand() % 600;
        hole2Hit = false; // Reset collision flag
    }

    // Check collisions with holes
    if ((playerX + 60 >= hole1X && playerX <= hole1X + 30) &&
        (playerY + 110 >= hole1Y && playerY <= hole1Y + 30) &&
        !hole1Hit)
    {
        holeCollisionCount++;
        hole1Hit = true;
    }

    if ((playerX + 60 >= hole2X && playerX <= hole2X + 30) &&
        (playerY + 110 >= hole2Y && playerY <= hole2Y + 30) &&
        !hole2Hit)
    {
        holeCollisionCount++;
        hole2Hit = true;
    }

    // If two hole collisions occur, decrement one life
    if (holeCollisionCount >= 2)
    {
        lives--;
        holeCollisionCount = 0;
    }

    
    if (lives <= 0)
    {
        easypage = 0;
        mediumpage = 0;
        hardpage = 0;
        homepage = 2;     // Game over page
        lives = maxLives; 
    }
    y -= 15;
    if (y <= 0)
    {
        y = 601;
        x = 210 + rand() % 500;

        // Ensure separation from Vehicle 2 and Vehicle 3
        while (isTooClose(x, a, 65) || isTooClose(x, c, 65))
        {
            x = 210 + rand() % 500;
        }
    }
    collision();

    b -= 16;
    if (b <= 0)
    {
        b = 601;
        a = 220 + rand() % 500;

      
        while (isTooClose(a, x, 65) || isTooClose(a, c, 65))
        {
           a = 210 + rand() % 500;
        }
    }
    collision();

    d -= 16.5;
    if (d <= 0)
    {
        d = 601;
        c = 210 + rand() % 500;

       
        while (isTooClose(c, x, 65) || isTooClose(c, a, 65))
        {
            c = 210 + rand() % 500;
        }
    }
    collision();

    iSetColor(255, 215, 0);
    iFilledCircle(c1x + 15, c1y + 15, 15);
    iFilledCircle(c2x + 15, c2y + 15, 15);
    iFilledCircle(c3x + 15, c3y + 15, 15);

    c1y -= 7;
    if (c1y <= 0)
    {
        c1y = 600;
        c1x = 200 + rand() % 600;
        coin1Hit = false;
    }
    coincollision();

    c2y -= 7.5;
    if (c2y <= 0)
    {
        c2y = 600;
        c2x = 400 + rand() % 400;
        coin2Hit = false;
    }
    coincollision();

    c3y -= 7;
    if (c3y <= 0)
    {
        c3y = 600;
        c3x = 450 + rand() % 350;
        coin3Hit = false;
    }
    coincollision();
    // Display score
    iSetColor(0,0,0);
    iFilledRectangle(13,558,139,34);
    iSetColor(246, 228, 20);
    iFilledRectangle(15,560,135,30);
    
    char scoreText[50];

     iSetColor(0, 00, 0);
    sprintf(scoreText, "SCORE : %d", coinscore);
    iText(20, 570, scoreText, GLUT_BITMAP_HELVETICA_18);
}


void drawmediumpage()
{
    // Background
    iShowBMP2(0, 0, "pictures2\\road2.bmp", 0);
    render(); 

    
    iShowBMP2(hole1X, hole1Y, "hole.bmp", 0);
    iShowBMP2(hole2X, hole2Y, "hole.bmp", 0);

    // Draw coins
    iSetColor(255, 215, 0);
    iFilledCircle(c1x + 15, c1y + 15, 15);
    iFilledCircle(c2x + 15, c2y + 15, 15);
    iFilledCircle(c3x + 15, c3y + 15, 15);

    // Draw player and enemy cars
    // Player car
    iShowBMP2(x, y, "new_cars//gari5.bmp", 0);             // Enemy car 1
    iShowBMP2(a, b, "new_cars//gari3_ulta.bmp", 0);        // Enemy car 2
    iShowBMP2(c, d, "new_cars//gari6_ulta.bmp", 0);        // Enemy car 3
    iShowBMP2(playerX, playerY, "new_cars//gari3.bmp", 0); 
    
    drawLives();
  //pause logic
     if (isPaused) {
        iSetColor(0, 0, 0);
        iFilledRectangle(359,282,300,60);
        iSetColor(255, 255, 255);
        iText(368, 305, "Game Paused. Press 'P' to Resume.", GLUT_BITMAP_HELVETICA_18);
        return; 
    }

    
    y -= 30;
    if (y <= 0)
    {
        y = 601;
        x = 210 + rand() % 500;

       
        while (isTooClose(x, a, 65) || isTooClose(x, c, 65))
        {
           x = 210 + rand() % 500;
        }
    }
    collision();

 
    b -= 30;
    if (b <= 0)
    {
        b = 601;
       a = 210 + rand() % 530;

        
        while (isTooClose(a, x, 65) || isTooClose(a, c, 65))
        {
           a = 210 + rand() % 530;
        }
    }
    collision();

   
    d -= 40;
    if (d <= 0)
    {
        d = 601;
        c = 210 + rand() % 540;

        // Ensure no overlap with other enemies
        while (isTooClose(c, x, 65) || isTooClose(c, a, 65))
        {
           c = 210 + rand() % 530;
        }
    }
    collision();

    // Move holes
    hole1Y -= 7; 
    if (hole1Y <= 0)
    {
        hole1Y = 600;
        hole1X = 200 + rand() % 600;
        hole1Hit = false; // Reset collision flag
    }

    hole2Y -= 7.5; 
    if (hole2Y <= 0)
    {
        hole2Y = 600;
        hole2X = 200 + rand() % 600;
        hole2Hit = false; // Reset collision flag
    }

    // Check collisions with holes
    if ((playerX + 60 >= hole1X && playerX <= hole1X + 30) &&
        (playerY + 110 >= hole1Y && playerY <= hole1Y + 30) &&
        !hole1Hit)
    {
        holeCollisionCount++;
        hole1Hit = true;
    }

    if ((playerX + 60 >= hole2X && playerX <= hole2X + 30) &&
        (playerY + 110 >= hole2Y && playerY <= hole2Y + 30) &&
        !hole2Hit)
    {
        holeCollisionCount++;
        hole2Hit = true;
    }

   
    if (holeCollisionCount >= 2)
    {
        lives--;
        holeCollisionCount = 0;
    }

    // End game if lives reach zero
    if (lives <= 0)
    {
        easypage = 0;
        mediumpage = 0;
        hardpage = 0;
        homepage = 2; 
        lives = maxLives; // Reset lives for the next game
    }
    // Update coins
    c1y -= 12;
    if (c1y <= 0)
    {
        c1y = 600;
        c1x = 200 + rand() % 600;
        coin1Hit = false;
    }
    coincollision();

    c2y -= 12.5;
    if (c2y <= 0)
    {
        c2y = 600;
        c2x = 400 + rand() % 400;
        coin2Hit = false;
    }
    coincollision();

    c3y -= 12;
    if (c3y <= 0)
    {
        c3y = 600;
        c3x = 450 + rand() % 350;
        coin3Hit = false;
    }
    coincollision();

     // Display score
    iSetColor(0,0,0);
    iFilledRectangle(13,558,139,34);
    iSetColor(246, 228, 20);
    iFilledRectangle(15,560,135,30);
    
    char scoreText[50];

     iSetColor(0, 00, 0);
    sprintf(scoreText, "SCORE : %d", coinscore);
    iText(20, 570, scoreText, GLUT_BITMAP_HELVETICA_18);
}

void drawhardpage()
{
    // Background
    iShowBMP2(0, 0, "pictures2\\road2.bmp", 0);
    render(); 

    // Draw holes
    iShowBMP2(hole1X, hole1Y, "hole.bmp", 0);
    iShowBMP2(hole2X, hole2Y, "hole.bmp", 0);

    // Draw coins
    iSetColor(255, 215, 0);
    iFilledCircle(c1x + 15, c1y + 15, 15);
    iFilledCircle(c2x + 15, c2y + 15, 15);
    iFilledCircle(c3x + 15, c3y + 15, 15);

    // Draw player and enemy cars
    
    iShowBMP2(x, y, "new_cars//gari5.bmp", 0);             // Enemy car 1
    iShowBMP2(a, b, "new_cars//gari3_ulta.bmp", 0);        // Enemy car 2
    iShowBMP2(c, d, "new_cars//gari6_ulta.bmp", 0);        // Enemy car 3
    iShowBMP2(playerX, playerY, "new_cars//gari6.bmp", 0);
  
    drawLives();

   //pause logic
    if (isPaused) {
        iSetColor(0, 0, 0);
        iFilledRectangle(359,282,300,60);
        iSetColor(255, 255, 255);
        iText(368, 305, "Game Paused. Press 'P' to Resume.", GLUT_BITMAP_HELVETICA_18);
        return; 
    }

    y -= 40;
    if (y <= 0)
    {
        y = 601;
        x = 210 + rand() % 530;

      
        while (isTooClose(x, a, 65) || isTooClose(x, c, 65))
        {
            x = 210 + rand() % 530;
        }
    }
    collision();

    // Update enemy car 2
    b -= 40;
    if (b <= 0)
    {
        b = 601;
      a = 210 + rand() % 510;

     
        while (isTooClose(a, x, 65) || isTooClose(a, c, 65))
        {
         a = 210 + rand() % 510;
        }
    }
    collision();

    // Update enemy car 3
    d -= 50;
    if (d <= 0)
    {
        d = 601;
        c = 210 + rand() % 520;

        while (isTooClose(c, x, 65) || isTooClose(c, a, 65))
        {
            c = 210 + rand() % 520;
        }
    }
    collision();

    // Move holes
    hole1Y -= 7; 
    if (hole1Y <= 0)
    {
        hole1Y = 600;
        hole1X = 200 + rand() % 600;
        hole1Hit = false; // Reset collision flag
    }

    hole2Y -= 7.5; 
    if (hole2Y <= 0)
    {
        hole2Y = 600;
        hole2X = 200 + rand() % 600;
        hole2Hit = false; // Reset collision flag
    }

    
    if ((playerX + 60 >= hole1X && playerX <= hole1X + 30) &&
        (playerY + 110 >= hole1Y && playerY <= hole1Y + 30) &&
        !hole1Hit)
    {
        holeCollisionCount++;
        hole1Hit = true;
    }

    if ((playerX + 60 >= hole2X && playerX <= hole2X + 30) &&
        (playerY + 110 >= hole2Y && playerY <= hole2Y + 30) &&
        !hole2Hit)
    {
        holeCollisionCount++;
        hole2Hit = true;
    }

    
    if (holeCollisionCount >= 2)
    {
        lives--;
        holeCollisionCount = 0;
    }

    // End game if lives reach zero
    if (lives <= 0)
    {
        easypage = 0;
        mediumpage = 0;
        hardpage = 0;
        homepage = 2;    
        lives = maxLives; // Reset lives for the next game
    }

    // Update coins
    c1y -= 17;
    if (c1y <= 0)
    {
        c1y = 600;
        c1x = 200 + rand() % 600;
        coin1Hit = false;
    }
    coincollision();

    c2y -= 17.5;
    if (c2y <= 0)
    {
        c2y = 600;
        c2x = 400 + rand() % 400;
        coin2Hit = false;
    }
    coincollision();

    c3y -= 17;
    if (c3y <= 0)
    {
        c3y = 600;
        c3x = 450 + rand() % 350;
        coin3Hit = false;
    }
    coincollision();

    // Display score
    iSetColor(0,0,0);
    iFilledRectangle(13,558,139,34);
    iSetColor(246, 228, 20);
    iFilledRectangle(15,560,135,30);
    
    char scoreText[50];

     iSetColor(0, 00, 0);
    sprintf(scoreText, "SCORE : %d", coinscore);
    iText(20, 570, scoreText, GLUT_BITMAP_HELVETICA_18);
    
}

void increaseScore() {
    coinscore += 20;

    // Check if the next milestone is reached
    if (coinscore >= nextMilestone) {
        showMilestoneMessage = true;
        milestoneDisplayTime = 3000;  // Show message for 3 seconds
        nextMilestone += 1000;  // Set the next milestone
    }
}


void drawgameover()
{
    iShowBMP2(0,0,"new_cars\\gameover.bmp", 0);
    stopAccelerationSound(); // Stop the acceleration sound when game is over

    // Restart background music after game over
    PlaySound(TEXT("sound1.wav"), NULL, SND_LOOP | SND_ASYNC);
    playerX = 470;
    playerY = 0;
    x = 400;
    y = 601;
    a = 600;
    b = 601;
    c = 550;
    d = 601;
    homepage = 2;
    // sprintf( "Score: %d", coinscore);
}
void drawscorepage()
{
    // Background for the score page
    
    iShowBMP2(0, 0, "new_cars\\2.bmp", 0);
    // iText(400, 550, "High Scores", GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(0, 0, 0);
    // Display all high scores
    for (int i = 0; i < 5; i++)
    {
        char scoreDisplay[50];
        sprintf(scoreDisplay, "%s - %d", high_score[i].name, high_score[i].score);
        iText(267, 385 - i * 50, scoreDisplay, GLUT_BITMAP_TIMES_ROMAN_24);
    }

    // Back button
    iShowBMP2(0, 555, "back2.bmp", 0);
}
void drawLives()
{
    iSetColor(255, 0, 0); // Red color for hearts
    for (int i = 0; i < lives; i++)
    {
        double xOffset = lifeX - (i * 40); // Position hearts with spacing

        // Draw the left circle of the heart
        iFilledCircle(xOffset - 7, lifeY, 8);

        // Draw the right circle of the heart
        iFilledCircle(xOffset + 7, lifeY, 8);

        // Points for the bottom triangle of the heart
        double triangleX[3] = {xOffset - 10, xOffset + 10, xOffset};
        double triangleY[3] = {lifeY - 4, lifeY - 4, lifeY - 15};

        // Draw the bottom triangle of the heart
        iFilledPolygon(triangleX, triangleY, 3);
    }
}

// handler functions

void startclickhandler()
{
    homepage = 0;
    startpage = 1;
    coinscore = 0;
    coin1Hit = coin2Hit = coin3Hit = false;
}
void insclickhandler()
{
    homepage = 0;
    startpage = 0;
    instructions = 1;
}
void scoreclickhandler()
{
    homepage = 0;
    startpage = 0;
    instructions = 0;
    scorepage = 1;
     readscore();
}
void backclickhandler()
{
    homepage = 1;
    startpage = 0;
    instructions = 0;
    aboutpage = 0;
    scorepage = 0;
}
void aboutclickhandler()
{
    homepage = 0;

    aboutpage = 1;
}
void easyclickhandler()
{
    homepage = 0;
    startpage = 0;
    instructions = 0;
    scorepage = 0;
    easypage = 1;
    lives = 3; // Set lives for easy mode
    maxLives = 3;
    counter = 0;       // Reset collision counter
    initializeHoles();
}

void mediumclickhandler()
{
    homepage = 0;
    startpage = 0;
    instructions = 0;
    scorepage = 0;
    easypage = 0;
    mediumpage = 1;
    lives = 2; // Set lives for medium mode
    maxLives = 2;
    counter = 0; // Reset collision counter
    initializeHoles();
}

void hardclickhandler()
{
    homepage = 0;
    startpage = 0;
    instructions = 0;
    scorepage = 0;
    easypage = 0;
    mediumpage = 0;
    hardpage = 1;
    lives = 1; // Set lives for hard mode
    maxLives = 1;
    counter = 0; // Reset collision counter
    initializeHoles();
}

// main funciton

int main()
{
    initialization();    
    readscore();           
    iSetTimer(1000, increaseScore); 
    iSetTimer(30, moveback);        
    iSetTimer(10, []() {
        if (milestoneDisplayTime > 0) {
            milestoneDisplayTime -= 10;
        }
    });
    
    playSound(BGM_SOUND, true);

    
    startTimer();
    iInitialize(1000, 600, "Racing Game");

    return 0;
}
