
#include <checkbox.h>
#include <thread>
#include "log.h"

  void Checkbox::getParentBitmap(){
     
     if(this->parentBitmap != NULL){
        DeleteObject(this->parentBitmap);
    }
     
     HDC parentDc = GetDC(this->parent);
     
     HDC memoryDc = CreateCompatibleDC(parentDc);
     this->parentBitmap = CreateCompatibleBitmap(parentDc,this->xSize,this->ySize);
     HBITMAP oldBitmap = (HBITMAP) SelectObject(memoryDc,this->parentBitmap);

     BitBlt(memoryDc,0,0,this->xSize,this->ySize,parentDc,this->xPos,this->yPos,SRCCOPY);

     SelectObject(memoryDc,oldBitmap);
     DeleteDC(memoryDc);
     ReleaseDC(this->parent,parentDc);
  }

  void Checkbox::paint(const HWND& hwnd){      
     PAINTSTRUCT ps;
     HDC dc = BeginPaint(hwnd, &ps);
     HDC memoryDc = CreateCompatibleDC(dc);
     HBITMAP memoryBitmap = CreateCompatibleBitmap(dc,this->xSize,this->ySize);
     HBITMAP oldBitmap = (HBITMAP) SelectObject(memoryDc,memoryBitmap);

     Graphics graphics(memoryDc);

     Bitmap bitmap(this->parentBitmap,NULL);

     graphics.DrawImage(&bitmap,0,0,this->xSize,this->ySize);


     Rect rect = {0,0,this->xSize,this->ySize};

     Pen toggleStroke(Color(200,200,200),1);

     if(switchOn){
       SolidBrush onBrush(this->backgroundOnColor);
       graphics.FillRectangle(&onBrush,rect);
     }else{
       SolidBrush offBrush(Color(this->backgroundOffColor));
       graphics.FillRectangle(&offBrush,rect);
     }


    if(this->hover){

      // SolidBrush hoverBrush(Color(70,255,255,255));
      //
      // graphics.FillRectangle(&hoverBrush,rect);
      
    }

    if(this->switchOn){
      graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
      Pen tickPen(Color(255,255,255),1.5);
      SolidBrush tempbrush(Color(128,0,0,0));
      int innerWidth = this->xSize/6;
      Rect tickRectangle = {innerWidth,innerWidth,this->xSize-(innerWidth*2),this->ySize-(innerWidth*2)};
      // graphics.FillRectangle(&tempbrush,tickRectangle);
      graphics.DrawLine(&tickPen,innerWidth,this->ySize/1.5,this->xSize/2.5,tickRectangle.Width+innerWidth);
      graphics.DrawLine(&tickPen,this->xSize/2.5,tickRectangle.Width+innerWidth,tickRectangle.Width+innerWidth,tickRectangle.GetTop()+innerWidth);
    }else{
      graphics.DrawRectangle(&toggleStroke,0,0,rect.Width-1.5,rect.Height-1.5);
    }


    if(this->disabled){
      SolidBrush disabledBrush(Color(128,0,0,0));

      graphics.FillRectangle(&disabledBrush,rect);
    }
     
     BitBlt(dc,0,0,this->xSize,this->ySize,memoryDc,0,0,SRCCOPY);

     SelectObject(memoryDc,oldBitmap);
     DeleteObject(memoryBitmap);
     DeleteDC(memoryDc);
     EndPaint(hwnd,&ps);
  }

  LRESULT CALLBACK Checkbox::callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      switch(msg){
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
      break;
      case WM_LBUTTONDOWN:
          if(this->disabled == false){
            SetFocus(hwnd);
          }
      break;
      case WM_LBUTTONUP:
        if(this->switchOn == true && this->disabled == false){
          this->switchOn = false;
          update();
        }else if(this->switchOn == false && this->disabled == false){
          this->switchOn = true;
          update();
        }

        if(this->onChange != nullptr){
          onChange(this);
        }
      break;
      case WM_MOUSEMOVE:
        if(this->hover == false && this->disabled == false){
          registerMouseCapure(hwnd);
          this->hover = true;
          std::thread worker(Checkbox::hoverAnimation,this);
          worker.detach();
          update();
        }
      break;
      case WM_MOUSELEAVE:
        if(this->hover == true && this->disabled == false){
          this->hover = false;
          update();
        }
      break;
      case WM_PAINT:
        if(this->gotParentBitmap == false){
          getParentBitmap();
          this->gotParentBitmap = true;
        }
         paint(hwnd);
      break;
      case WM_ERASEBKGND:
      break;
      default:
        return DefWindowProcW(hwnd, msg, wp, lp);
      }
     return (LRESULT) nullptr;
  }
  

  Checkbox::Checkbox(const HWND& hwnd,const int& x,const int& y,const int& size){
     this->xPos = x;
     this->yPos = y;
     this->xSize = size;
     this->ySize = size;
     this->parent = hwnd;
     this->backgroundOnColor.SetFromCOLORREF(RGB(34,108,224));
     this->backgroundOffColor.SetFromCOLORREF(RGB(50,50,50));
  }

  Checkbox::~Checkbox(){
     DestroyWindow(this->handle);
     LOG("Checkbox destroyed");
  }

  void Checkbox::setParent(const HWND& parent){
      this->parent = parent;
      this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

      SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
      SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)Checkbox::callbackProcedure);

      LOG("Checkbox created");
  }


  void Checkbox::setBackgroundOnColor(const int& r,const int& g,const int& b,const int& a){
     
      this->backgroundOnColor.SetValue(Color::MakeARGB(a, r, g, b));
    update();
  }

  void Checkbox::setBackgroundOffColor(const int& r,const int& g,const int& b,const int& a){
     
      this->backgroundOffColor.SetValue(Color::MakeARGB(a, r, g, b));
    update();
  }

  bool Checkbox::buttonState(){
    return this->switchOn;
  } 
