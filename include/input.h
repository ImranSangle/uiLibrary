#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "element.h"

using namespace Gdiplus;

class InputBox : public Element{
  private:
  int textSize;
  Color backgroundColor;
  Color textColor;
  std::wstring backgroundImage; 
  std::wstring text;
  std::wstring hint;
  std::wstring font;
  bool hover = false;
  bool isFocused = false;

  bool stringLock = false;

public:
  void(*onClick)(InputBox*) = nullptr;
  void(*onInput)(InputBox*) = nullptr;
  void(*onFocus)(InputBox*) = nullptr;
  bool hidden = false;
  bool alignCenter = false;
private:

  static void hoverAnimation(InputBox* const object){

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

  void handleKeyboardEvents(const HWND& hwnd,const WPARAM& wp);

  void paint(const HWND& hwnd); 

  LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) override;

  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      InputBox* input = (InputBox*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(input != nullptr){
         return input->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

public: 

  InputBox(const HWND& hwnd,const int& x,const int& y,const int& cx,const int& cy);

  ~InputBox();

  void setParent(const HWND& parent) override;

  void setText(const std::wstring& name);
  
  std::wstring getText() const ;

  void setHint(const std::wstring& hint);

  void setTextColor(const int& r,const int& g,const int& b);

  void setTextSize(const int& size);

  void setBackgroundColor(const int& r,const int& g,const int& b,const int& a);

  void setBackgroundColor(const int& r,const int& g,const int& b);

  void setBackgroundImage(const std::wstring& path);

  void setFont(const std::wstring& fontname);

  bool isInputFocused() const;

};
