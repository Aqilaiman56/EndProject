#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <cstdlib>

using namespace std;

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define PI 3.141592653
#define FPS 30

// Global variables
///////////////////////////////////// SCENE 1 & 2 GLOBAL VARIABLES //////////////////////////////////////
int delay = 1000 / FPS; // Delay between frames in milliseconds
float scaleX = 1.0f;
float scaleY = 1.0f;
float scaleSpeed = 0.05f; // adjust this for desired speed
int delaySecondsBB = 3; // Delay before transitioning to monitor scene (in seconds)
int delaySecondsMonitor = 5; // Delay before scaling monitor (in seconds)
int delayBeforeZoom = 3; // Delay before starting the zoom animation (in seconds)

bool firstTextDisplayed = false;
int loadingTime = 0;
int startTime = 0;
bool loadingComplete = false;

// Animation states
enum AnimationState {
    INTRO,
    SCENE_1,
    TURN_ON_MONITOR,
    BITS_AND_BYTES_SCREEN,
    CHARACTERS_MOVES,
    IDE_WINDOW_POP_UP,
    BIT_AND_BYTE_TYPING,

};

AnimationState currentState = INTRO;

///////////////////////////////////// SCENE 3 GLOBAL VARIABLES //////////////////////////////////////////
// Global variables to store the current position of characters
GLfloat byte_x = 300, byte_y = 40; // Initial position of Byte
GLfloat bit_x = 700, bit_y = 40;   // Initial position of Bit
GLfloat virus_x = 100, virus_y = 700;   // Initial position of virus

// Target positions for the characters
GLfloat target_byte_x = 50, target_byte_y = 50; // Target position of Byte
GLfloat target_bit_x = 50, target_bit_y = 50;   // Target position of Bit
GLfloat target_virus_x = 800, target_virus_y = 150;   // Target position of virus

// Duration of the movement transition (in milliseconds)
const int transition_duration = 5000; // 5 seconds

// Number of frames for the transition
const int num_frames = 100; // Adjust for desired smoothness

// Incremental movement for each frame
GLfloat byte_dx, byte_dy; // Incremental movement for Byte
GLfloat bit_dx, bit_dy;   // Incremental movement for Bit
GLfloat virus_dx, virus_dy; // incremental movement for virus

int total_elapsed_time = 0; // Global variable to track total elapsed time
const int STOP_DURATION = 8000; // Stop duration in milliseconds (e.g., 11 seconds)

///////////////////////////////////// SCENE 4 GLOBAL VARIABLES //////////////////////////////////////////
int frameCount = 0;
int shotsFired = 0;
float gunAlpha = (frameCount - 30) / 30.0f;
float rifleAlpha = (frameCount - 30) / 30.0f;
bool gunPulled = false; // Flag to track whether the gun is pulled out
int gunTransparency = 0; // Transparency level of the gun
float bitRotation = 0.0f;
bool allAnimationsFinished = false;
int totalFrames = 480;

///////////////////////////////////// SCENE 5 GLOBAL VARIABLES //////////////////////////////////////////
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BIT_SPEED 1.5  // Speed of Bit's movement (pixels per frame)
#define BYTE_SPEED 1.5 // Speed of Byte's movement (pixels per frame)
#define EYE_CHANGE_DELAY 500 // Eye change delay in milliseconds
#define JUMP_HEIGHT 180.0 // Height of the jump (pixels)
#define JUMP_SPEED 4.0
#define SPARKLE_COUNT 10 // Number of sparkles
#define MIN_SPARKLE_DISTANCE 100.0 // Minimum distance between sparkles

float byteX = SCREEN_WIDTH + 110.0;
float bitX = -180.0;
float bitY = 50.0;
float byteY = 50.0;

bool bitLookRight = true;   // Initial direction for Bit's eyes
bool byteLookRight = true;  // Initial direction for Byte's eyes
float bitEyeOffset = 0.0;   // Offset for Bit's pupil within the eye
float byteEyeOffset = 0.0;  // Offset for Byte's pupil within the eye

bool bitStopped = false;
bool byteStopped = false;

bool bitHappy = false; // Flag to indicate if Bit's eyes should be happy
bool byteHappy = false; // Flag to indicate if Byte's eyes should be happy

bool jumpingUp = false;
bool jumpingDown = false;
float jumpHeight = JUMP_HEIGHT;
// Structure to hold sparkle properties
struct Sparkle {
    float x;
    float y;
    float size;
    float maxSize;
    bool growing;
};
// Array of sparkles
Sparkle sparkles[SPARKLE_COUNT];
bool curtainClosing = false;
float leftCurtainPosition = -1.0;  // Start position of the left curtain
float rightCurtainPosition = 1001.0; // Start position of the right curtain
float curtainSpeed = 2.0; // Adjust this value to control the speed of the curtain descent
int curtainDelay = 10000; // Delay in milliseconds before curtain animation starts




///////////////////////////////////// FUNCTION PROTOTYPES ///////////////////////////////////////////////
void initGL();
void display();
// Scene 1
void renderText(float x, float y, void* font, const char* string, int r, int g, int b);
void rectangle(int x, int y, int w, int h, int r, int g, int b);
void monitorOnDesk(int x, int y, int w, int h);
void zoomInMonitor(int x, int y, int w, int h);
void drawBBStudio();
void drawBitsBytes();
void displayIntro();
void displayMonitorScene();
// Scene 2
void displayLoadingScreen();
void displayScene1();
void animateScene1(int value);
void animateIntro(int value);
// Scene 3
void timer(int value);
void monitor(int x, int y, int w, int h);
void drawVirus(float x, float y);
// Scene 4
void drawScope(double centerX, double centerY, double radius);
void drawSniperRifle(int x, int y, int w, int h, float alpha);
void drawGun(int x, int y, float alpha);
void drawExplosion(int frame);
void drawBullet(int x, int y, int a, int b);
void bit_byte(int frame);
void fireBullets(int frame);
void glitchEffect(int frame);
void explosionEffect(int frame);
void finalBlackScreen(int frame);
void scene4(int value);
// Scene 5
void drawHappyEye(double centerX, double centerY, double eyeRadius);
void drawCurtain(float leftPos, float rightPos);
void drawSparkle(float x, float y, float size);

//////////////////////////////////////// ANIMATION FUNCTIONS ////////////////////////////////////////////
// BIT&BYTES //
void rectangle(int x, int y, int w, int h, int r, int g, int b) {
    glBegin(GL_POLYGON);
    glColor3ub(r, g, b);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
}
void drawCircle(double centerX, double centerY, double radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i++) {
        double angle = i * PI / 180.0;
        double x = centerX + radius * cos(angle);
        double y = centerY + radius * sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}
void drawBody(double centerX, double centerY, double bodyRadius, double holeRadius) {
    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Circle
    drawCircle(centerX, centerY, bodyRadius);

    //Holes
    // Set color with alpha (last parameter)
    glColor4ub(0, 0, 0, 128); // 128 for half transparency
    drawCircle(centerX, centerY, holeRadius + 5);

    // Disable blending after drawing
    glDisable(GL_BLEND);
}
void drawEye(double centerX, double centerY, double eyeRadius) {
    // White part of the eye
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(centerX, centerY, eyeRadius);

    // Black pupil
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(centerX, centerY, eyeRadius / 3);
}
void drawHappyEye(double centerX, double centerY, double eyeRadius) {
    glLineWidth(5.0); // Set line width for drawing

    // Draw the upper part of the happy eye (curved line)
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i++) {
        double angle = i * M_PI / 180.0;
        double x = centerX + eyeRadius * cos(angle);
        double y = centerY + eyeRadius * sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}

void drawByte(int x, int y, int w, int h) {
    // Draw the character "Byte"
    glColor3ub(0, 255, 255);
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();

    // Draw the body drawBody(x-axis, y-axis, bodyRadius, holeRadius)
    drawBody((x + w * 1.23), (y + h / 3), 50, 15);
    glColor3ub(0, 255, 255);
    drawBody((x + w * 1.23) - 5, (y + h / 3) + 60, 40, 10);

    // Draw the eyes drawEye(x-axis, y-axis, radius)
    drawEye((x + w / 3), (y + h / 1.3), 18);  // Draw eye 1
    drawEye((x + 3 * w / 2), (y + h / 1.3), 18);  // Draw eye 2
}
void drawBit(int x, int y, int w, int h) {
    // Draw the character "Byte"
    glColor3ub(255, 0, 255);
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();

    // Draw the body drawBody(x-axis, y-axis, bodyRadius, holeRadius)
    drawBody((x + w * 1.23) - 5, (y + h / 3) - 10, 40, 10);

    // Draw the eyes drawEye(x-axis, y-axis, radius)
    drawEye((x + w / 5), (y + h / 1.3), 15);  // Draw eye 1
    drawEye((x + w), (y + h / 1.3), 15);  // Draw eye 2
}

