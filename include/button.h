#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class Button : public Element{
  private:
  int textSize;
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

  static void hoverAnimation(Button* const object){

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

  void getParentBitmap();

  void paint(const HWND& hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) override;
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      Button* button = (Button*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(button != nullptr){
         return button->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  Button(const HWND& hwnd,const int& x,const int& y,const int& cx,const int& cy);

  ~Button();

  void setParent(const HWND& parent) override; 

  void setText(const std::wstring& name);

  std::wstring getText();

  void setTextColor(const int& r,const int& g,const int& b);

  void setTextSize(const int& size);

  void setBackgroundColor(const int& r,const int& g,const int& b,const int& a);

  void setBackgroundColor(const int& r,const int& g,const int& b);

  void setBackgroundImage(const std::wstring& path);

  void setFont(const std::wstring& fontname);

  bool buttonState();

};
