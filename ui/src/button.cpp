
#include <button.h>
#include <thread>
#include <iostream>

  void Button::registerMouseCapure(HWND hwnd){
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);

  }

  void Button::updateButton(){
     InvalidateRect(this->handle,NULL,TRUE);
  }

  void Button::paint(HWND hwnd){      
     PAINTSTRUCT ps;
     HDC dc = BeginPaint(hwnd, &ps);
     HDC parentDc = GetDC(this->parent);

     Graphics Graphics(dc);

     RectF rect = {0,0,(float)this->xSize,(float)this->ySize};

    if(this->backgroundImage.length() > 0){

      Bitmap bitmap(this->backgroundImage.c_str());

      Graphics.DrawImage(&bitmap,0,0,this->xSize,this->ySize);
       
    }else{

      SolidBrush backbrush(this->backgroundColor);
      
      Graphics.FillRectangle(&backbrush,rect);

    }

     Font font(this->font.c_str(),this->textSize);
     
     SolidBrush brush(this->textColor);

     StringFormat format;
     
     format.SetAlignment(StringAlignmentCenter);
     
     format.SetLineAlignment(StringAlignmentCenter);

     Graphics.DrawString(this->text.c_str(),-1,&font,rect,&format,&brush);

    if(this->hover){

      SolidBrush backbrush(Color(75,255,255,255));

      Graphics.FillRectangle(&backbrush,rect);
    }
    
    if(this->pressed){
      SolidBrush backbrush(Color(100,255,255,255));

      Graphics.FillRectangle(&backbrush,rect);
    }

     EndPaint(hwnd,&ps);
  }

  LRESULT CALLBACK Button::buttonProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      switch(msg){
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
      break;
      case WM_LBUTTONDOWN:
        if(this->pressed == false){
          this->pressed = true;
          SetFocus(hwnd);
          updateButton();

        }else if(this->pressed == true && this->latchButton == true){
          this->pressed = false;
          SetFocus(hwnd);
          updateButton();
        }
      break;
      case WM_LBUTTONUP:
        if(this->pressed == true && this->latchButton == false){
          this->pressed = false;
          updateButton();
          if(this->onClick != nullptr){
            onClick(this);
          }
        }
      break;
      case WM_MOUSEMOVE:
        registerMouseCapure(hwnd);
        if(this->hover == false){
          this->hover = true;
          std::thread worker(Button::hoverAnimation,this);
          worker.detach();
          // updateButton();
        }
      break;
      case WM_MOUSELEAVE:
        if(this->hover == true){
          this->hover = false;
          updateButton();
        }
      break;
      case WM_PAINT:
         paint(hwnd);
      break;
      case WM_ERASEBKGND:
      break;
      default:
        return DefWindowProcW(hwnd, msg, wp, lp);
      }
     return (LRESULT) nullptr;
  }
  

  Button::Button(HWND hwnd,int x,int y,int cx,int cy){
     this->xPos = x;
     this->yPos = y;
     this->xSize = cx;
     this->ySize = cy;
     this->parent = hwnd;
     this->backgroundColor.SetFromCOLORREF(RGB(100,100,100));
     this->textColor.SetFromCOLORREF(RGB(255,255,255));
     this->font = L"Roboto";
     this->textSize = 12;

     // this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);
     //
     // SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
     // SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)Button::buttonProcedure);
     //
     // std::cout<<"button created"<<std::endl;
  }

  Button::~Button(){
     DestroyWindow(this->parent);
     std::cout<<"button destroyed"<<std::endl;
  }

  void Button::setParent(HWND parent){
      this->parent = parent;
      this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

      SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
      SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)Button::buttonProcedure);

      std::cout<<"button created"<<std::endl;
  }

  void Button::setText(const wchar_t* name){
     this->text = name; 
    updateButton();
  }

  std::wstring Button::getText(){
     return this->text;
  }

  void Button::setTextColor(int r,int g,int b){
     
     this->textColor.SetFromCOLORREF(RGB(r,g,b));
    updateButton();
  }

  void Button::setTextSize(int size){
    this->textSize = size;
    updateButton();
  }

  void Button::setBackgroundColor(int r,int g,int b){
     
     this->backgroundColor.SetFromCOLORREF(RGB(r,g,b));
    updateButton();
  }

  void Button::setBackgroundImage(const wchar_t* path){
     this->backgroundImage = path;
    updateButton();
  }

  void Button::setFont(const wchar_t* fontname){
    this->font = fontname;
    updateButton();
  }

  bool Button::buttonState(){
    return this->pressed;
  }