void displayBitsAndBytes() {

    static bool moveCharacter = true;

    printf("Printing Bits and Bytes\n");
    glClear(GL_COLOR_BUFFER_BIT);
    monitor(0, 0, 1000, 600);

    static bool AtNewCoordinates = false;
    static int delayCounter = 0;
    static double byteY = -150; // Initial y-coordinate of "Byte"
    static double bitY = -150;  // Initial y-coordinate of "Bit"

    if (!AtNewCoordinates) {
        if (moveCharacter) {
            // Update y-coordinates for animation
            if (byteY < 60) {
                byteY += 1;
            }
            if (bitY < 60) {
                bitY += 1;
            }

            // Draw "Byte" and "Bit" with updated coordinates
            drawByte((SCREEN_WIDTH - 200) / 2 - 50, byteY, 40, 150); // Centered and shifted to the left
            drawBit((SCREEN_WIDTH + 200) / 2 + 50, bitY, 40, 150);  // Centered and shifted to the right

            glutSwapBuffers();
            if (byteY < 60 || bitY < 60) {
                glutPostRedisplay(); // Continue animation until y-coordinate reaches 200
            }
            else {
                AtNewCoordinates = true; // Set flag to indicate that characters have reached their destination
            }
        }
        else {
            // Update y-coordinates for animation
            if (byteY < 60) {
                byteY += 0.5;
            }
            if (bitY < 60) {
                bitY += 0.5;
            }

            // Draw "Byte" and "Bit" with updated coordinates
            drawByte((SCREEN_WIDTH - 200) / 2 - 50, byteY, 40, 150); // Centered and shifted to the left
            drawBit((SCREEN_WIDTH + 200) / 2 + 50, bitY, 40, 150);  // Centered and shifted to the right

            glFlush();
            if (byteY < 60 || bitY < 60) {
                glutPostRedisplay(); // Continue animation until y-coordinate reaches 200
            }
        }
    }
    else {
        // Stay at the final coordinates for a few seconds
        delayCounter++;
        if (delayCounter > FPS * 1) { // Change the 1 to adjust the delay duration
            currentState = CHARACTERS_MOVES; // Switch to the next state
            glutPostRedisplay(); // Request a redraw to start the new animation
        }
    }
}
void drawBitsBytes() {
    renderText(SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2, GLUT_BITMAP_TIMES_ROMAN_24, "'Bits & Bytes'", 255, 255, 255);
}

// MONITOR & BACKGROUND //
void renderText(float x, float y, void* font, const char* string, int r, int g, int b) {
    glColor3ub(r, g, b);
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(font, *string);
        string++;
    }
}
void monitorOnDesk(int x, int y, int w, int h) {
    // Background
    rectangle(x, y, x + SCREEN_WIDTH, y + SCREEN_HEIGHT, 224, 224, 224);
    // Desk
    rectangle(x, y + SCREEN_HEIGHT / 5, x + SCREEN_WIDTH, y + SCREEN_HEIGHT / 5, 255, 204, 153);
    // Monitor
    rectangle(x + SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4 / 2, y + SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 4 / 2, x + SCREEN_WIDTH / 4, y + SCREEN_HEIGHT / 4, 96, 96, 96); // Outer monitor
    rectangle(x + SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4 / 2 + 5, y + SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 4 / 2 + 5, x + SCREEN_WIDTH / 4 - 10, y + SCREEN_HEIGHT / 4 - 10, 16, 16, 16); // Inner monitor
    rectangle(x + SCREEN_WIDTH / 2 - SCREEN_WIDTH / 20 / 2, y + SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 25 - SCREEN_HEIGHT / 4 / 2, x + SCREEN_WIDTH / 20, y + SCREEN_HEIGHT / 25, 128, 128, 128); // Monitor leg
    rectangle(x + SCREEN_WIDTH / 2 - SCREEN_WIDTH / 10 / 2, y + SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 25 - SCREEN_HEIGHT / 4 / 2, x + SCREEN_WIDTH / 10, SCREEN_HEIGHT / 100, 120, 120, 120); // Monitor base
}
void zoomInMonitor(int x, int y, int w, int h) {
    int monitorCenterX = SCREEN_WIDTH / 2; // X-coordinate of the center of the outer monitor
    int monitorCenterY = SCREEN_HEIGHT / 2; // Y-coordinate of the center of the outer monitor



    glPushMatrix(); // Push current matrix onto stack
    glTranslatef(monitorCenterX, monitorCenterY, 0); // Translate to the center of the outer monitor
    glScalef(scaleX, scaleY, 1.0f); // Scale the monitor
    glTranslatef(-monitorCenterX, -monitorCenterY, 0); // Translate back to the original position
    monitorOnDesk(0, 0, 0, 0);

    glPopMatrix(); // Pop matrix from stack, restoring previous state
}
void drawBBStudio() {
    renderText(SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2, GLUT_BITMAP_TIMES_ROMAN_24, "B&B Studio Animation presents", 255, 255, 255);
}
void folderIcon(int x, int y) {
    glBegin(GL_POLYGON);
    glColor3ub(247, 238, 159);
    glVertex2i(x, y);
    glVertex2i(x, y + 40);
    glVertex2i(x + 5, y + 45);
    glVertex2i(x + 15, y + 45);
    glVertex2i(x + 30, y + 40);
    glVertex2i(x + 50, y + 40);
    glVertex2i(x + 50, y);
    glEnd();
}
void renderText(float x, float y, void* font, const char* string) {
    glColor3ub(0, 0, 0);
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(font, *string);
        string++;
    }
}
//bright theme background
void monitor(int x, int y, int w, int h) {
    //outer monitor
    rectangle(x, y, w, h, 0, 0, 0);

    //screen
    //desktop bcg color - teal
    rectangle(x + 20, y + 20, w - 40, h - 40, 85, 128, 129);

    //task bar
    rectangle(x + 20, y + 20, w - 40, 48, 192, 192, 192);
    rectangle(x + 20, y + 66, w - 40, 1, 220, 220, 220); //highlightTaskbar

    //start button
    glColor3ub(190, 190, 190);
    rectangle(x + 25, y + 25, 80, 38, 186, 187, 189); //startButton
    rectangle(x + 26, y + 23, 80, 2, 145, 145, 145); //shadowBottom
    rectangle(x + 106, y + 23, 2, 38, 145, 145, 145); //shadowLeft
    rectangle(x + 25, y + 60, 80, 2, 220, 220, 220); //highlightTop
    rectangle(x + 25, y + 25, 2, 36, 220, 220, 220); //highlightRight
    renderText(x + 40, y + 35, GLUT_BITMAP_TIMES_ROMAN_24, "Start");

    //clock
    glColor3ub(0, 0, 0);
    rectangle(x + 860, y + 28, 110, 30, 188, 189, 191); //clockBox
    rectangle(x + 860, y + 27, 110, 2, 220, 220, 220); //highlightBottom
    rectangle(x + 970, y + 27, 2, 31, 220, 220, 220); //highlightLeft
    rectangle(x + 860, y + 29, 2, 30, 145, 145, 145); //shadowTop
    rectangle(x + 860, y + 58, 110, 2, 145, 145, 145); //shadowRight
    renderText(x + 870, y + 35, GLUT_BITMAP_TIMES_ROMAN_24, "9:00 AM");

    //icon1
    folderIcon(40, 520);
    renderText(x + 40, y + 500, GLUT_BITMAP_TIMES_ROMAN_10, "GRAPHICS");

    //icon2
    folderIcon(40, 420);
    renderText(x + 55, y + 400, GLUT_BITMAP_TIMES_ROMAN_10, "OOP");

    //icon3
    folderIcon(40, 320);
    renderText(x + 50, y + 300, GLUT_BITMAP_TIMES_ROMAN_10, "CODE");

    //IDE icon
    rectangle(x + 190, y + 28, 26, 26, 0, 0, 0);
    rectangle(x + 192, y + 30, 12, 12, 255, 225, 0);
    rectangle(x + 205, y + 30, 12, 12, 0, 0, 225);
    rectangle(x + 192, y + 43, 12, 12, 225, 0, 0);
    rectangle(x + 205, y + 43, 12, 12, 0, 225, 0);

}
//dark theme background
void darkMonitor(int x, int y, int w, int h) {
    //outer monitor
    rectangle(x, y, w, h, 0, 0, 0);

    //screen
    //desktop bcg color - red
    rectangle(x + 20, y + 20, w - 40, h - 40, 159, 56, 49);

    //task bar
    rectangle(x + 20, y + 20, w - 40, 48, 192, 192, 192);
    rectangle(x + 20, y + 66, w - 40, 1, 220, 220, 220); //highlightTaskbar

    //start button
    glColor3ub(190, 190, 190);
    rectangle(x + 25, y + 25, 80, 38, 186, 187, 189); //startButton
    rectangle(x + 26, y + 23, 80, 2, 145, 145, 145); //shadowBottom
    rectangle(x + 106, y + 23, 2, 38, 145, 145, 145); //shadowLeft
    rectangle(x + 25, y + 60, 80, 2, 220, 220, 220); //highlightTop
    rectangle(x + 25, y + 25, 2, 36, 220, 220, 220); //highlightRight
    renderText(x + 40, y + 35, GLUT_BITMAP_TIMES_ROMAN_24, "tratS");

    //clock
    glColor3ub(0, 0, 0);
    rectangle(x + 860, y + 28, 110, 30, 188, 189, 191); //clockBox
    rectangle(x + 860, y + 27, 110, 2, 220, 220, 220); //highlightBottom
    rectangle(x + 970, y + 27, 2, 31, 220, 220, 220); //highlightLeft
    rectangle(x + 860, y + 29, 2, 30, 145, 145, 145); //shadowTop
    rectangle(x + 860, y + 58, 110, 2, 145, 145, 145); //shadowRight
    renderText(x + 870, y + 35, GLUT_BITMAP_TIMES_ROMAN_24, "MA 00:9");

    //icon1
    folderIcon(60, 510);
    renderText(x + 50, y + 500, GLUT_BITMAP_TIMES_ROMAN_10, "SCIHPAG");

    //icon2
    folderIcon(35, 400);
    renderText(x + 60, y + 390, GLUT_BITMAP_TIMES_ROMAN_10, "POO");

    //icon3
    folderIcon(50, 290);
    renderText(x + 60, y + 270, GLUT_BITMAP_TIMES_ROMAN_10, "EDOC");

    //IDE icon
    rectangle(x + 190, y + 28, 26, 26, 0, 0, 0);
    rectangle(x + 192, y + 30, 12, 12, 0, 225, 0);
    rectangle(x + 205, y + 30, 12, 12, 255, 225, 0);
    rectangle(x + 192, y + 43, 12, 12, 0, 0, 225);
    rectangle(x + 205, y + 43, 12, 12, 225, 0, 0);

}
//IDE window design
void IDE_window(int x, int y, int w, int h) {
    //outer part
    rectangle(x + 180, y + 120, w + 640, h + 400, 192, 192, 192);
    rectangle(x + 180, y + 520, w + 640, 3, 220, 220, 220); //highlight
    rectangle(x + 180, y + 120, w + 3, 400, 220, 220, 220); //highlight
    rectangle(x + 183, y + 120, w + 640, 3, 145, 145, 145); //shadow
    rectangle(x + 820, y + 120, w + 3, 403, 145, 145, 145); //shadow
    //inner part
    rectangle(x + 186, y + 125, w + 631, h + 370, 225, 225, 225);
    rectangle(x + 186, y + 475, w + 631, h + 3, 192, 192, 192);
    rectangle(x + 290, y + 125, w + 3, h + 350, 192, 192, 192);
    //exit button
    rectangle(x + 780, y + 500, w + 30, h + 18, 225, 0, 0);
    renderText(x + 792, y + 505, GLUT_BITMAP_TIMES_ROMAN_10, "X");
    //window name
    renderText(x + 190, y + 505, GLUT_BITMAP_TIMES_ROMAN_10, "IDE Window");

    //updated IDE icon
    rectangle(x + 180, y + 20, w + 48, 48, 220, 220, 220);
    rectangle(x + 190, y + 28, 26, 26, 0, 0, 0);
    rectangle(x + 192, y + 30, 12, 12, 255, 225, 0);
    rectangle(x + 205, y + 30, 12, 12, 0, 0, 225);
    rectangle(x + 192, y + 43, 12, 12, 225, 0, 0);
    rectangle(x + 205, y + 43, 12, 12, 0, 225, 0);

}
//first part of the code
void code1() {
    renderText(300, 450, GLUT_BITMAP_TIMES_ROMAN_10, "int x = 10;");
    renderText(300, 435, GLUT_BITMAP_TIMES_ROMAN_10, "for(int i = 0; i < x; i++) {");
    renderText(300, 420, GLUT_BITMAP_TIMES_ROMAN_10, "    printf(''Hello, ''); ");
    renderText(300, 405, GLUT_BITMAP_TIMES_ROMAN_10, "}");
    printf("Printing code 1\n");
}
//second part of the code
void code2() {
    renderText(300, 385, GLUT_BITMAP_TIMES_ROMAN_10, "char str[6] = {'w', 'o', 'r', 'l', 'd', '\0'};");
    renderText(300, 370, GLUT_BITMAP_TIMES_ROMAN_10, "for(int i = 0; i < strlen(str); i++) {");
    renderText(300, 355, GLUT_BITMAP_TIMES_ROMAN_10, "    putchar(str[i]);");
    renderText(300, 340, GLUT_BITMAP_TIMES_ROMAN_10, "}");
}
//third part of the code
void code3() {
    renderText(300, 305, GLUT_BITMAP_TIMES_ROMAN_10, "char password[] = ''g1bb3r1$hP@ssw0rd''; ");
    renderText(300, 290, GLUT_BITMAP_TIMES_ROMAN_10, "if(strcmp(user_input, password) == 0) {");
    renderText(300, 275, GLUT_BITMAP_TIMES_ROMAN_10, "    execute_virus();");
    renderText(300, 260, GLUT_BITMAP_TIMES_ROMAN_10, "}");
}
//error screen design
void displayERROR() {
    monitor(0, 0, 1000, 600);
    //screen
    //desktop bcg color - blue
    rectangle(20, 20, 1000 - 40, 600 - 40, 0, 0, 225);

    //task bar
    rectangle(20, 20, 1000 - 40, 48, 192, 192, 192);
    rectangle(20, 66, 1000 - 40, 1, 220, 220, 220); //highlightTaskbar
    renderText(460, 350, GLUT_BITMAP_HELVETICA_18, "WARNING!");
    renderText(380, 300, GLUT_BITMAP_HELVETICA_18, "VIRUS HAS BEEN DETECTED.");
}

