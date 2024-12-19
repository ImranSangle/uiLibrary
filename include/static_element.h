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

  void getParentBitmap();

  void paint(const HWND& hwnd); 

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

  StaticElement(const HWND& hwnd,const int& x,const int& y,const int& cx,const int& cy);

  ~StaticElement();

  void setParent(const HWND& parent) override; 

  void changePosition(const int& x,const int& y) override;

  void setText(const std::wstring& name);

  std::wstring getText()const;

  void setTextColor(const int& r,const int& g,const int& b);

  void setTextSize(const int& size);

  void setBackgroundColor(const int& r,const int& g,const int& b,const int& a);

  void setBackgroundColor(const int& r,const int& g,const int& b);

  void setBackgroundImage(const wchar_t* path);

  void setFont(const wchar_t* fontname);
};
