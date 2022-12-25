#include <algorithm>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <cstdio>

static double currentY = 0;
static double playerSize = 0.5;
static double timeSinceUpArrow = 0;
static double calculatedY = 0;
static double gravity = 0.035;
static double gravityWithTime = 0.02;
static double jumpPower = 1.5;
static double firstPipeX = -3,secondPipeX = 0,thirdPipeX = 3;

static double firstPipeSpaceY,secondPipeSpaceY,thirdPipeSpaceY;
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

void DecreaseYValue(int value)
{
    timeSinceUpArrow += gravityWithTime * playerSize;

    calculatedY = currentY - (timeSinceUpArrow + gravity) * playerSize;
    currentY = std::max(-4.5,calculatedY);
    glutPostRedisplay();
    glutTimerFunc(25, DecreaseYValue, 0);
}

void DrawSinglePipe(double pipeX,double pipeSpaceY)
{
    glBegin(GL_POLYGON);
    //upside
    glVertex3d(pipeX - defaultPipeWidth/2,pipeSpaceY + defaultPipeSpacing/2 ,-10);
    glVertex3d(pipeX + defaultPipeWidth/2,pipeSpaceY + defaultPipeSpacing/2 ,-10);
    glVertex3d(pipeX + defaultPipeWidth/2,pipeSpaceY + defaultPipeSpacing/2 + defaultPipeHeight ,-10);
    glVertex3d(pipeX - defaultPipeWidth/2,pipeSpaceY + defaultPipeSpacing/2 + defaultPipeHeight ,-10);
    glEnd();

    glBegin(GL_POLYGON);
    //downSide

    glVertex3d(pipeX - defaultPipeWidth/2,pipeSpaceY + defaultPipeSpacing/2 - defaultPipeHeight - defaultPipeSpacing ,-10);
    glVertex3d(pipeX + defaultPipeWidth/2,pipeSpaceY + defaultPipeSpacing/2 - defaultPipeHeight - defaultPipeSpacing ,-10);
    glVertex3d(pipeX + defaultPipeWidth/2,pipeSpaceY + defaultPipeSpacing/2 - defaultPipeSpacing ,-10);
    glVertex3d(pipeX - defaultPipeWidth/2,pipeSpaceY + defaultPipeSpacing/2 - defaultPipeSpacing ,-10);
    glEnd();

    glFlush();
}


void DrawPipes()
{
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f(0,1,0);

    //First Pipe
    firstPipeSpaceY = 2;
    DrawSinglePipe(firstPipeX,firstPipeSpaceY);

    secondPipeSpaceY = 2.5;
    DrawSinglePipe(secondPipeX,secondPipeSpaceY);

    thirdPipeSpaceY = 1.5;
    DrawSinglePipe(thirdPipeX,thirdPipeSpaceY);
}



static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    DrawPipes();

    //Main Character
    glColor3d(1.0,1.0,0);
    glPushMatrix();
        glTranslated(-5,currentY,-10);
        glRotated(a,0,0,1);
        glutSolidSphere(0.5,16,16);
    glPopMatrix();

    glutSwapBuffers();
}




void KeyboardFunction(int key, int, int) {
  switch (key) {
    case GLUT_KEY_UP:
        calculatedY = currentY + jumpPower * playerSize;
        currentY = std::min(4.5,calculatedY);
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
    glutTimerFunc(25,DecreaseYValue,0);
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
