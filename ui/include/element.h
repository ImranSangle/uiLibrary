#pragma once

#include <windows.h>

class Element{
  private:
  public:

  virtual void update() = 0;

  virtual void fullUpdate() = 0;

  virtual void setParent(HWND parent) = 0;

  virtual void changePosition(int x,int y) = 0;
  
  virtual void changeSize(int width,int height) = 0;

  virtual int getX() = 0;

  virtual int getY() = 0;

  virtual int getWidth() = 0;

  virtual int getHeight() = 0;

};
