#include <U8g2lib.h>

#include "aperture.h"

#define msgfont u8g2_font_t0_15b_tf
#define msgfontH 12

#define H 64
#define W 128

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 23);

const int msgOff = 8;
const int btnY = 50;
const int btnW = 64;
const int btnH = 5;
const int cubeS = 36;

int calMaxStrLen(String str) {
  //int i = min(str.length(), 31); // Use for 8bit mode
  int i = str.length();
  while(1) {
    String newStr = str.substring(0, i);
    char charBuf[i+1];
    newStr.toCharArray(charBuf, i+1);
    if (u8g2.getUTF8Width(charBuf) <= W) {
      //Serial.println(newStr);
      return i;
    }
    i--;
  }
}

void centrePrint(String str, int Y) {
  char charBuf[str.length()+1];
  str.toCharArray(charBuf, str.length()+1);
  u8g2.drawUTF8((W-u8g2.getUTF8Width(charBuf))/2, Y, charBuf);
}

void showMsgStep(String str, int i, int Y, bool internal = false);

void showMsgStep(String str, int i, int Y, bool internal) {
  if (!internal)
    u8g2.clearBuffer();
  int showingLen = min(i, calMaxStrLen(str));
  centrePrint(str.substring(0, showingLen), Y);
  if (showingLen < i) {
    String newStr = str.substring(showingLen, i);
    showMsgStep(newStr, i - showingLen, Y + msgfontH, true);
  }
  if (!internal)
    u8g2.sendBuffer();
}

void showMsg(String str, long int del, int Y) {
  u8g2.setFont(msgfont);
  for (long int i = 0; i <= str.length(); i++) {
    showMsgStep(str, i, Y);
    delay(del);
  }
}

void setup() {
  u8g2.begin();
  u8g2.setFontMode(true);
  Serial.begin(115200);
}

void fillNoise(int x, int y, int w, int h, int prc) {
  for (int x1 = x; x1 <= x + w; x1++) {
    for (int y1 = y; y1 <= y + h; y1++) {
      if (prc >= random(101)) u8g2.drawPixel(x1, y1);
    }
  }
}

void fillCubeWithNoise(long double side, int Xoff, int Yoff, int prc) {
  side += ((float)prc / 100.0) * side;
  fillNoise(Xoff-side/2, Yoff-side/2, side, side, prc);
}

void cubeFrameNoise(long double side, int Xoff, int Yoff, void (*cubeFunc)(), void (*bgFunc)(), void (*bgFunc2)() = NULL) {
  if (bgFunc2 == NULL) bgFunc2 = bgFunc;
  for (int i=1; i <= 20; i+=2) {
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    cubeFunc();
    u8g2.setDrawColor(2);
    fillCubeWithNoise(side, Xoff, Yoff, i);
    u8g2.setDrawColor(1);
    bgFunc();
    u8g2.sendBuffer();
    delay(50);
  }
  for (int i=15; i >= 1; i-=3) {
    u8g2.clearBuffer();
    u8g2.setDrawColor(2);
    fillCubeWithNoise(side, Xoff, Yoff, i);
    u8g2.setDrawColor(1);
    bgFunc2();
    u8g2.sendBuffer();
    delay(50);
  };
  for (int i=2; i >= 1; i--) {
    u8g2.clearBuffer();
    u8g2.setDrawColor(2);
    fillCubeWithNoise(side, Xoff, Yoff, i);
    u8g2.setDrawColor(1);
    bgFunc2();
    u8g2.sendBuffer();
    delay(50);
  };
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  bgFunc2();
  u8g2.sendBuffer();
  delay(2000);
}

void doorShow(bool mod) { // true - opening, false - closing
  if (mod) {
    for (int i=100;i>=0;i-=5) {
      u8g2.clearBuffer();
      drawCheckbox(u8g2, 75, 10, true);
      drawDoor(u8g2, 28, 35, 35, i);
      u8g2.sendBuffer();
      delay(10);
    }
  } else {
    for (int i=0;i<=100;i+=5) {
      u8g2.clearBuffer();
      drawCheckbox(u8g2, 75, 10, false);
      drawDoor(u8g2, 28, 35, 35, i);
      u8g2.sendBuffer();
      delay(10);
    }
  }
  delay(2000);
}

void intro() {
  u8g2.clearBuffer();
  showMsg("Hello, and welcome to...", 100, H/2);
  delay(2000);
  for (int i=360;i >= 15; i-=10) {
    u8g2.clearBuffer();
    drawLogo(u8g2, 15, 22, 25, 6.0 * PI / 180.0, 1, i * PI / 180.0);
    u8g2.sendBuffer();
  }
  u8g2.clearBuffer();
  drawBigLogo(u8g2, 7, 10);
  u8g2.sendBuffer();
  delay(4000);
  u8g2.clearBuffer();
  showMsg("...on our products presentation.", 100, H/2);
  delay(4000);
}

void cubeShowButton(int btnW, int btnY, int extraY) {
  drawButton(u8g2, btnW, W/2, 50, extraY);
}

void cubeShowFrame() {
  cubeShowButton(btnW, btnY, 1);
}

void cubeShowFrame2() {
  cubeShowButton(btnW, btnY, btnH);
}

void cubeShowStorageFrame() {
  drawStorageCube(u8g2, cubeS, W/2, btnY - (cubeS / 2) - 2);
}

void cubeShowCompanionFrame() {
  drawCompanionCube(u8g2, cubeS, W/2, btnY - (cubeS / 2) - 2);
}

void cubeShow() {
  u8g2.clearBuffer();
  cubeShowButton(btnW, btnY, btnH);
  u8g2.sendBuffer();
  delay(2000);
  u8g2.setFont(u8g2_font_profont10_tr);
  centrePrint("1500 Megawatt Aperture", msgOff*1);
  centrePrint("Science Heavy Duty", msgOff*2);
  centrePrint("Super-Colliding", msgOff*3);
  centrePrint("Super Button", msgOff*4);
  u8g2.sendBuffer();
  delay(10000);
  for (int i = (cubeS / 2); i <= btnY - (cubeS / 2) - 1; i += 2) {
    u8g2.clearBuffer();
    cubeShowButton(btnW, btnY, min(btnY - (cubeS / 2) - i - 1, btnH));
    drawStorageCube(u8g2, cubeS, W/2, i);
    u8g2.sendBuffer();
    delay(10);
  }
  delay(2000);
  u8g2.setFont(u8g2_font_profont10_tr);
  centrePrint("Weighted Storage Cube", msgOff*1);
  u8g2.sendBuffer();
  delay(5000);
  doorShow(true);
  cubeFrameNoise(cubeS, W/2, btnY - (cubeS / 2) - 1, cubeShowStorageFrame, cubeShowFrame, cubeShowFrame2);
  doorShow(false);
  for (int i = (cubeS / 2); i <= btnY - (cubeS / 2) - 1; i += 2) {
    u8g2.clearBuffer();
    cubeShowButton(btnW, btnY, min(btnY - (cubeS / 2) - i - 1, btnH));
    drawCompanionCube(u8g2, cubeS, W/2, i);
    u8g2.sendBuffer();
    delay(10);
  }
  delay(2000);
  u8g2.setFont(u8g2_font_profont10_tr);
  centrePrint("Weighted Companion Cube", msgOff*1);
  u8g2.sendBuffer();
  delay(5000);
  cubeFrameNoise(cubeS, W/2, btnY - (cubeS / 2) - 1, cubeShowCompanionFrame, cubeShowFrame, cubeShowFrame2);
}

void loop() {
  //intro();
  cubeShow();
  //testDoor();
}