// VIRUS MODEL //
void drawVirus(float x, float y) {
    glTranslatef(x, y, 0.0);

    // Draw main body (circle) with red filling
    glColor3f(1.0, 0.0, 0.0); // Red color
    float radius = 50; // Increase the size for better visibility
    int num_segments = 100; // Increase the number of segments for smoother circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments); // Calculate the angle
        float x_pos = radius * cosf(theta); // Calculate the x position
        float y_pos = radius * sinf(theta); // Calculate the y position
        glVertex2f(x_pos, y_pos); // Add vertex to draw the circle
    }
    glEnd();

    // Increase the thickness of body outline
    glLineWidth(5.0);

    // Draw outline of the main body in black
    glColor3f(0.0, 0.0, 0.0); // Black color
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159 / 180;
        glVertex2f(radius * cos(theta), radius * sin(theta));
    }
    glEnd();

    // Increase the thickness of antennas
    glLineWidth(3.0);

    // Draw antennas as curved lines (Bezier curves)
    glColor3f(0.0, 0.0, 0.0); // Black color
    glBegin(GL_LINE_STRIP);
    // Starting point of the curve
    glVertex2f(-0.01, radius);
    // Control points for the curve
    glVertex2f(-0.015, radius + 0.025);
    glVertex2f(-0.03, radius + 0.05);
    glVertex2f(-0.045, radius + 0.075);
    glVertex2f(-0.06, radius + 0.1);
    // Ending point of the curve
    glVertex2f(-0.075, radius + 0.1);
    glEnd();

    glBegin(GL_LINE_STRIP);
    // Starting point of the curve
    glVertex2f(0.01, radius);
    // Control points for the curve
    glVertex2f(0.015, radius + 0.025);
    glVertex2f(0.03, radius + 0.05);
    glVertex2f(0.045, radius + 0.075);
    glVertex2f(0.06, radius + 0.1);
    // Ending point of the curve
    glVertex2f(0.075, radius + 0.1);
    glEnd();

    // Increase the thickness of legs
    glLineWidth(5.0);

    // Draw legs
    glBegin(GL_LINES);
    // First set of legs, +0.9 +0.5
    glVertex2f(-radius * 0.8, -radius * 0.5); // Start of left leg
    glVertex2f(-radius * 1.7, -radius * 1.0); // End of left leg
    glVertex2f(radius * 0.8, -radius * 0.5); // Start of right leg
    glVertex2f(radius * 1.7, -radius * 1.0); // End of right leg
    // Extra leg, -0.2 +0.9
    glVertex2f(-radius * 1.7, -radius * 1.0); // Start of extra left leg
    glVertex2f(-radius * 1.5, -radius * 1.9); // End of extra left leg
    glVertex2f(radius * 1.7, -radius * 1.0); // Start of extra right leg
    glVertex2f(radius * 1.5, -radius * 1.9); // End of extra right leg

    // Second set of legs, +1.1, -0.1
    glVertex2f(-radius * 1.0, -radius * -0.2); // Start of left leg
    glVertex2f(-radius * 2.1, -radius * -0.3); // End of left leg
    glVertex2f(radius * 1.0, -radius * -0.2); // Start of right leg
    glVertex2f(radius * 2.1, -radius * -0.3); // End of right leg
    // Extra leg, +0.5 +2.2
    glVertex2f(-radius * 2.1, -radius * -0.3); // Start of extra left leg
    glVertex2f(-radius * 2.6, -radius * 1.9); // End of extra left leg
    glVertex2f(radius * 2.1, -radius * -0.3); // Start of extra right leg
    glVertex2f(radius * 2.6, -radius * 1.9); // End of extra right leg

    glEnd();

    // Reset translation for subsequent drawings
    glTranslatef(-x, -y, 0.0);
}

