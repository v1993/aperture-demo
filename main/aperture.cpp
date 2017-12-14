#include "aperture.h"
#include "point.h"

// CUBES

struct Point calcHeartPoint(long double t) {
  struct Point p;

  p.x = (pow(sin(t), 3.0)); // [-1;1]
  p.y = ((13.0*cos(t)-5.0*cos(2.0*t)-2.0*cos(3.0*t)-cos(4.0*t) + 2.5) / 14.5 * (-1)); // [1;-1]

  return p;
}

void drawHeart(U8G2 u8g2, long double funcstep, long double mulp, int Xoff, int Yoff) {
  for (long double t = 0; t <= PI*2; t += funcstep) {
    struct Point p = calcHeartPoint(t);
    u8g2.drawPixel(p.x * mulp + Xoff, p.y * mulp + Yoff);
  }
}

void drawCubeSide(U8G2 u8g2, long double side, int basicX, int basicY, bool vert, bool inv) {
//  Serial.println("Emmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
  long double inOff = (side / 12);

  Point p[6];
  p[0] = {0, 0};
  p[1] = {side / 3.0, 0};
  if (!inv) {
    p[2] = {(side / 3.0) + inOff, inOff};
    p[3] = {(side / 3.0 * 2.0) - inOff, inOff};
  } else {
    p[2] = {(side / 3.0) + inOff, -inOff};
    p[3] = {(side / 3.0 * 2.0) - inOff, -inOff};
  }
  p[4] = {(side / 3.0 * 2.0), 0};
  p[5] = {side, 0};
  if (vert) swapPointArray(p, 5);
  sumPointsArray(p, 5, {(long double)basicX, (long double)basicY});
  PointDrawLineArray(u8g2, p, 5);

  // Draw extra line
  if (!inv) {
    p[0] = {side / 2.0, inOff};
    p[1] = {side / 2.0, side / 8.0 * 2.0};
  } else {
    p[0] = {side / 2.0, -inOff};
    p[1] = {side / 2.0, -(side / 8.0 * 2.0)};
  }
  if (vert) swapPointArray(p, 1);
  sumPointsArray(p, 1, {(long double)basicX, (long double)basicY});
  PointDrawLineArray(u8g2, p, 1);
}

void drawBasicCube(U8G2 u8g2, long double side, int Xoff, int Yoff) {
  int basicX = Xoff - (side / 2);
  int basicY = Yoff - (side / 2); 

  // Draw sides

  drawCubeSide(u8g2, side, basicX, basicY, false, false); // Top
  drawCubeSide(u8g2, side, basicX, basicY+side, false, true); // Bottom
  drawCubeSide(u8g2, side, basicX, basicY, true, false); // Left
  drawCubeSide(u8g2, side, basicX+side, basicY, true, true); // Left

  // Draw centre
  u8g2.drawCircle(Xoff, Yoff, side / 4.0);

  // Draw extra lines
  //u8g2.drawHLine(basicX, Yoff, side / 8.0 * 3.0);
  //u8g2.drawHLine(Xoff+side / 8.0, Yoff, side / 8.0 * 3.0);
}

void drawStorageCube(U8G2 u8g2, long double side, int Xoff, int Yoff, float mAngle) {
  drawBasicCube(u8g2, side, Xoff, Yoff);
  drawLogo(u8g2, side / 2.0 / 3.5, Xoff, Yoff, mAngle);
}

void drawCompanionCube(U8G2 u8g2, long double side, int Xoff, int Yoff) {
  drawBasicCube(u8g2, side, Xoff, Yoff);
  drawHeart(u8g2, 0.01, side / 2.0 / 3.5, Xoff, Yoff+1);
}

void drawRedirectionCube(U8G2 u8g2, long double side, int Xoff, int Yoff) {
  drawBasicCube(u8g2, side, Xoff, Yoff);
  u8g2.drawCircle(Xoff, Yoff, side / 4.5);
}

// BUTTON

