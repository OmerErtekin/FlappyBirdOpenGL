#include <algorithm>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <cstdio>

static double time;
static double playerY = 0;
static double xProgress = -8;
static double speedFactor = 0.5;
static double playerSize = 0.5;
static double timeSinceUpArrow = 0;
static double calculatedY = 0;
static double gravity = 0.035;
static double gravityWithTime = 0.02;
static double jumpPower = 1.5;

double pipeXPositions[] = {-4,-0.5,3,6.5,9.5};
double pipeYPositions[] = {2,1,-1,-2,0};

static double defaultPipeSpacing = 3;
static double defaultPipeWidth = 1.5,defaultPipeHeight = 10;

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

void UpdatePipePositions()
{
    for(int i = 0;i<5;i++)
    {
        pipeXPositions[i] += xProgress / 20;
        printf("%f\n",pipeXPositions[i]);
        if(pipeXPositions[i] < -10)
          pipeXPositions[i] += 17;
    }
}


void ProgressThePlayer()
{
    const double time = glutGet(GLUT_ELAPSED_TIME) / 1000000.0;
    xProgress += time * speedFactor;
}
void DecreasePlayerYValue(int value)
{
    timeSinceUpArrow += gravityWithTime * playerSize;

    calculatedY = playerY - (timeSinceUpArrow + gravity) * playerSize;
    playerY = std::max(-4.5,calculatedY);
    glutPostRedisplay();
    UpdatePipePositions();
    glutTimerFunc(25, DecreasePlayerYValue, 0);
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
    for(int i = 0;i<5;i++)
    {
        DrawSinglePipe(pipeXPositions[i],pipeYPositions[i]);
    }

}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    ProgressThePlayer();
    DrawPipes();

    glColor3d(1.0,1.0,0);
    glPushMatrix();


    glPushMatrix();
        glTranslated(xProgress,playerY,-10);
        glutSolidSphere(0.5,16,16);
    glPopMatrix();

    glutSwapBuffers();
}




void KeyboardFunction(int key, int, int) {
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
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("FlappyBird");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutSpecialFunc(KeyboardFunction);
    glutTimerFunc(25,DecreasePlayerYValue,0);
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


    glutMainLoop();

    return EXIT_SUCCESS;
}