// WEAPON //
void drawScope(double centerX, double centerY, double radius) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 360; i++) {
        double angle = i * PI / 180.0;
        double x = centerX + radius * cos(angle);
        double y = centerY + radius * sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}
void drawSniperRifle(int x, int y, int w, int h, float alpha) {
    // Draw rifle barrel
    glColor4f(128 / 255.0f, 128 / 255.0f, 128 / 255.0f, alpha); // Gray color for the rifle body
    glBegin(GL_POLYGON);
    glVertex2i(x - 35, y);
    glVertex2i(x + w + 15, y);
    glVertex2i(x + w + 15, y + h / 3);
    glVertex2i(x - 35, y + h / 3);
    glEnd();

    // Draw rifle body
    glColor4f(100 / 255.0f, 50 / 255.0f, 0.0, alpha); // Gray color for the barrel
    glBegin(GL_POLYGON);
    glVertex2i(x - w / 3, y - h / 3 + 20);
    glVertex2i(x - w / 2 + w, y - h / 3 + 20);
    glVertex2i(x - w / 2 + w, y - h + 20);
    glVertex2i(x - w / 2, y - h + 10);
    glEnd();

    glColor4f(100 / 255.0f, 50 / 255.0f, 0.0, alpha);
    glBegin(GL_POLYGON);
    glVertex2i(x - w, y - h / 3 + 10);
    glVertex2i(x - w + w, y - h / 3 + 10);
    glVertex2i(x - w + w, y - h + 10);
    glVertex2i(x - w, y - h);
    glEnd();

    // Draw rifle scope
    glColor4f(105 / 255.0f, 105 / 255.0f, 105 / 255.0f, alpha); // Dim gray color for the scope
    glBegin(GL_POLYGON);
    glVertex2i(x / 1.3, y + 2 * h / 5);
    glVertex2i(x + w / 4, y + 2 * h / 5);
    glVertex2i(x + w / 4, y + 7 * h / 10);
    glVertex2i(x / 1.3, y + 7 * h / 10);
    glEnd();

    // Draw scope circle
    glColor4f(85 / 255.0f, 85 / 255.0f, 85 / 255.0f, alpha); // Dim gray color for the circle
    drawScope(x - 10, y + 1.5 * h / 3 + h / 16, w / 12);
}
void drawGun(int x, int y, float alpha) {
    // Set the alpha value for the gun
    glColor4f(0.0, 0.0, 0.0, alpha); // Black color with variable alpha{
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + 12, y);
    glVertex2i(x + 12, y + 20);
    glVertex2i(x, y + 20);
    glEnd();

    glColor4f(0.0, 0.0, 0.0, alpha);;
    glBegin(GL_QUADS);
    glVertex2i(x, y + 20);
    glVertex2i(x + 35, y + 20);
    glVertex2i(x + 35, y + 10);
    glVertex2i(x, y + 10);
    glEnd();

}
void drawExplosion(int frame) {
    float duration = 3.0f; // Duration of the animation in seconds
    float scale = (float)frame / (duration * FPS); // Calculate scale based on frame count and FPS

    // Draw the explosion
    glColor3ub(255, 165, 0); // Orange color
    glBegin(GL_POLYGON);
    glVertex2i(SCREEN_WIDTH, SCREEN_HEIGHT / 2);
    glVertex2i(SCREEN_WIDTH - 70 * scale, SCREEN_HEIGHT / 2 + 150 * scale);
    glVertex2i(SCREEN_WIDTH - 160 * scale, SCREEN_HEIGHT / 2 - 75 * scale);
    glVertex2i(SCREEN_WIDTH - 200 * scale, SCREEN_HEIGHT / 2 + 125 * scale);
    glVertex2i(SCREEN_WIDTH - 130 * scale, SCREEN_HEIGHT / -65 * scale);
    glVertex2i(SCREEN_WIDTH - 350 * scale, SCREEN_HEIGHT / 2 + 70 * scale);
    glVertex2i(SCREEN_WIDTH - 275 * scale, SCREEN_HEIGHT / 2 - 70 * scale);
    glVertex2i(SCREEN_WIDTH - 450 * scale, SCREEN_HEIGHT / 2 - 35 * scale);
    glVertex2i(SCREEN_WIDTH - 280 * scale, SCREEN_HEIGHT / 3 - 25 * scale);
    glVertex2i(SCREEN_WIDTH - 450 * scale, SCREEN_HEIGHT / 3 - 50 * scale);
    glVertex2i(SCREEN_WIDTH - 370 * scale, SCREEN_HEIGHT / 6 - 25 * scale);
    glVertex2i(SCREEN_WIDTH - 460 * scale, SCREEN_HEIGHT / 6 - 80 * scale);
    glVertex2i(SCREEN_WIDTH - 360 * scale, 0);
    glVertex2i(SCREEN_WIDTH, 0);
    glEnd();

    glColor3ub(0, 0, 0); // Black color
    glBegin(GL_POLYGON);
    glVertex2i(SCREEN_WIDTH, SCREEN_HEIGHT / 4 + 50);
    glVertex2i(SCREEN_WIDTH - 100 / 2 * scale, SCREEN_HEIGHT / 4 + 150 * scale);
    glVertex2i(SCREEN_WIDTH - 160 / 2 * scale, SCREEN_HEIGHT / 4 - 75 * scale);
    glVertex2i(SCREEN_WIDTH - 200 / 2 * scale, SCREEN_HEIGHT / 4 + 125 * scale);
    glVertex2i(SCREEN_WIDTH - 130 / 2 * scale, SCREEN_HEIGHT / 4 - 65 * scale);
    glVertex2i(SCREEN_WIDTH - 350 / 1.5 * scale, SCREEN_HEIGHT / 4 + 70 * scale);
    glVertex2i(SCREEN_WIDTH - 275 / 1.5 * scale, SCREEN_HEIGHT / 4 - 70 * scale);
    glVertex2i(SCREEN_WIDTH - 450 / 1.5 * scale, SCREEN_HEIGHT / 4 - 15 * scale);
    glVertex2i(SCREEN_WIDTH - 280 / 1.5 * scale, SCREEN_HEIGHT / 6 - 25 * scale);
    glVertex2i(SCREEN_WIDTH - 450 / 1.5 * scale, SCREEN_HEIGHT / 6 - 15 * scale);
    glVertex2i(SCREEN_WIDTH - 370 / 1.5 * scale, SCREEN_HEIGHT / 8 - 25 * scale);
    glVertex2i(SCREEN_WIDTH - 460 / 1.5 * scale, SCREEN_HEIGHT / 6 - 80 * scale);
    glVertex2i(SCREEN_WIDTH - 360 / 1.5 * scale, 0);
    glVertex2i(SCREEN_WIDTH, 0);
    glEnd();

}
void drawBullet(int x, int y, int a, int b) {
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2i(x, y);
    glVertex2i(a, b);
    glEnd();
}

