#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class InputBox : public Element{
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
  std::wstring hint;
  std::wstring font;
  bool hover = false;
  bool isFocused = false;
  bool disabled = false;

  bool stringLock = false;

public:
  void(*onClick)(InputBox*) = nullptr;
  void(*onInput)(InputBox*) = nullptr;
  void(*onFocus)(InputBox*) = nullptr;
  bool hidden = false;
  bool alignCenter = false;
private:

  static void hoverAnimation(InputBox* object){

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

  void update();

  void updateParent();

  void handleKeyboardEvents(HWND hwnd,WPARAM wp);

  void paint(HWND hwnd); 

  LRESULT CALLBACK inputProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
  
  static LRESULT CALLBACK inputProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      InputBox* input = (InputBox*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(input != nullptr){
         return input->inputProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  InputBox(HWND hwnd,int x,int y,int cx,int cy);

  ~InputBox();

  void setParent(HWND parent);

  void getParentBitmap();

  void setText(const wchar_t* name);
  
  std::wstring getText();

  void setHint(const wchar_t* hint);

  void setTextColor(int r,int g,int b);

  void setTextSize(int size);

  void setBackgroundColor(int r,int g,int b,int a);

  void setBackgroundColor(int r,int g,int b);

  void setBackgroundImage(const wchar_t* path);

  void setFont(const wchar_t* fontname);

  void changePosition(int x,int y);

  void disable();

  void enable();

  bool isInputFocused();

};
