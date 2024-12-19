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

  void getParentBitmap();

  void paint(const HWND& hwnd); 

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

  Group(const HWND& hwnd,const int& x,const int& y,const int& cx,const int& cy);

  ~Group();

  std::vector<Element*> getChilds();

  void add(Element* const element);

  void setParent(const HWND& parent) override; 

  void changePosition(const int& x,const int& y) override;

  void setBackgroundColor(const int& r,const int& g,const int& b,const int& a);

  void setBackgroundColor(const int& r,const int& g,const int& b);

  void setBackgroundImage(const std::wstring& path);

  void setPadding(const int& paddingAmount);
};
