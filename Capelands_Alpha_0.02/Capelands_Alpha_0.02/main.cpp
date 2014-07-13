//
//  main.cpp
//  Capelands_Alpha_0.02
//
//  Created by Jacob on 7/12/14.
//  Copyright (c) 2014 placeholdername. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#pragma GCC diagnostic ignored "-Wwarning-flag" //ignores the depreciation warnings. You should probably remove this if you are fixing bugs.

// angle of rotation for the camera direction
float angleX = 0.0f; //for left-right
float angleY = 0.0f; //for up-down

// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f, ly = 0.0f, lsx = 0.0f, lsz = 0.0f;

// XZ position of the camera
float x=0.0f, z=5.0f, y = 1.75f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngleX = 0.0f;
float deltaAngleY = 0.0f;
float deltaMoveX = 0; //forward/backwards
float deltaMoveS = 0; //strafe


float mouseSensitivity = 0.01f; //what camera movements are multiplied by. Default = 0.001f

int xOrigin = -1;
int yOrigin = -1;

// width and height of the window
int h,w;

// variables to compute frames per second
int frame;
long newtime, timebase;
char s[50];

// variables to hold window identifiers
int mainWindow, subWindow1,subWindow2,subWindow3;
//border between subwindows
int border = 6;

void setProjection(int w1, int h1)
{
	float ratio;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	ratio = 1.0f * w1 / h1;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
	// Set the viewport to be the entire window
    glViewport(0, 0, w1, h1);
    
	// Set the clipping volume
	gluPerspective(45,ratio,0.1,1000);
	glMatrixMode(GL_MODELVIEW);
}

void changeSize(int w1,int h1) {
    
	if(h1 == 0)
		h1 = 1;
    
	// we're keeping these values cause we'll need them latter
	w = w1;
	h = h1;
    //
	// set subwindow 1 as the active window
	glutSetWindow(subWindow1);
	// resize and reposition the sub window
	glutPositionWindow(border,border);
	glutReshapeWindow(w-2*border, h/2 - border*3/2);
	setProjection(w-2*border, h/2 - border*3/2);
    
	// set subwindow 2 as the active window
	glutSetWindow(subWindow2);
	// resize and reposition the sub window
	glutPositionWindow(border,(h+border)/2);
	glutReshapeWindow(w/2-border*3/2, h/2 - border*3/2);
	setProjection(w/2-border*3/2,h/2 - border*3/2);
    
	// set subwindow 3 as the active window
	glutSetWindow(subWindow3);
	// resize and reposition the sub window
	glutPositionWindow((w+border)/2,(h+border)/2);
	glutReshapeWindow(w/2-border*3/2,h/2 - border*3/2);
	setProjection(w/2-border*3/2,h/2 - border*3/2);
}

void drawTower() {
    
	glColor3f(1.0f, 1.0f, 1.0f);
    
    //Base cube
	glTranslatef(0.0f , 1.0f, 0.0f);
	glutSolidCube(1);
    
    //Second cube
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidCube(1);
    
    //Green Middle
    glPushMatrix();
    glColor3f(0.0f,0.5f,0.0f);
	glTranslatef(0.0f, -0.5f, 0.0f);
	glutSolidCube(1.2);
	glPopMatrix();
    
    //reset the color
	glColor3f(1.0f, 1.0f, 1.0f);
    
}

