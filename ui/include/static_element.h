#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class StaticElement : public Element{
  private:
  int xPos;
  int yPos;
  int xSize;
  int ySize;
  int textSize;
  HWND parent;
  bool gotParentBitmap = false;
  HBITMAP parentBitmap = NULL;
  HWND handle;
  Color backgroundColor;
  Color textColor;
  std::wstring backgroundImage; 
  std::wstring text;
  std::wstring font;
  bool hover = false;
  bool pressed = false;
  bool disabled = false;
  bool autoResize = false;

public:
  void(*onClick)(StaticElement*) = nullptr;
  bool latchButton = false;
private:

  void registerMouseCapure(HWND hwnd);

  void getParentBitmap();

  void update();

  void updateParent();

  void paint(HWND hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
  
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

  void setParent(HWND parent) override; 

  void setText(const std::wstring& name);

  std::wstring getText();

  void setTextColor(int r,int g,int b);

  void setTextSize(int size);

  void setBackgroundColor(int r,int g,int b,int a);

  void setBackgroundColor(int r,int g,int b);

  void setBackgroundImage(const wchar_t* path);

  void setFont(const wchar_t* fontname);

  void changePosition(int x,int y);

  void disable();

  void enable();

};