void drawButton(U8G2 u8g2, long double side, int Xoff, int Yoff, int extraY) {
  u8g2.drawBox((long double)Xoff-(side/2.0), Yoff, side, side/6.0);
  if (extraY > 0) {
    u8g2.drawBox((long double)Xoff-(side/8.0*3.0), Yoff-extraY, side/4.0*3.0, extraY);
  }
}

// DOOR

void drawDoor(U8G2 u8g2, long double rad, int Xoff, int Yoff, long double prc) {
  u8g2.drawCircle(Xoff, Yoff, rad);

  //rad = rad * 0.9;

  int rx = round(prc / 100.0 * rad);
  int ry = round(sqrt(pow(rad, 2) - pow((100 - prc)/100.0*rad, 2)));

  if (ry <= 0) ry = 1;
  Serial.println(rx);
  Serial.println(ry);
  u8g2.drawFilledEllipse(Xoff - rad + rx, Yoff, rx, ry, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_LOWER_LEFT);
  u8g2.drawFilledEllipse(Xoff + rad - rx, Yoff, rx, ry, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_LOWER_RIGHT);
}

// CHECKBOX

void drawCheckbox(U8G2 u8g2, int Xoff, int Yoff, bool ok) {
  const int h = 16;
  const int w = 16;
  const int d = 2;

  uint16_t encoding;
  if (ok) {
    encoding = 0x2714;
  } else {
    encoding = 0x2715;
  }
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawGlyph(Xoff, Yoff + h, encoding);
  u8g2.drawFrame(Xoff - d, Yoff - d + 2, w + d * 2, h + d * 2);
}

// LOGO

float drawArc(U8G2 u8g2, float centerX, float centerY, float rad, 
                                                     float startAngle, float endAngle,
                                                     float x0, float y0);

struct Point drawLogo(U8G2 u8g2, float RAD, float CENTER_X, float CENTER_Y, float mAngle, int startFrom, float offset){
  //Inner radius.
  float r0 = 0.5 * RAD;

  //Outer radius.
  float r1 = RAD;

  //Octagon.
  int NUM_VERTEX = 8;

  //Lateral angle for each shape.
  float dAngle = 2.0 * PI / (float)NUM_VERTEX;

  // Text point.
  struct Point resPoint;

  for (int i=startFrom; i<NUM_VERTEX; i++){ 
    //Add an inner u8g2.drawPixel.
    float t0 = (i + 1) * dAngle - offset;
    float x0 = CENTER_X + r0 * cos(t0);
    resPoint.x = x0;
    float y0 = CENTER_Y + r0 * sin(t0);
    u8g2.drawPixel(x0, y0);

    //Add vertices for an arc.
    float t1 = (i - 0.5) * dAngle + mAngle - offset;
    float t2 = (i + 0.5) * dAngle - mAngle - offset;
    float ArcY = drawArc(u8g2, CENTER_X, CENTER_Y, r1, t1, t2, x0, y0);
    if (i == startFrom) resPoint.y = ArcY;
  }
  return resPoint;
}
 
// Draw an arc and lines
float drawArc(U8G2 u8g2, float centerX, float centerY, float rad, 
                                                     float startAngle, float endAngle,
                                                     float x0, float y0){
  float firstY;
  int NUM_VERTEX = 100;
  float dt = (endAngle - startAngle) / (float)NUM_VERTEX;
 
  for (int i=0; i<NUM_VERTEX; i++){
    float x = centerX + rad * cos(startAngle + i * dt); 
    float y = centerY + rad * sin(startAngle + i * dt);
    if (i == 0) firstY = y;
 
    //Add lines
    u8g2.drawPixel(x, y);
    u8g2.drawLine(x, y, x0, y0);
  }
  return firstY;
}

void drawBigLogo(U8G2 u8g2, float X, float Y) {
  struct Point p = drawLogo(u8g2, 15, X+15, Y+15, 6.0 * PI / 180.0, 1);
  u8g2.setFont(u8g2_font_t0_15b_tf);
  u8g2.drawStr(p.x + 5, p.y - 2, "APERTURE");
  u8g2.setFont(u8g2_font_t0_11b_tf);
  u8g2.drawStr(p.x + 10, p.y + 9, "LABORATORIES");
}

