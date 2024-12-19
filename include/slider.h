#pragma once

#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>

#include "element.h"
#include "static_element.h"

using namespace Gdiplus;

class Slider : public Element{
  private:
  int knobPosition;
  float position;
  float min = 0;
  float max = 1;
  StaticElement* hint = nullptr;
  Color backgroundColor;
  Color knobColor;
  bool isHint = false;
  bool latched = false;
  bool hover = false;
  bool switchOn = false;

public:
  void(*onChange)(Slider*) = nullptr;
private:

  void getParentBitmap();

  float remap(const float&,const float&,const float&,const float&,const float&)const;

  void DrawCircle(Graphics&,SolidBrush&,const Point&,float)const;

  void paint(const HWND& hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) override;
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      Slider* slider = (Slider*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(slider != nullptr){
         return slider->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  Slider(const HWND& hwnd,const int& x,const int& y,const int& size);

  ~Slider();

  float getMin()const;

  float getMax()const;

  float getPosition()const;

  void setParent(const HWND& parent) override; 

  void setBackgroundColor(const int& r,const int& g,const int& b);

  void setKnobColor(const int& r,const int& g,const int& b);

  void setBackgroundColor(const int& r,const int& g,const int& b,const int& a);

  void setKnobColor(const int& r,const int& g,const int& b,const int& a);

  void setMin(const float& value);

  void setMax(const float& value);

  void enableHint();

  void disableHint();

};