// CURTAIN & SPARKLE //
void drawCurtain(float leftPos, float rightPos) {
    // Draw the left curtain
    glColor3ub(191, 27, 0);
    glBegin(GL_POLYGON);
    glVertex2f(leftPos, 0);
    glVertex2f(0, 0);
    glVertex2f(0, SCREEN_HEIGHT);
    glVertex2f(leftPos, SCREEN_HEIGHT);
    glEnd();

    // Draw the right curtain
    glBegin(GL_POLYGON);
    glVertex2f(rightPos, 0);
    glVertex2f(SCREEN_WIDTH, 0);
    glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    glVertex2f(rightPos, SCREEN_HEIGHT);
    glEnd();

    // Draw yellow line at the bottom of left curtain
    glColor3ub(255, 215, 0);
    glLineWidth(50.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0, 40.0);
    glVertex2f(leftPos, 40.0);
    glEnd();

    glColor3ub(255, 255, 0);
    glLineWidth(30.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0, 0);
    glVertex2f(leftPos, 0);
    glEnd();

    // Draw yellow line at the bottom of right curtain
    glColor3ub(255, 215, 0);
    glLineWidth(50.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(rightPos, 40.0);
    glVertex2f(SCREEN_WIDTH, 40.0);
    glEnd();

    glColor3ub(255, 255, 0);
    glLineWidth(30.0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(rightPos, 0);
    glVertex2f(SCREEN_WIDTH, 0);
    glEnd();
}
void drawSparkle(float x, float y, float size) {
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white

    // Define the outer points of the star
    float outerRadius = 0.7f * size; // Scale outer radius by size
    float innerRadius = 0.15f * size; // Scale inner radius by size
    int numPoints = 8; // Number of points on the star

    glPushMatrix();
    glTranslatef(x, y, 0.0f); // Translate to sparkle position
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // Center of the star

    // Calculate vertices for the star
    for (int i = 0; i <= numPoints; ++i) {
        float angle = i * 2.0f * M_PI / numPoints - M_PI / 2.0f;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }

    glEnd();
    glPopMatrix();
}

//////////////////////////////////////// SCENE 1 ANIMATION //////////////////////////////////////////////
void displayIntro() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (!firstTextDisplayed) {
        drawBBStudio();
    }
    else {
        drawBitsBytes();
    }
    printf("Display Intro\n");
    glutSwapBuffers();
}
// Function to handle display of the monitor scene
void displayMonitorScene() {
    monitorOnDesk(0, 0, 0, 0);
    glutSwapBuffers();
}
void loadingString(int x, int y) {
    renderText(x + 40, y + 200, GLUT_BITMAP_TIMES_ROMAN_24, "Loading....", 255, 255, 255);
}
void loadingScreen(int x, int y, int w, int h, int colorR, int colorG, int colorB) {
    rectangle(x + 20, y + 20, w - 40, h - 40, colorR, colorG, colorB);
}
// Function to handle display of the loading screen
void displayLoadingScreen() {
    static int loadingStartTime = glutGet(GLUT_ELAPSED_TIME); // Record the start time when the function is first called
    int elapsedTime = glutGet(GLUT_ELAPSED_TIME) - loadingStartTime; // Calculate elapsed time since the function was called

    glClear(GL_COLOR_BUFFER_BIT);

    if (elapsedTime <= 2000) { // Display loading screen for the first 2 seconds
        loadingScreen(0, 0, 1000, 600, (loadingTime % 2 == 0) ? 0 : 42, (loadingTime % 2 == 0) ? 0 : 42, (loadingTime % 2 == 0) ? 0 : 42);
        loadingString(0, 0);
        //monitor(0, 0, 1000, 600);
        printf("Displaying loading screen\n");
    }
    else {
        printf("Display Monitor\n");
        monitor(0, 0, 1000, 600);
        currentState = BITS_AND_BYTES_SCREEN;
        glutSwapBuffers();
        return; // Exit the function to avoid further execution of code
    }
    glutSwapBuffers();
}
void updateLoadingTime(int value) {
    // Update loading time at the frame rate
    loadingTime++;
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, updateLoadingTime, 0);
}
// Function to handle display of the first scene
void displayScene1() {
    glClear(GL_COLOR_BUFFER_BIT);
    int monitorX = SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4 / 2; // X-coordinate of the monitor
    int monitorY = SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 4 / 2; // Y-coordinate of the monitor
    int monitorWidth = SCREEN_WIDTH / 4; // Width of the monitor
    int monitorHeight = SCREEN_HEIGHT / 4; // Height of the monitor



    zoomInMonitor(monitorX, monitorY, monitorWidth, monitorHeight);
    printf("Display Scene 1\n");
    glutSwapBuffers();
}
// Function to animate the first scene
void animateScene1(int value) {
    // Update scale factors
    scaleX += scaleSpeed;
    scaleY += scaleSpeed;

    // Stop scaling when the outer monitor size is equal to or greater than the screen size
    if (SCREEN_WIDTH / 4 * scaleX >= SCREEN_WIDTH && SCREEN_HEIGHT / 4 * scaleY >= SCREEN_HEIGHT) {
        scaleX = SCREEN_WIDTH / (float)(SCREEN_WIDTH / 4); // Ensure scaleX is exactly the screen width
        scaleY = SCREEN_HEIGHT / (float)(SCREEN_HEIGHT / 4); // Ensure scaleY is exactly the screen height
        glutPostRedisplay(); // Redraw to update the display
        currentState = TURN_ON_MONITOR;
        return; // Exit the function
    }

    // Redraw
    //glutPostRedisplay();

    // Call update function again after a delay
    glutTimerFunc(1000 / FPS, animateScene1, 0);
}
// Function to handle animation of the introduction scene
void animateIntro(int value) {

    static int frame = 0;

    frame++;
    if (frame == FPS * delaySecondsBB) {
        firstTextDisplayed = true;
    }
    if (frame == FPS * delaySecondsMonitor) {
        currentState = SCENE_1;
        startTime = glutGet(GLUT_ELAPSED_TIME);
        glutTimerFunc(delay, animateScene1, 0);
        return; // Exit the function after transitioning to the monitor scene
    }
    glutPostRedisplay();
    glutTimerFunc(delay, animateIntro, 0);
}
void displayCharacterMoves() {
    printf("Moving Bits and Bytes\n");
    glClear(GL_COLOR_BUFFER_BIT);
    monitor(0, 0, 1000, 600);

    // Initial coordinates for characters
    static double byteX = 350;
    static double byteY = 60;
    static double bitX = 650;
    static double bitY = 60;
    // New designated coordinates for characters
    double New_byteX = 300;
    double New_byteY = 60;
    double New_bitX = 700;
    double New_bitY = 60;

    static int transitionFrames = FPS * 3; // Adjust the duration of the transition
    static int idleFrames = FPS * 5;        // Duration for idle animation
    static int stopFrames = FPS * 10;        // Duration for stopping before next state
    static int frameCounter = 0;

    double idleOffset = 5 * sin(2 * PI * frameCounter / (FPS * 40)); // Idle animation offset

    if (frameCounter < idleFrames) {
        // Draw "Byte" with idle animation offset
        drawByte(byteX, byteY + idleOffset, 40, 150);
        // Draw "Bit" with idle animation offset
        drawBit(bitX, bitY + idleOffset, 40, 150);

        frameCounter++;
        glutSwapBuffers();
        glutPostRedisplay(); // Continue animation
    }
    else if (frameCounter < idleFrames + stopFrames) {
        // Draw "Byte" and "Bit" at their final idle position
        drawByte(byteX, New_byteY + idleOffset, 40, 150);
        drawBit(bitX, New_bitY + idleOffset, 40, 150);

        frameCounter++;
        glutSwapBuffers();
        glutPostRedisplay(); // Continue animation
    }
    else if (frameCounter < idleFrames + stopFrames + transitionFrames) {
        // Calculate the intermediate coordinates for the transition
        double byteXTransition = byteX + (New_byteX - byteX) * (double)(frameCounter - idleFrames - stopFrames) / transitionFrames;
        double byteYTransition = byteY + (New_byteY - byteY) * (double)(frameCounter - idleFrames - stopFrames) / transitionFrames + idleOffset;
        double bitXTransition = bitX + (New_bitX - bitX) * (double)(frameCounter - idleFrames - stopFrames) / transitionFrames;
        double bitYTransition = bitY + (New_bitY - bitY) * (double)(frameCounter - idleFrames - stopFrames) / transitionFrames + idleOffset;

        // Draw "Byte" with updated coordinates
        drawByte(byteXTransition, byteYTransition, 40, 150);
        // Draw "Bit" with updated coordinates
        drawBit(bitXTransition, bitYTransition, 40, 150);

        frameCounter++;
        glutSwapBuffers();
        glutPostRedisplay(); // Continue animation
    }
    else {
        // Reset frame counter and coordinates for next transition
        frameCounter = 0;
        byteX = New_byteX;
        byteY = New_byteY;
        bitX = New_bitX;
        bitY = New_bitY;

        currentState = IDE_WINDOW_POP_UP;
        glutPostRedisplay(); // Switch to the next state

    }
}

