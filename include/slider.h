#pragma once

#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>

#include "element.h"
#include "static_element.h"

using namespace Gdiplus;

class Slider : public Element{
  private:
  int xPos;
  int yPos;
  int xSize;
  int ySize;
  int knobPosition;
  float position;
  float min = 0;
  float max = 1;
  HWND parent;
  StaticElement* hint = nullptr;
  bool gotParentBitmap = false;
  HBITMAP parentBitmap = NULL;
  HWND handle;
  Color backgroundColor;
  Color knobColor;
  bool isHint = false;
  bool latched = false;
  bool hover = false;
  bool switchOn = false;
  bool disabled = false;

public:
  void(*onChange)(Slider*) = nullptr;
private:

  void registerMouseCapure(HWND hwnd);

  void getParentBitmap();

  void update();

  void updateParent();

  void fullUpdate();

  float remap(const float&,const float&,const float&,const float&,const float&);

  void DrawCircle(Graphics&,SolidBrush&,const Point&,float);

  void paint(HWND hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      Slider* slider = (Slider*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(slider != nullptr){
         return slider->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  Slider(HWND hwnd,int x,int y,int size);

  ~Slider();

  int getX() override;

  int getY() override;

  int getWidth() override;

  int getHeight() override;

  float getMin();

  float getMax();

  float getPosition();

  void setParent(HWND parent) override; 

  void setBackgroundColor(int r,int g,int b);

  void setKnobColor(int r,int g,int b);

  void setBackgroundColor(int r,int g,int b,int a);

  void setKnobColor(int r,int g,int b,int a);

  void setMin(const float& value);

  void setMax(const float& value);

  void changePosition(int x,int y) override;

  void changeSize(int width,int height) override;

  void show();

  void hide();

  void disable();

  void enable();

  void enableHint();

  void disableHint();

};
