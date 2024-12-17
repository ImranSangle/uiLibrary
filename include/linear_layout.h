#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class LinearLayout : public Element{
  private:
  int xPos;
  int yPos;
  int xSize;
  int ySize;
  float yLength = 0;
  float padding = 0;
  float scrollbarYPosition = 0;
  int scrollVelocity = 0;
  WPARAM scrollDirection;
  HWND parent;
  bool gotParentBitmap = false;
  HBITMAP parentBitmap = NULL;
  HWND handle;
  Color backgroundColor;
  std::wstring backgroundImage; 
  bool hover = false;
  bool pressed = false;
  bool disabled = false;
  std::vector<Element*> childs;

public:
  void(*onClick)(LinearLayout*) = nullptr;
  void(*onSize)(LinearLayout*) = nullptr;
private:

  void registerMouseCapure(HWND hwnd);

  void getParentBitmap();

  void update();

  void updateParent();

  void fullUpdate();

  void updateScrollbar();

  void scroll(WPARAM wp,int velocity);

  void paint(HWND hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      LinearLayout* linearLayout = (LinearLayout*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(linearLayout != nullptr){
         return linearLayout->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

 static void scrollWorker(LinearLayout* layout){
    
    while(layout->scrollVelocity > 0){
      layout->scroll(layout->scrollDirection,layout->scrollVelocity);
      layout->scrollVelocity--;
    }
      layout->update();
  }

public: 

  LinearLayout(HWND hwnd,int x,int y,int cx,int cy);

  ~LinearLayout();

  int getX() override;

  int getY() override;

  int getWidth() override;

  int getHeight() override;

  std::vector<Element*> getChilds();

  void add(Element* element);

  void setParent(HWND parent) override; 

  void setBackgroundColor(int r,int g,int b,int a);

  void setBackgroundColor(int r,int g,int b);

  void setBackgroundImage(const std::wstring& path);

  void setPadding(int paddingAmount);

  void changePosition(int x,int y) override;

  void changeSize(int width,int height) override;

  void show();

  void hide();

  void disable();

  void enable();
};
