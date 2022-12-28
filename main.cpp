#include <algorithm>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <windows.h>
#include <math.h>
#include <string.h>

static double playerY = 0;
static double speedFactor = 0.05,maxSpeedFactor = 0.10;
static double playerSize = 0.5;
static double timeSinceUpArrow = 0;
static double calculatedY = 0;
static double gravity = 0.035;
static double gravityWithTime = 0.02;
static double jumpPower = 1.5;
static double playerX = -4;

static double distanceBetweenPipes = 6;
static int pipeCount = 4;
double pipeXPositions[] = {0,distanceBetweenPipes,distanceBetweenPipes * 2,distanceBetweenPipes * 3};
double pipeYPositions[] = {2,1,-1,-2};
double calculatedPipeY = 0;
static double defaultPipeSpacing = 3;
static double defaultPipeWidth = 1.5,defaultPipeHeight = 10;

int nextPipeIndex = 0;
int previousIndex = 0;
int lastIndex = 0;
int currentScore = 0;

bool isGameStarted = false;
bool didCollide = false;


float topPipeVertices[] =
    {
        0.0, 0.0, // left bot
        0.0, 0.0, // right bot
        0.0, 0.0, // left top
        0.0, 0.0  // right top
    };

float botPipeVertices[] =
    {
        0.0, 0.0, // left top
        0.0, 0.0, // right top
        0.0, 0.0, // left bot
        0.0, 0.0  // right bot
    };


static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 25.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


bool CollisionControl()
{
    for(int i = 0;i<pipeCount;i++)
    {
        if(fabs(pipeXPositions[i] - playerX) > playerSize) continue;

        topPipeVertices[0] = pipeXPositions[i] - defaultPipeWidth / 2;
        topPipeVertices[1] = pipeYPositions[i] + defaultPipeSpacing / 2;

        topPipeVertices[2] = topPipeVertices[0] + defaultPipeWidth;
        topPipeVertices[3] = topPipeVertices[1];

        topPipeVertices[4] = topPipeVertices[0];
        topPipeVertices[5] = topPipeVertices[1] + defaultPipeHeight;

        topPipeVertices[6] = topPipeVertices[1];
        topPipeVertices[7] = topPipeVertices[5];
        //-----------------------------------------------------------------//
        botPipeVertices[0] = pipeXPositions[i] - defaultPipeWidth / 2;
        botPipeVertices[1] = pipeYPositions[i] - defaultPipeSpacing / 2;

        botPipeVertices[2] = botPipeVertices[0] + defaultPipeWidth;
        botPipeVertices[3] = botPipeVertices[1];

        botPipeVertices[4] = botPipeVertices[0];
        botPipeVertices[5] = botPipeVertices[1] - defaultPipeHeight;

        botPipeVertices[6] = botPipeVertices[1];
        botPipeVertices[7] = botPipeVertices[5];


        if(playerY + playerSize >= topPipeVertices[1] && playerY - playerSize <= topPipeVertices[5])
        {
            if(playerX + playerSize > topPipeVertices[0] && playerX - playerSize < topPipeVertices[2])
            {
                return true;
            }
        }

        if(playerY - playerSize <= botPipeVertices[1] && playerY + playerSize >= botPipeVertices[5])
        {
            if(playerX + playerSize > botPipeVertices[0] && playerX - playerSize < botPipeVertices[2])
            {
                return true;
            }
        }
    }

    return false;
}


void DecreasePlayerYValue(int value)
{
    if(isGameStarted)
    {
        timeSinceUpArrow += gravityWithTime * playerSize;
        calculatedY = playerY - (timeSinceUpArrow + gravity) * playerSize;
        playerY = std::max(-4.5,calculatedY);
    }

    glutPostRedisplay();

    didCollide = CollisionControl();
    if(didCollide) printf("Collided!\n");

    glutTimerFunc(25, DecreasePlayerYValue, 0);
}


void IncreaseGameSpeed(int value)
{
    if(speedFactor >= maxSpeedFactor) return;

    speedFactor += 0.01;
    glutTimerFunc(5000,IncreaseGameSpeed,0);
}


