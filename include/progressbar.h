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

  void registerMouseCapure(HWND hwnd);

  void getParentBitmap();

  float remap(const float&,const float&,const float&,const float&,const float&);

  void paint(HWND hwnd); 

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

  Progressbar(HWND hwnd,int x,int y,int cx,int cy);

  ~Progressbar();

  float getMin();

  float getMax();

  float getProgress();

  void setParent(const HWND& parent) override; 

  void setBackgroundColor(int r,int g,int b,int a);

  void setBackgroundColor(int r,int g,int b);

  void setBackgroundImage(const std::wstring& path);

  void setBarColor(int r,int g,int b,int a);

  void setBarColor(int r,int g,int b);

  void setMin(const float& value);

  void setMax(const float& value);

  void setProgress(const float& value);
};
