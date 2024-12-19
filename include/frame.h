#pragma once

#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include <string>

#include "element.h"

using namespace Gdiplus;

class Frame : public Element{
  private:
  ULONG_PTR gdiplusToken;
  std::vector<Element*> childs;
  std::wstring backgroundImage;
  Color backgroundColor;
  bool isFocused = false;

public:
  void(*onFocus)(Frame*) = nullptr;
  void(*onSize)(Frame*) = nullptr;
  bool hasTitlebar = false;
private:

  void paint(const HWND& hwnd);

  void fullUpdate()override;
 
  LRESULT CALLBACK frameProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);

  static LRESULT CALLBACK frameProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
    Frame* frame = (Frame*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

    if(frame != nullptr){
      return frame->frameProcedureImplementation(hwnd, msg, wp, lp);
    }else{
      return DefWindowProcW(hwnd, msg, wp, lp);
    }
  }

public:

  Frame(const std::wstring& name,const int& x,const int& y,const int& cx,const int& cy,const bool& titlebar);

  ~Frame();

  void start();

  void add(Element* const element);

  void setBackgroundColor(const int& r,const int& g,const int& b);

  void setBackgroundImage(const wchar_t* path);

  HWND getHandle()const;

  void quit()const;

};
