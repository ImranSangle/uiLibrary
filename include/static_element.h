#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class StaticElement : public Element{
  private:
  int textSize;
  Color backgroundColor;
  Color textColor;
  std::wstring backgroundImage; 
  std::wstring text;
  std::wstring font;
  bool hover = false;
  bool pressed = false;
  bool autoResize = false;

public:
  void(*onClick)(StaticElement*) = nullptr;
private:

  void registerMouseCapure(HWND hwnd);

  void getParentBitmap();

  void paint(HWND hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) override;
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      StaticElement* staticElement = (StaticElement*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(staticElement != nullptr){
         return staticElement->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  StaticElement(HWND hwnd,int x,int y,int cx,int cy);

  ~StaticElement();

  void setParent(const HWND& parent) override; 

  void changePosition(const int& x,const int& y) override;

  void setText(const std::wstring& name);

  std::wstring getText();

  void setTextColor(int r,int g,int b);

  void setTextSize(int size);

  void setBackgroundColor(int r,int g,int b,int a);

  void setBackgroundColor(int r,int g,int b);

  void setBackgroundImage(const wchar_t* path);

  void setFont(const wchar_t* fontname);
};
