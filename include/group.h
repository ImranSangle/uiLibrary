#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class Group : public Element{
  private:
  float padding = 0;
  Color backgroundColor;
  std::wstring backgroundImage; 
  bool hover = false;
  bool pressed = false;
  std::vector<Element*> childs;

public:
  void(*onClick)(Group*) = nullptr;
  void(*onSize)(Group*) = nullptr;
private:

  void registerMouseCapure(HWND hwnd);

  void getParentBitmap();

  void paint(HWND hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) override;
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      Group* group = (Group*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(group != nullptr){
         return group->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  Group(HWND hwnd,int x,int y,int cx,int cy);

  ~Group();

  std::vector<Element*> getChilds();

  void add(Element* element);

  void setParent(const HWND& parent) override; 

  void changePosition(const int& x,const int& y) override;

  void setBackgroundColor(int r,int g,int b,int a);

  void setBackgroundColor(int r,int g,int b);

  void setBackgroundImage(const std::wstring& path);

  void setPadding(int paddingAmount);
};
