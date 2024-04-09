#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class Progressbar : public Element{
  private:
  int xPos;
  int yPos;
  int xSize;
  int ySize;
  float min = 0;
  float max = 100;
  float progress;
  HWND parent;
  bool gotParentBitmap = false;
  HBITMAP parentBitmap = NULL;
  HWND handle;
  Color backgroundColor;
  Color barColor;
  std::wstring backgroundImage; 
  bool hover = false;
  bool pressed = false;
  bool disabled = false;

public:
  bool vertical = false;
  bool reversed = false;
  void(*onClick)(Progressbar*) = nullptr;
private:

  void registerMouseCapure(HWND hwnd);

  void getParentBitmap();

  void update();

  void updateParent();

  void fullUpdate();

  float remap(const float&,const float&,const float&,const float&,const float&);

  void paint(HWND hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      Progressbar* progressbar = (Progressbar*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(progressbar != nullptr){
         return progressbar->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  Progressbar(HWND hwnd,int x,int y,int cx,int cy);

  ~Progressbar();

  int getX() override;

  int getY() override;

  int getWidth() override;

  int getHeight() override;

  float getMin();

  float getMax();

  float getProgress();

  void setParent(HWND parent) override; 

  void setBackgroundColor(int r,int g,int b,int a);

  void setBackgroundColor(int r,int g,int b);

  void setBackgroundImage(const std::wstring& path);

  void setBarColor(int r,int g,int b,int a);

  void setBarColor(int r,int g,int b);

  void setMin(const float& value);

  void setMax(const float& value);

  void setProgress(const float& value);

  void changePosition(int x,int y) override;

  void changeSize(int width,int height) override;

  void show();

  void hide();

  void disable();

  void enable();

};
