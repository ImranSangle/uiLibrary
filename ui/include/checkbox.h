#pragma once

#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class Checkbox : public Element{
  private:
  int xPos;
  int yPos;
  int xSize;
  int ySize;
  HWND parent;
  bool gotParentBitmap = false;
  HBITMAP parentBitmap = NULL;
  HWND handle;
  Color backgroundOnColor;
  Color backgroundOffColor;
  bool hover = false;
  bool switchOn = false;
  bool disabled = false;

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

  void update();

  void updateParent();

  void paint(HWND hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
  
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

  void setParent(HWND parent) override; 

  void setBackgroundOnColor(int r,int g,int b,int a);

  void setBackgroundOffColor(int r,int g,int b,int a);

  void changePosition(int x,int y);

  void disable();

  void enable();

  bool buttonState();

};
