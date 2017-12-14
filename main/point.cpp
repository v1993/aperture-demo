#include "point.h"

Point sumPoints(Point p1, Point p2) {
  Point p3 = {p1.x + p2.x, p1.y + p2.y};
  return p3;
}

void sumPointsArray(Point *p, int len, Point p2) {
  for(int i=0; i <= len; i++) {
    p[i] = sumPoints(p[i], p2);
  }
}

void swapPoint(Point &p) {
//  Serial.print("SWAP BEGIN");
  Point p1 = {p.y, p.x};
  p.x = p1.x;
  p.y = p1.y;
//  Serial.print("SWAP OK");
}

void swapPointArray(Point *p, int len) {
  for(int i=0; i <= len; i++) {
    swapPoint(p[i]);
  }
}

void PointDrawPixel(U8G2 u8g2, Point p1) {
  u8g2.drawPixel(round(p1.x), round(p1.y));
}

void PointDrawPixelArray(U8G2 u8g2, Point *p, int len) {
  for(int i=0; i <= len; i++) {
    PointDrawPixel(u8g2, p[i]);
  }
}

void PointDrawLine(U8G2 u8g2, Point p1, Point p2) {
  u8g2.drawLine(round(p1.x), round(p1.y), round(p2.x), round(p2.y));
//  Serial.println("LINE OK");
//  Serial.println((double)p1.x);
//  Serial.println((double)p1.y);
//  Serial.println((double)p2.x);
//  Serial.println((double)p2.y);
}

void PointDrawLineArray(U8G2 u8g2, Point *p, int len) {
//  Serial.println(len);
  for(int i=0; i < len; i++) {
//    Serial.print("Drawing new line #");
//    Serial.println(i);
    PointDrawLine(u8g2, p[i], p[i+1]);
  }
}