//////////////////////////////////////// SCENE 2 ANIMATION //////////////////////////////////////////////
void displayIDEWindowPopUp() {
    static enum { START, DELAY, DISPLAY_WINDOW, TRANSITION_DELAY } state = START;
    static int startTime = glutGet(GLUT_ELAPSED_TIME);
    static int elapsedTime = 0;

    glClear(GL_COLOR_BUFFER_BIT);
    monitor(0, 0, 1000, 600); // Background of the animation
    drawBit(700, 60, 40, 150);
    drawByte(300, 60, 40, 150);

    switch (state) {
    case START:
        printf("Starting part IDE Window pop up animation\n");
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);
        state = DELAY; // Transition to next state
        startTime = glutGet(GLUT_ELAPSED_TIME);
        break;

    case DELAY:
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime > 1000) { // Delay for 1 second
            state = DISPLAY_WINDOW;
            startTime = glutGet(GLUT_ELAPSED_TIME);
        }
        break;

    case DISPLAY_WINDOW:
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        printf("Display IDE Window pop up\n");
        if (elapsedTime < 1000) { // Display for 1 second
            IDE_window(0, 0, 0, 0);
        }
        else {
            state = TRANSITION_DELAY;
            startTime = glutGet(GLUT_ELAPSED_TIME);
        }
        break;

    case TRANSITION_DELAY:
        printf("Transition delay\n");
        IDE_window(0, 0, 0, 0);
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime > 2000) { // Delay for 2 seconds
            currentState = BIT_AND_BYTE_TYPING; // Update currentState
            startTime = glutGet(GLUT_ELAPSED_TIME);
            glutPostRedisplay(); // Trigger a redraw to switch to BIT_AND_BYTE_TYPING state
        }
        break;
    }

    glutSwapBuffers();
    glutPostRedisplay(); // Continue animation
}
void displayBitsAndBytesTyping() {
    static enum { STEP_INIT, STEP_BIT_HOP, STEP_BIT_STOP, STEP_CODE1, STEP_DELAY1, STEP_BYTE_HOP, STEP_BYTE_STOP, STEP_CODE2, STEP_BIT_HOP2, STEP_BIT_STOP2, STEP_CODE3, STEP_DELAY2, STEP_ALMOST_DONE, STEP_ALMOST_DONE_2, STEP_ALMOST_DONE_3, STEP_ALMOST_DONE_4, STEP_DONE } state = STEP_INIT;
    static int startTime = glutGet(GLUT_ELAPSED_TIME);
    static int elapsedTime = 0;

    glClear(GL_COLOR_BUFFER_BIT);
    monitor(0, 0, 1000, 600); // Background of the animation
    IDE_window(0, 0, 0, 0);   // Also the background of the animation

    switch (state) {
    case STEP_INIT:
        printf("Starting Bits and Bytes Typing Animation\n");
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);
        state = STEP_BIT_HOP;
        startTime = glutGet(GLUT_ELAPSED_TIME);
        break;

    case STEP_BIT_HOP:
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        printf("case STEP_BIT_HOP started. Elapsed Time : % d\n", elapsedTime);
        if (elapsedTime < 1000) { // Hop for 1 second
            drawBit(700, 60 + (40 * sin(elapsedTime * PI / 1000.0)), 40, 150);
            drawByte(300, 60, 40, 150);
            //printf(("Frame" + std::to_string(elapsedTime)).c_str());
        }
        else {
            state = STEP_BIT_STOP;
            startTime = glutGet(GLUT_ELAPSED_TIME);
        }
        break;

    case STEP_BIT_STOP:
        printf("case STEP_BIT_STOP started. Elapsed Time : % d\n", elapsedTime);
        drawBit(700, 60, 40, 150); // Stop hopping
        drawByte(300, 60, 40, 150);
        state = STEP_CODE1;
        break;

    case STEP_CODE1:
        code1();
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);
        printf("case STEP_CODE1 started. Elapsed Time : % d\n", elapsedTime);
        state = STEP_DELAY1;
        startTime = glutGet(GLUT_ELAPSED_TIME);
        break;

    case STEP_DELAY1:
        printf("case STEP_DELAY1 started\n");
        code1();
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime > 2000) { // Delay for 2 seconds
            state = STEP_BYTE_HOP;
            startTime = glutGet(GLUT_ELAPSED_TIME);
        }
        break;

    case STEP_BYTE_HOP:
        printf("case STEP_BYTE_HOP started\n");
        code1();

        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime < 1000) { // Hop for 1 second
            drawByte(300, 60 + (40 * sin(elapsedTime * PI / 1000.0)), 40, 150);
            drawBit(700, 60, 40, 150);
        }
        else {
            state = STEP_BYTE_STOP;
            startTime = glutGet(GLUT_ELAPSED_TIME);
        }
        break;

    case STEP_BYTE_STOP:
        printf("case STEP_BYTE_STOP started\n");

        code1();
        drawByte(300, 60, 40, 150); // Stop hopping
        drawBit(700, 60, 40, 150);
        state = STEP_CODE2;
        break;

    case STEP_CODE2:
        printf("case STEP_CODE2 started\n");

        code1();
        code2();
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);
        state = STEP_BIT_HOP2;
        startTime = glutGet(GLUT_ELAPSED_TIME);
        break;

    case STEP_BIT_HOP2:
        printf("case STEP_BIT_HOP2 started\n");

        code1();
        code2();
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime < 1000) { // Hop for 1 second
            drawBit(700, 60 + (40 * sin(elapsedTime * PI / 1000.0)), 40, 150);
            drawByte(300, 60, 40, 150);
        }
        else {
            state = STEP_BIT_STOP2;
            startTime = glutGet(GLUT_ELAPSED_TIME);
        }
        break;

    case STEP_BIT_STOP2:
        printf("case STEP_BIT_STOP2 started\n");

        code1();
        code2();
        drawBit(700, 60, 40, 150); // Stop hopping
        drawByte(300, 60, 40, 150);
        state = STEP_CODE3;
        break;

    case STEP_CODE3:
        printf("case STEP_CODE3 started\n");

        code1();
        code2();
        code3();
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);
        state = STEP_DELAY2;
        startTime = glutGet(GLUT_ELAPSED_TIME);
        break;

    case STEP_DELAY2:
        printf("case STEP_DELAY2 started\n");

        code1();
        code2();
        code3();
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);

        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime > 2000) { // Delay for 2 seconds
            state = STEP_ALMOST_DONE;
        }
        break;

    case STEP_ALMOST_DONE:
        printf("case STEP_ALMOST_DONE started\n");

        code1();
        code2();
        code3();
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);

        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime > 2000) { // Delay for 2 seconds
            state = STEP_ALMOST_DONE_2;
        }

        break;

    case STEP_ALMOST_DONE_2:
        printf("case STEP_ALMOST_DONE_2 started\n");

        monitor(0, 0, 1000, 600);
        //screen
        //desktop bcg color - BLACK
        rectangle(20, 20, 1000 - 40, 600 - 40, 0, 0, 0);

        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime > 3000) { // Delay 
            state = STEP_ALMOST_DONE_3;
        }

        break;

    case STEP_ALMOST_DONE_3:
        printf("case STEP_ALMOST_DONE_3 started\n");

        monitor(0, 0, 1000, 600);
        //screen
        //desktop bcg color - WHITE
        rectangle(20, 20, 1000 - 40, 600 - 40, 225, 225, 225);

        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime > 4000) { // Delay 
            state = STEP_ALMOST_DONE_4;
        }

        break;

    case STEP_ALMOST_DONE_4:
        printf("case STEP_ALMOST_DONE_4 started\n");

        displayERROR();


        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime > 5000) { // Delay 
            state = STEP_DONE;
        }

        break;

    case STEP_DONE:
        printf("case STEP_DONE started\n");

        darkMonitor(0, 0, 1000, 600);
        drawBit(700, 60, 40, 150);
        drawByte(300, 60, 40, 150);

        // Animation completed
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
        if (elapsedTime > 2000) { // Delay for 2 seconds
            //currentState = DARK_BACKGROUND_SCENE; // Update currentState
            startTime = glutGet(GLUT_ELAPSED_TIME);
            glutPostRedisplay(); // Trigger a redraw to switch to BIT_AND_BYTE_TYPING state
        }

        break;
    }

    glutSwapBuffers();
    glutPostRedisplay(); // Continue animation
}

