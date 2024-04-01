#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class ToggleButton : public Element{
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
  void(*onChange)(ToggleButton*) = nullptr;
private:

  static void hoverAnimation(ToggleButton* object){

     HDC dc = GetDC(object->handle);
      
     Graphics graphics(dc);

     graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

     Rect rect = {4,4,object->xSize-8,object->ySize-8};

     int borderRadius = rect.Height/2; // Adjust border radius as needed

     GraphicsPath path;
     path.AddArc(rect.X, rect.Y, 2 * borderRadius, 2 * borderRadius, 90, 180); // Left arc
     path.AddArc(rect.GetRight() - 2 * borderRadius, rect.Y, 2 * borderRadius, 2 * borderRadius, 270, 180); // Right arc
     path.CloseFigure();

     SolidBrush backbrush(Color(7,255,255,255));

     for(int i =0;i<10;i++){
      if(object->hover == false){
        return;
      }
        graphics.FillPath(&backbrush,&path);
        Sleep(1);
      }

     ReleaseDC(object->handle,dc);
  }

  void registerMouseCapure(HWND hwnd);

  void getParentBitmap();

  void update();

  void updateParent();

  void DrawCircle(Graphics&,SolidBrush&,const Point&,float);

  void paint(HWND hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      ToggleButton* togglebutton = (ToggleButton*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(togglebutton != nullptr){
         return togglebutton->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  ToggleButton(HWND hwnd,int x,int y,int size);

  ~ToggleButton();

  void setParent(HWND parent) override; 

  void setBackgroundOnColor(int r,int g,int b);

  void setBackgroundOffColor(int r,int g,int b);

  void changePosition(int x,int y);

  void disable();

  void enable();

  bool buttonState();

};
