#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class Button : public Element{
  private:
  int xPos;
  int yPos;
  int xSize;
  int ySize;
  int textSize;
  HWND parent;
  HWND handle;
  Color backgroundColor;
  Color textColor;
  std::wstring backgroundImage; 
  std::wstring text;
  std::wstring font;
  bool hover = false;
  bool pressed = false;

public:
  void(*onClick)(Button*) = nullptr;
  bool latchButton = false;
private:

  static void hoverAnimation(Button* object){

     HDC dc = GetDC(object->handle);
      
     Graphics graphics(dc);

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

  void updateButton();

  void paint(HWND hwnd); 

  LRESULT CALLBACK buttonProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
  
  static LRESULT CALLBACK buttonProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      Button* button = (Button*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(button != nullptr){
         return button->buttonProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  Button(HWND hwnd,int x,int y,int cx,int cy);

  ~Button();

  void setParent(HWND parent) override; 

  void setText(const wchar_t* name);

  std::wstring getText();

  void setTextColor(int r,int g,int b);

  void setTextSize(int size);

  void setBackgroundColor(int r,int g,int b);

  void setBackgroundImage(const wchar_t* path);

  void setFont(const wchar_t* fontname);

  bool buttonState();

};