void renderBitmapString(
                        float x,
                        float y,
                        float z,
                        void *font,
                        char *string) {
    
	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void restorePerspectiveProjection() {
    
	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();
    
	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {
    
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
    
	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
    
	// reset matrix
	glLoadIdentity();
    
	// set a 2D orthographic projection
	gluOrtho2D(0, w, h, 0);
    
	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void computePos(float deltaMoveX, float deltaMoveS) {
    
	x += deltaMoveX * lx * 0.1f;
	z += deltaMoveX * lz * 0.1f;
    
    x += deltaMoveS * lsx * 0.1f;
    z += deltaMoveS * lsz * 0.1f;
        //add strafe move here
}

// Common Render Items for all subwindows
void renderScene2() {
    
    // Draw ground
    
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
    
    // Draw 36 Towers
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++)
		{
			glPushMatrix();
			glTranslatef(i*10.0f, 0.0f, j * 10.0f);
			drawTower();
			glPopMatrix();
		}
}

// Display func for main window
void renderScene() {
	glutSetWindow(mainWindow);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

// Display func for sub window 1
void renderScenesw1() {
    
	glutSetWindow(subWindow1);
     glutSetCursor(GLUT_CURSOR_NONE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
	gluLookAt(x, y, z,
              x + lx,y + ly,z + lz,
              0.0f,1.0f,0.0f);
    
	renderScene2();
    
	// display fps in the top window
 	frame++;
    
	newtime=glutGet(GLUT_ELAPSED_TIME);
	if (newtime - timebase > 1000) {
		sprintf(s,"Capelands - FPS:%4.2f",
                frame*1000.0/(newtime-timebase));
		timebase = newtime;
		frame = 0;
	}
    
	setOrthographicProjection();
    
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(5,30,0,GLUT_BITMAP_HELVETICA_12,s);
	glPopMatrix();
    
	restorePerspectiveProjection();
    
	glutSwapBuffers();
}

// Display func for sub window 2
void renderScenesw2() {
    
	glutSetWindow(subWindow2);
     glutSetCursor(GLUT_CURSOR_NONE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
	gluLookAt(x, y+15, z,
              x ,y - 1,z,
              lx,0,lz);
    
	// Draw red cone at the location of the main camera
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(x,y,z);
	glRotatef(180-(angleX+deltaAngleX)*180.0/3.14,0.0,1.0,0.0);
	glutSolidCone(0.2,0.8f,4,4);
	glPopMatrix();
    
	renderScene2();
    
	glutSwapBuffers();
}

// Display func for sub window 3
void renderScenesw3() {
    
	glutSetWindow(subWindow3);
     glutSetCursor(GLUT_CURSOR_NONE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
	gluLookAt(x-lz*10 , y, z+lx*10,
              x ,y ,z ,
              0.0f,1.0f,0.0f);
    
	// Draw red cone at the location of the main camera
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(x,y,z);
	glRotatef(180-(angleX+deltaAngleX)*180.0/3.14,0.0,1.0,0.0);
	glutSolidCone(0.2,0.8f,4,4);
	glPopMatrix();
    
	renderScene2();
    
	glutSwapBuffers();
}

// Global render func
void renderSceneAll() {
    
	// check for keyboard movement
	//if (deltaMoveX || deltaMoveS) {
    computePos(deltaMoveX,deltaMoveS);
    glutSetWindow(mainWindow);
     glutSetCursor(GLUT_CURSOR_NONE);
    glutPostRedisplay();
	//}
    
	renderScene();
	renderScenesw1();
	renderScenesw2();
	renderScenesw3();
}

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void processNormalKeys(unsigned char key, int xx, int yy) {
    
    //cout<<key;
    //printf("key %d\n", key);
    printf("angle %f\n", angleX);
    printf("angle %f\n", deltaAngleX);


    switch (key) {
        case 119: deltaMoveX = 0.5f; break;
        case 115: deltaMoveX = -0.5f; break;
        case 100: deltaMoveS = 0.5f; break;
        case 97: deltaMoveS = -0.5; break;
        case 27 : glutDestroyWindow(mainWindow); exit(0);
    }
    
    glutSetWindow(mainWindow);
    glutPostRedisplay();
    
}

void pressKey(int key, int xx, int yy) {
    
	switch (key) {
		case GLUT_KEY_UP : deltaMoveX = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMoveX = -0.5f; break;
        case 119: deltaMoveX = 0.5f; break;
        case 115: deltaMoveX = -0.5f; break;
        case 100: deltaMoveS = 0.5f; break;
        case 97: deltaMoveS = -0.5; break;
	}
    
	glutSetWindow(mainWindow);
	glutPostRedisplay();
    
}

void releaseKey(int key, int x, int y) {
    
	switch (key) {
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN :
        case 119 :
        case 115 : deltaMoveX = 0; break;
            
        case 97 :
        case 100 : deltaMoveS = 0; break;
	}
}

// -----------------------------------
//             MOUSE
// -----------------------------------



void mouseMove(int x, int y) {
    
    glutSetWindow(mainWindow);

    
    deltaAngleX = (x - xOrigin);
    deltaAngleY = (y - yOrigin);
    
    int Xprogress = 0;
    int Yprogress = 0;
    
   // cout << deltaAngleX << "\n";
   // cout << deltaAngleY << "\n";
    

    
    
    
    deltaAngleY = (y - yOrigin + Yprogress) * mouseSensitivity;
    deltaAngleX = (x - xOrigin + Xprogress) * mouseSensitivity;

    // update camera's direction
    lx = sin(angleX + deltaAngleX);
    lz = -cos(angleX + deltaAngleX);
    ly = -sin(angleY + deltaAngleY);
    lsx = sin(angleX + deltaAngleX + 1.5707); //90 degrees in radians is 1.5707
    lsz = -cos(angleX + deltaAngleX + 1.5707);
    cout << "x:" << x << "\n";
    cout << "y:" << y << "\n";
    cout << "deltaAngleY:" << deltaAngleY << "\n";
    cout << "deltaAngleX:" << deltaAngleX << "\n";
    cout << "xOrigin:" << xOrigin << "\n";
    cout << "yOrigin:" << yOrigin << "\n";

    
    if( x <= -w/2 || (y) <= -h/2 || x >= w/2 || y >= h/2) {
        
        Xprogress += x + (x - xOrigin + Xprogress);
        Yprogress += y + (y - yOrigin + Yprogress);
        
        
        glutWarpPointer( w/2, h/2 ); // TODO: REPLACE THIS METHOD, IT CAUSES CHOPPY MOUSE MOVEMENT
        
        //NOTE: the x and y values of the game and the x/y values of the origin are not on the same window

        // Have to re-hide if the user touched any UI element with the invisible pointer, like the Dock.
        //CGDisplayHideCursor(kCGDirectMainDisplay);
        
        //	If on Mac OS X, the following will also work (and CGwarpMouseCursorPosition seems faster than glutWarpPointer).
        //	CGPoint centerPos = CGPointMake( windowX + lastX, windowY + lastY );
        //	CGWarpMouseCursorPosition( centerPos );
        // Have to re-hide if the user touched any UI element with the invisible pointer, like the Dock.
        //	CGDisplayHideCursor(kCGDirectMainDisplay);
    }
        
    angleX += deltaAngleX;
    angleY += deltaAngleY;
    
    xOrigin = x - Xprogress;
    yOrigin = y - Yprogress;
    
    //deltaAngleX = 0.0f;
    //deltaAngleY = 0.0f;
    
    
        
    //}
    
//    if (yOrigin >= 0) {
        //get looking up and down
//        deltaAngleY = (y - yOrigin) * mouseSensitivity;
        
//        ly = -sin(angleY + deltaAngleY);
//    }
    
	// this will only be true when the left button is down
//	if (xOrigin >= 0) {
        
		// update deltaAngleX
//		deltaAngleX = (x - xOrigin) * mouseSensitivity;
        
		// update camera's direction
//		lx = sin(angleX + deltaAngleX);
	//	lz = -cos(angleX + deltaAngleX);
  
//        lsx = sin(angleX + deltaAngleX + 1.5707); //90 degrees in radians is 1.5707
//        lsz = -cos(angleX + deltaAngleX + 1.5707);
        
        

//	}
}

void mouseButton(int button, int state, int x, int y) {
    
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {
        
		// when the button is released
		if (state == GLUT_UP)
        {
			angleX += deltaAngleX;
			deltaAngleX = 0.0f;
		}
		else  {// state = GLUT_DOWN
            
		}
	}
}

// -----------------------------------
//             MAIN and INIT
// -----------------------------------




void init() {
    
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    
	// register callbacks
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
    glutPassiveMotionFunc(mouseMove);
}

int main(int argc, char **argv) {
    
	// init GLUT and create main window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1200,1000);
	mainWindow = glutCreateWindow("Capelands Alpha 0.02");
    
    
	// callbacks for main window
	glutDisplayFunc(renderSceneAll);
	glutReshapeFunc(changeSize);
    
	// Removing the idle function to save CPU and GPU
	//glutIdleFunc(renderSceneAll);
	init();
    
	// sub windows
	subWindow1 = glutCreateSubWindow(mainWindow, border,border,w-2*border, h/2 - border*3/2);
	glutDisplayFunc(renderScenesw1);
	init();
    
	subWindow2 = glutCreateSubWindow(mainWindow, border,(h+border)/2,w/2-border*3/2, h/2 - border*3/2);
	glutDisplayFunc(renderScenesw2);
	init();
    
	subWindow3 = glutCreateSubWindow(mainWindow, (w+border)/2,(h+border)/2,w/2-border*3/2,h/2 - border*3/2);
	glutDisplayFunc(renderScenesw3);
	init();
    
	// enter GLUT event processing cycle
	glutMainLoop();
	
	return 1;
}






































