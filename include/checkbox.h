#pragma once

#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class Checkbox : public Element{
  private:
  Color backgroundOnColor;
  Color backgroundOffColor;
  bool hover = false;
  bool switchOn = false;

public:
  void(*onChange)(Checkbox*) = nullptr;
private:

  static void hoverAnimation(Checkbox* object){

     HDC dc = GetDC(object->handle);
      
     Graphics graphics(dc);

     graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

     Rect rect = {0,0,object->xSize,object->ySize};

     SolidBrush backbrush(Color(7,255,255,255));

     for(int i =0;i<10;i++){
      if(object->hover == false){
        return;
      }
        graphics.FillRectangle(&backbrush,rect);
        Sleep(1);
      }

     ReleaseDC(object->handle,dc);
  }

  void registerMouseCapure(HWND hwnd);

  void getParentBitmap();

  void paint(HWND hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) override;
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      Checkbox* checkbox = (Checkbox*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(checkbox != nullptr){
         return checkbox->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  Checkbox(HWND hwnd,int x,int y,int size);

  ~Checkbox();

  void setParent(const HWND& parent) override; 

  void setBackgroundOnColor(int r,int g,int b,int a);

  void setBackgroundOffColor(int r,int g,int b,int a);

  bool buttonState();

};