void startButton()
{
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-1.5f, -1.5f);
    glVertex2f( 1.5f, -1.5f);
    glVertex2f( 1.5f,  1.5f);
    glVertex2f(-1.5f,  1.5f);
    glEnd();
}


void drawText(char *string)
{
   char *c;
   glRasterPos3f(-2.0,0.0f, -9.0f);
   for (c=string; *c != '\0'; c++)
   {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
   }
}


void scoreText(int score) {
  std::string temp_str = std::to_string(score);
  const char *string = temp_str.c_str();
  glRasterPos3f(0.0, 4.0f, -9.0f);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'C');
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
  for (const char *c = string; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }
}

void MoveThePipes(int value)
{
    for(int i = 0;i<pipeCount;i++)
    {
        pipeXPositions[i] -= speedFactor;
        if(pipeXPositions[i] < -(pipeCount / 2) * distanceBetweenPipes)
        {
            pipeXPositions[i] += pipeCount * distanceBetweenPipes;
            previousIndex = (i + pipeCount - 1) % pipeCount;
            double nextY = pipeYPositions[previousIndex];
            srand(GetTickCount());
            double randomDifference = (3.0f * rand()) / RAND_MAX - 1.5;
            calculatedPipeY += randomDifference;
            calculatedPipeY = std::max(-3.0,calculatedPipeY);
            calculatedPipeY = std::min(3.0,calculatedPipeY);
            pipeYPositions[i] = calculatedPipeY;
        }

        if(pipeXPositions[i] - playerX > 0 && pipeXPositions[i] - playerX < distanceBetweenPipes)
        {
            if(lastIndex != nextPipeIndex)
            {
                lastIndex = nextPipeIndex;
                currentScore++;
                printf("Score : %d\n",currentScore);
            }
            nextPipeIndex = i;
        }
    }

    glutTimerFunc(10, MoveThePipes, 0);
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if(isGameStarted) return;
        isGameStarted = true;
        glutTimerFunc(10, MoveThePipes, 0);
  }
}


void DrawSinglePipe(double pipeX,double pipeSpaceY)
{
    glColor3f(0,1,0);
    glPushMatrix();
        glTranslated(pipeX,pipeSpaceY + defaultPipeSpacing/2 + defaultPipeHeight,-10);
        glRotated(90,1,0,0);
        glutSolidCylinder(defaultPipeWidth/2,defaultPipeHeight,16,16);
    glPopMatrix();
    glPushMatrix();
        glTranslated(pipeX,pipeSpaceY - defaultPipeSpacing/2 ,-10);
        glRotated(90,1,0,0);
        glutSolidCylinder(defaultPipeWidth/2,defaultPipeHeight,16,16);
    glPopMatrix();
}


void DrawPipes()
{
    for(int i = 0;i<pipeCount;i++)
    {
        DrawSinglePipe(pipeXPositions[i],pipeYPositions[i]);
    }
}


static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    DrawPipes();

    glColor3d(1.0,1.0,0);
    glPushMatrix();

    glPushMatrix();
        glTranslated(playerX,playerY,-10);
        glutSolidSphere(0.5,16,16);
    glPopMatrix();

    glPushMatrix();
        startButton();
    glPopMatrix();

    scoreText(currentScore);

    if(!isGameStarted){
    char *s = "Oyuna baslamak icin tiklayin";
    drawText(s);
    }
    glFlush();

    glutSwapBuffers();
}


void KeyboardFunction(int key, int, int)
{
      if(!isGameStarted) return;

      switch (key) {
        case GLUT_KEY_UP:
            calculatedY = playerY + jumpPower * playerSize;
            playerY = std::min(4.5,calculatedY);
            timeSinceUpArrow = 0;
            break;
        default: return;
      }
      glutPostRedisplay();
}


static void idle(void)
{
    glutPostRedisplay();
}


int main(int argc, char *argv[])
{
    srand(GetTickCount());
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Flappy Bird");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutSpecialFunc(KeyboardFunction);
    glutTimerFunc(25,DecreasePlayerYValue,0);
    //glutTimerFunc(2000,MoveThePipes,1);
    glutTimerFunc(5000,IncreaseGameSpeed,0);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glutMouseFunc(mouse);
    glutMainLoop();

    return EXIT_SUCCESS;
}
