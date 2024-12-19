#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class LinearLayout : public Element{
  private:
  float yLength = 0;
  float padding = 0;
  float scrollbarYPosition = 0;
  int scrollVelocity = 0;
  WPARAM scrollDirection;
  Color backgroundColor;
  std::wstring backgroundImage; 
  bool hover = false;
  bool pressed = false;
  std::vector<Element*> childs;

public:
  void(*onClick)(LinearLayout*) = nullptr;
  void(*onSize)(LinearLayout*) = nullptr;
private:

  void getParentBitmap();

  void updateScrollbar()const;

  void scroll(const WPARAM& wp,const int& velocity);

  void paint(const HWND& hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) override;
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      LinearLayout* linearLayout = (LinearLayout*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(linearLayout != nullptr){
         return linearLayout->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

 static void scrollWorker(LinearLayout* const layout){
    
    while(layout->scrollVelocity > 0){
      layout->scroll(layout->scrollDirection,layout->scrollVelocity);
      layout->scrollVelocity--;
    }
      layout->update();
  }

public: 

  LinearLayout(const HWND& hwnd,const int& x,const int& y,const int& cx,const int& cy);

  ~LinearLayout();

  std::vector<Element*> getChilds() const;

  void add(Element* const element);

  void setParent(const HWND& parent) override; 

  void changePosition(const int& x,const int& y) override;

  void setBackgroundColor(const int& r,const int& g,const int& b,const int& a);

  void setBackgroundColor(const int& r,const int& g,const int& b);

  void setBackgroundImage(const std::wstring& path);

  void setPadding(const int& paddingAmount);

};