//////////////////////////////////////// SCENE 3 ANIMATION //////////////////////////////////////////////
void calculateIncrementalMovements() {
    // Calculate incremental movements for each frame
    byte_dx = (target_byte_x - byte_x) / num_frames;
    byte_dy = (target_byte_y - byte_y) / num_frames;
    bit_dx = (target_bit_x - bit_x) / num_frames;
    bit_dy = (target_bit_y - bit_y) / num_frames;
    virus_dx = (target_virus_x - virus_x) / num_frames; // Calculate incremental movement for the virus
    virus_dy = (target_virus_y - virus_y) / num_frames; // Calculate incremental movement for the virus
}
void moveCharacters(int value) {
    // Check if the first second has passed
    static int elapsed_time = 0;
    if (elapsed_time < 1000) { // 1000 milliseconds = 1 second
        // Random movement within the first second
        byte_x += (rand() % 3) - 1; // Random movement in the x direction (-1, 0, 1)
        byte_y += (rand() % 3) - 1; // Random movement in the y direction (-1, 0, 1)
        bit_x += (rand() % 3) - 1; // Random movement in the x direction (-1, 0, 1)
        bit_y += (rand() % 3) - 1; // Random movement in the y direction (-1, 0, 1)
        // Gradually bring the virus into the screen
        if (virus_y < SCREEN_HEIGHT) {
            virus_y += 1; // Move the virus down by 1 unit
        }
    }
    else {
        // Update positions incrementally after the first second
        byte_x += byte_dx;
        byte_y += byte_dy;
        bit_x += bit_dx;
        bit_y += bit_dy;
        virus_x += virus_dx; // Update virus position incrementally
        virus_y += virus_dy; // Update virus position incrementally

        // Check if characters have reached their target positions
        if (fabs(byte_x - target_byte_x) < fabs(byte_dx) && fabs(byte_y - target_byte_y) < fabs(byte_dy) &&
            fabs(bit_x - target_bit_x) < fabs(bit_dx) && fabs(bit_y - target_bit_y) < fabs(bit_dy)) {
            // Stop incremental movements
            byte_dx = 0;
            byte_dy = 0;
            bit_dx = 0;
            bit_dy = 0;
            virus_dx = 0; // Stop virus movement
            virus_dy = 0; // Stop virus movement
        }
    }

    // Redraw
    glutPostRedisplay();

    // Increment elapsed time
    elapsed_time += 1000 / num_frames;
    total_elapsed_time += 1000 / num_frames; // Increment total elapsed time

    // Check if movement transition is complete
    if (elapsed_time < transition_duration) {
        // Continue transition
        glutTimerFunc(transition_duration / num_frames, moveCharacters, 0);
    }
    else {
        // Transition complete, reset incremental movements
        calculateIncrementalMovements();
    }
}
void moveVirusToTopLeft(int value) {
    // Set target position to the top left corner
    target_virus_x = 100;
    target_virus_y = 500;

    // Calculate incremental movements
    virus_dx = (target_virus_x - virus_x) / num_frames;
    virus_dy = (target_virus_y - virus_y) / num_frames;

    // Start the movement transition for the virus
    moveCharacters(0);
}
void moveVirusToTopRight(int value) {
    // Set target position to the top right corner
    target_virus_x = 650;
    target_virus_y = 450;

    // Calculate incremental movements
    virus_dx = (target_virus_x - virus_x) / num_frames;
    virus_dy = (target_virus_y - virus_y) / num_frames;

    // Start the movement transition for the virus
    moveCharacters(0);
}
void moveCharactersToCorners(int value) {
    // Set target positions to corners
    target_bit_x = 70;
    target_bit_y = 450;
    target_byte_x = 650;
    target_byte_y = 450;

    // Calculate incremental movements
    calculateIncrementalMovements();

    // Start the movement transition
    moveCharacters(0);
}
void moveVirusToBottomRight(int value) {
    // Set target position to the bottom right corner
    target_virus_x = 800;
    target_virus_y = 150;

    // Calculate incremental movements
    virus_dx = (target_virus_x - virus_x) / num_frames;
    virus_dy = (target_virus_y - virus_y) / num_frames;

    // Start the movement transition for the virus
    moveCharacters(0);
}
void moveCharactersToBottom(int value) {
    // Set target positions to the bottom of the screen
    target_bit_x = 200; // Maintain current x position
    target_bit_y = 50;    // Move Bit to the bottom
    target_byte_x = 50; // Maintain current x position
    target_byte_y = 50;    // Move Byte to the bottom

    // Calculate incremental movements
    calculateIncrementalMovements();

    // Start the movement transition
    moveCharacters(0);

    // After reaching the bottom, stop at specific positions
    target_bit_x = 200;
    target_bit_y = 50;
    target_byte_x = 50;
    target_byte_y = 50;

    glutPostRedisplay();

}
void startScene3() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);


    glClear(GL_COLOR_BUFFER_BIT); // display background colour
    glPointSize(10.0);
    glLineWidth(10.0);
    darkMonitor(0, 0, 1000, 600);

    drawByte(byte_x, byte_y, 40, 150);  // Draw the character "Byte"
    drawBit(bit_x, bit_y, 40, 150);  // Draw the character "Bit"
    drawVirus(virus_x, virus_y);
    // Start the timer to move the virus to the top left corner
    glutTimerFunc(0, moveVirusToTopLeft, 0);

    // Start the timer for continuous random movement
    glutTimerFunc(3000, moveCharacters, 0);

    // Start the timer to move the virus to the top right corner after 7 seconds
    glutTimerFunc(7000, moveVirusToTopRight, 0);

    // Start the timer to move characters to corners after 7 seconds
    glutTimerFunc(7000, moveCharactersToCorners, 0);

    // Start the timer to move the virus to the bottom right corner after 10 seconds
    glutTimerFunc(10000, moveVirusToBottomRight, 0);

    // Start the timer to move characters to the bottom after 10 seconds
    glutTimerFunc(10000, moveCharactersToBottom, 0);

    glutMainLoop();
}

//////////////////////////////////////// SCENE 4 ANIMATION //////////////////////////////////////////////
// Function for Bit's jumping animation
void bit_byte(int frame) {
    glutTimerFunc(0, bit_byte, 0);

    int bitJumpDistanceY = 100; // Example jump distance in Y-axis
    int bitJumpDuration = 90; // Duration of the jump animation
    int bitInitialY = 50; // Initial Y position of Bit
    int bitFinalY = bitInitialY + bitJumpDistanceY; // Final Y position of Bit after jump
    int BitY = bitInitialY + ((bitFinalY - bitInitialY) * frameCount) / bitJumpDuration;

    // Bit moves a little bit in the x-axis
    int bitMoveDistanceX = 20; // Example movement distance in X-axis
    int bitInitialX = 200; // Initial X position of Bit
    int BitX = bitInitialX + ((bitMoveDistanceX * frameCount) / bitJumpDuration);

    drawBit(BitX, BitY, 40, 150);

    int gunPositionX = BitX + 30; // Example gun position
    int gunPositionY = BitY + 70; // Example gun position
    drawGun(gunPositionX, gunPositionY, gunAlpha); // Draw the gun

    drawByte(50, 50, 40, 150);
    drawSniperRifle(120, 130, 150, 40, rifleAlpha);

    glutTimerFunc(12000, bit_byte, 0);
}
// Function for firing bullets
void fireBullets(int frame) {
    glutTimerFunc(0, fireBullets, 0);

    if (shotsFired == 0) {
        drawBullet(290, 135, 850, 150);
    }
    if (shotsFired < 3) {
        drawBullet(290, 235, 850, 150);
    }
    shotsFired++;

    glutTimerFunc(12000, fireBullets, 0);
}
// Function for glitch effect
void glitchEffect(int frame) {
    glutTimerFunc(0, glitchEffect, 0);
    int centerX = 800; // X-coordinate of the center point
    int centerY = 150; // Y-coordinate of the center point
    float orbitRadius = 15; // Radius of the orbit

    float angleSquare = frameCount * 0.1; // Angle of rotation for square
    float angleTriangle = frameCount * 0.15; // Angle of rotation for triangle
    float angleCircle = frameCount * 0.2; // Angle of rotation for circle

    // Calculate the positions of the shapes based on the orbits
    int xSquare = centerX + orbitRadius * cos(angleSquare);
    int ySquare = centerY + orbitRadius * sin(angleSquare);

    int xTriangle = centerX + orbitRadius * cos(angleTriangle);
    int yTriangle = centerY + orbitRadius * sin(angleTriangle);

    int xCircle = centerX + orbitRadius * cos(angleCircle);
    int yCircle = centerY + orbitRadius * sin(angleCircle);

    float scaleFactor = sin(frameCount * 0.1); // Scaling factor based on sine function for pulsating effect
    float rotationAngle = frameCount * 2; // Rotation angle for spinning effect

    // Draw square
    glColor3ub(255, 0, 0); // Red color
    glPushMatrix();
    glTranslatef(xSquare, ySquare, 0); // Translate to the desired position
    glRotatef(rotationAngle, 0, 0, 1); // Rotate around z-axis
    glScalef(scaleFactor, scaleFactor, 1); // Scale the square
    glBegin(GL_QUADS);
    glVertex2i(-30, -30);
    glVertex2i(30, -30);
    glVertex2i(30, 30);
    glVertex2i(-30, 30);
    glEnd();
    glPopMatrix();

    // Draw triangle
    glColor3ub(0, 255, 0); // Green color
    glPushMatrix();
    glTranslatef(xTriangle, yTriangle, 0); // Translate to the desired position
    glRotatef(rotationAngle, 0, 0, 1); // Rotate around z-axis
    glScalef(scaleFactor, scaleFactor, 1); // Scale the triangle
    glBegin(GL_TRIANGLES);
    glVertex2i(0, 40);
    glVertex2i(-30, -10);
    glVertex2i(30, -10);
    glEnd();
    glPopMatrix();

    // Draw circle
    glColor3ub(0, 0, 255); // Blue color
    glPushMatrix();
    glTranslatef(xCircle, yCircle, 0); // Translate to the desired position
    glRotatef(rotationAngle, 0, 0, 1); // Rotate around z-axis
    glScalef(scaleFactor, scaleFactor, 1); // Scale the circle
    drawCircle(0, 0, 30); // Custom function to draw a circle
    glPopMatrix();

    glutTimerFunc(3000, glitchEffect, 0);
}
// Function for explosion effect
void explosionEffect(int frame) {
    glutTimerFunc(0, explosionEffect, 0);
    drawExplosion(90);
    glutTimerFunc(1000, explosionEffect, 0);
}
// Function for final black screen
void finalBlackScreen(int frame) {
    glColor3f(0.0, 0.0, 0.0); // Black color 
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(SCREEN_WIDTH, 0);
    glVertex2i(SCREEN_WIDTH, SCREEN_HEIGHT);
    glVertex2i(0, SCREEN_HEIGHT);
    glEnd();

    glutTimerFunc(3000, finalBlackScreen, 0);
    allAnimationsFinished = true;
}
void scene4(int frame) {
    // Increment frame count
    frameCount++;

    // Determine which segment of the animation to execute
    int segmentDuration = FPS * 3; // Duration of each segment (3 seconds)
    int segment = frameCount / segmentDuration;

    switch (segment) {
    case 0:
        darkMonitor(0, 0, 1000, 600);
        bit_byte(frame);
        drawVirus(800, 150);
        break;
    case 1:
        darkMonitor(0, 0, 1000, 600);
        drawByte(50, 50, 40, 150);
        drawBit(220, 150, 40, 150);
        drawGun(250, 220, 255);
        drawSniperRifle(120, 130, 150, 40, 255);
        drawVirus(800, 150);
        fireBullets(frame);
        break;
    case 2:
        darkMonitor(0, 0, 1000, 600);
        drawByte(50, 50, 40, 150);
        drawBit(220, 150, 40, 150);
        drawGun(250, 220, 255);
        drawSniperRifle(120, 130, 150, 40, 255);
        glitchEffect(frame);
        break;
    case 3:
        darkMonitor(0, 0, 1000, 600);
        drawByte(50, 50, 40, 150);
        drawBit(220, 150, 40, 150);
        drawGun(250, 220, 255);
        drawSniperRifle(120, 130, 150, 40, 255);
        explosionEffect(frame);
        break;
    case 4:
        darkMonitor(0, 0, 1000, 600);
        finalBlackScreen(frame);
        break;
    default:
        allAnimationsFinished = true;
        break;
    }

    // Check if all animations are finished
    if (!allAnimationsFinished) {
        // Request redisplay
        glutPostRedisplay();
    }
}

