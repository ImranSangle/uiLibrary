#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class Progressbar : public Element{
  private:
  float min = 0;
  float max = 100;
  float progress;
  Color backgroundColor;
  Color barColor;
  std::wstring backgroundImage; 
  bool hover = false;
  bool pressed = false;

public:
  bool vertical = false;
  bool reversed = false;
  void(*onClick)(Progressbar*) = nullptr;
private:

  void getParentBitmap();

  float remap(const float&,const float&,const float&,const float&,const float&)const;

  void paint(const HWND& hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) override;
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      Progressbar* progressbar = (Progressbar*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(progressbar != nullptr){
         return progressbar->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  Progressbar(const HWND& hwnd,const int& x,const int& y,const int& cx,const int& cy);

  ~Progressbar();

  float getMin()const;

  float getMax()const;

  float getProgress()const;

  void setParent(const HWND& parent) override; 

  void setBackgroundColor(const int& r,const int& g,const int& b,const int& a);

  void setBackgroundColor(const int& r,const int& g,const int& b);

  void setBackgroundImage(const std::wstring& path);

  void setBarColor(const int& r,const int& g,const int& b,const int& a);

  void setBarColor(const int& r,const int& g,const int& b);

  void setMin(const float& value);

  void setMax(const float& value);

  void setProgress(const float& value);
};
