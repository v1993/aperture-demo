#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include "point.h"

// CUBES
void drawCubeSide(U8G2 u8g2, long double side, int basicX, int basicY, bool vert, bool inv);
void drawBasicCube(U8G2 u8g2, long double side, int Xoff, int Yoff);
void drawStorageCube(U8G2 u8g2, long double side, int Xoff, int Yoff, float mAngle = 18.0 / 180. * PI);
void drawCompanionCube(U8G2 u8g2, long double side, int Xoff, int Yoff);
void drawRedirectionCube(U8G2 u8g2, long double side, int Xoff, int Yoff);

//BUTTON
void drawButton(U8G2 u8g2, long double side, int Xoff, int Yoff, int extraY);

// DOOR
void drawDoor(U8G2 u8g2, long double rad, int Xoff, int Yoff, long double prc);

// CHECKBOX
void drawCheckbox(U8G2 u8g2, int Xoff, int Yoff, bool ok);

// LOGO
struct Point drawLogo(U8G2 u8g2, float RAD, float CENTER_X, float CENTER_Y, float mAngle = 6.0 * PI / 180.0, int startFrom = 0, float offset = 15.0 * PI / 180.0);
void drawBigLogo(U8G2 u8g2, float CENTER_X, float CENTER_Y);