//////////////////////////////////////// SCENE 5 ANIMATION //////////////////////////////////////////////
void initSparkles() {
    // Initialize sparkles
    for (int i = 0; i < SPARKLE_COUNT; ++i) {
        // Generate a new sparkle position
        float newX, newY;
        bool validPosition = false;

        while (!validPosition) {
            // Randomize position within screen boundaries
            newX = rand() % SCREEN_WIDTH;
            newY = rand() % SCREEN_HEIGHT;

            // Check distance to all previously placed sparkles
            validPosition = true;
            for (int j = 0; j < i; ++j) {
                float dx = newX - sparkles[j].x;
                float dy = newY - sparkles[j].y;
                float distance = sqrt(dx * dx + dy * dy);

                // If too close to another sparkle, regenerate position
                if (distance < MIN_SPARKLE_DISTANCE) {
                    validPosition = false;
                    break;
                }
            }
        }

        sparkles[i].x = newX;
        sparkles[i].y = newY;
        sparkles[i].size = 1.0;
        sparkles[i].maxSize = 30.0 + (rand() % 20);
        sparkles[i].growing = true;
    }
}
void animateSparkles() {
    // Animate sparkles
    for (int i = 0; i < SPARKLE_COUNT; ++i) {
        if (sparkles[i].growing) {
            sparkles[i].size += 0.7;
            if (sparkles[i].size >= sparkles[i].maxSize) {
                sparkles[i].growing = false;
            }
        }
        else {
            sparkles[i].size -= 0.5;
            if (sparkles[i].size <= 5.0) {
                sparkles[i].growing = true;
            }
        }
    }
}
void animateCurtain() {
    if (curtainClosing) {
        leftCurtainPosition += curtainSpeed;
        rightCurtainPosition -= curtainSpeed;

        if (leftCurtainPosition >= SCREEN_WIDTH / 2 && rightCurtainPosition <= SCREEN_WIDTH / 2) {
            curtainClosing = false;
        }
    }
}
void animateCharacters(int value) {
    static bool delayComplete = false;

    // Check if delay is complete
    if (!delayComplete) {
        // Schedule a callback to mark delay complete
        glutTimerFunc(1000, [](int) {
            delayComplete = true;
            }, 0);
    }

    if (delayComplete) {
        bitStopped = (bitX >= SCREEN_WIDTH / 2 - 200.0);
        byteStopped = (byteX <= SCREEN_WIDTH / 2 + 130.0);

        if (!bitStopped) {
            bitX += BIT_SPEED;
        }

        if (!byteStopped) {
            byteX -= BYTE_SPEED;
        }

        // Toggle eye movement for Bit if not stopped
        const float bitEyeSpeed = 0.25; // Adjust this value for eye movement
        if (!bitStopped) {
            if (bitLookRight) {
                bitEyeOffset += bitEyeSpeed;
                if (bitEyeOffset >= 10.0) {
                    bitLookRight = false;
                }
            }
            else {
                bitEyeOffset -= bitEyeSpeed;
                if (bitEyeOffset <= -10.0) {
                    bitLookRight = true;
                }
            }
        }

        // Toggle eye movement for Byte if not stopped
        const float byteEyeSpeed = 0.25; // Adjust this value for eye movement
        if (!byteStopped) {
            if (byteLookRight) {
                byteEyeOffset += byteEyeSpeed;
                if (byteEyeOffset >= 10.0) {
                    byteLookRight = false;
                }
            }
            else {
                byteEyeOffset -= byteEyeSpeed;
                if (byteEyeOffset <= -10.0) {
                    byteLookRight = true;
                }
            }
        }

        // Check if both characters have stopped
        if (bitStopped && byteStopped) {
            // Set happy eyes after a delay
            if (!bitHappy && !byteHappy) {
                glutTimerFunc(EYE_CHANGE_DELAY, [](int) {
                    bitHappy = true;
                    byteHappy = true;
                    // Start jumping animation
                    jumpingUp = true;
                    }, 0);
            }
        }

        // Handle jumping animation
        if (jumpingUp) {
            bitY += JUMP_SPEED;
            byteY += JUMP_SPEED;

            // Check if characters reached the peak of the jump
            if (bitY >= 50.0 + jumpHeight) {
                jumpingUp = false;
                jumpingDown = true;
            }
        }
        else if (jumpingDown) {
            bitY -= JUMP_SPEED;
            byteY -= JUMP_SPEED;

            // Check if characters returned to the ground
            if (bitY <= 50.0) {
                jumpingDown = false;
                jumpingUp = true; // Restart jumping animation
            }
        }

        animateSparkles(); // Animate the sparkles
    }

    animateCurtain(); // Animate the curtain descent

    glutPostRedisplay();

    // Update at ~60 FPS
    glutTimerFunc(16, animateCharacters, 0);
}

//////////////////////////////////////// INITGL & DISPLAY FUNCTION //////////////////////////////////////
void initGL() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set background color to black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT); // Set up orthographic projection
}
// Main display function
void display() {
    switch (currentState) {
    case INTRO:
        displayIntro();
        break;
    case SCENE_1:
        displayScene1();
        break;
    case TURN_ON_MONITOR:
        displayLoadingScreen();
        break;
    case BITS_AND_BYTES_SCREEN:
        displayBitsAndBytes();
        break;
    case CHARACTERS_MOVES:
        displayCharacterMoves();
        break;
    case IDE_WINDOW_POP_UP:
        displayIDEWindowPopUp();
        break;
    case BIT_AND_BYTE_TYPING:
        displayBitsAndBytesTyping();
        break;
    }
    startScene3();
    scene4(0);
}
//////////////////////////////////////// MAIN FUNCTION //////////////////////////////////////////////////
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT); // Set window size
    glutInitWindowPosition(100, 100); // Set window position
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Test");

    initGL();

    // Set the initial display function and start animation
    glutDisplayFunc(display);
    startTime = glutGet(GLUT_ELAPSED_TIME); // Record start time for accurate timing
    glutTimerFunc(0, updateLoadingTime, 0);
    glutTimerFunc(delay, animateIntro, 0);


    glutMainLoop();
    return 0;
}
