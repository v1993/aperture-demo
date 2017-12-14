#ifndef POINT_H
#define POINT_H

#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

typedef struct Point {long double x, y;} Point;

Point sumPoints(Point p1, Point p2);
void sumPointsArray(Point *p, int len, Point p2);
void swapPoint(Point &p);
void swapPointArray(Point *p, int len);
void PointDrawPixel(U8G2 u8g2 , Point p1);
void PointDrawPixelArray(U8G2 u8g2, Point *p, int len);
void PointDrawLine(U8G2, Point, Point);
void PointDrawLineArray(U8G2 u8g2, Point *p, int len);
#endif
