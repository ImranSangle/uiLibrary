
#include <toggleButton.h>
#include <thread>
#include "log.h"

  void ToggleButton::getParentBitmap(){
     
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

  void ToggleButton::DrawCircle(Graphics& graphics,SolidBrush& brush,const Point& center, float radius)const{
      // Calculate the top-left corner of the bounding rectangle
      float x = center.X - radius;
      float y = center.Y - radius;

      RectF boundingRect(x, y, 2 * radius, 2 * radius);
      graphics.FillEllipse(&brush,boundingRect);
  }

  void ToggleButton::paint(const HWND& hwnd){      
     PAINTSTRUCT ps;
     HDC dc = BeginPaint(hwnd, &ps);
     HDC memoryDc = CreateCompatibleDC(dc);
     HBITMAP memoryBitmap = CreateCompatibleBitmap(dc,this->xSize,this->ySize);
     HBITMAP oldBitmap = (HBITMAP) SelectObject(memoryDc,memoryBitmap);

     Graphics graphics(memoryDc);

     Bitmap bitmap(this->parentBitmap,NULL);

     graphics.DrawImage(&bitmap,0,0,this->xSize,this->ySize);

     graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

     Rect rect = {2,2,this->xSize-4,this->ySize-4};

     int borderRadius = rect.Height/2; // Adjust border radius as needed

     GraphicsPath path;
     path.AddArc(rect.X, rect.Y, 2 * borderRadius, 2 * borderRadius, 90, 180); // Left arc
     path.AddArc(rect.GetRight() - 2 * borderRadius, rect.Y, 2 * borderRadius, 2 * borderRadius, 270, 180); // Right arc
     path.CloseFigure();

     Pen toggleStroke(Color(255,255,255),1);

     if(switchOn){
       SolidBrush onBrush(this->backgroundOnColor);
       graphics.FillPath(&onBrush,&path);
     }else{
       SolidBrush offBrush(Color(this->backgroundOffColor));
       graphics.FillPath(&offBrush,&path);
     }

    float knobRadius = borderRadius-this->ySize/6.0f;

    if(this->hover){

      // SolidBrush hoverBrush(Color(75,255,255,255));
      //
      // graphics.FillPath(&hoverBrush,&path);
      
      knobRadius = borderRadius-this->ySize/10.0f;
      
    }

    if(this->switchOn){
      SolidBrush knobOnBrush(Color(255,255,255));
      Point center((this->ySize/2)+(this->xSize/2),this->ySize/2);
      DrawCircle(graphics,knobOnBrush,center,knobRadius);
    }else{
      SolidBrush knobOffBrush(Color(200,200,200));
      Point center(this->ySize/2,this->ySize/2);
      DrawCircle(graphics,knobOffBrush,center,knobRadius);
      graphics.DrawPath(&toggleStroke,&path);
    }


    if(this->disabled){
      SolidBrush disabledBrush(Color(128,0,0,0));

      graphics.FillPath(&disabledBrush,&path);
    }
     
     BitBlt(dc,0,0,this->xSize,this->ySize,memoryDc,0,0,SRCCOPY);

     SelectObject(memoryDc,oldBitmap);
     DeleteObject(memoryBitmap);
     DeleteDC(memoryDc);
     EndPaint(hwnd,&ps);
  }

  LRESULT CALLBACK ToggleButton::callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      switch(msg){
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
      break;
      case WM_LBUTTONDOWN:
          SetFocus(hwnd);
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
          std::thread worker(ToggleButton::hoverAnimation,this);
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
  

  ToggleButton::ToggleButton(const HWND& hwnd,const int& x,const int& y,const int& size){
     this->xPos = x;
     this->yPos = y;
     this->xSize = size*2;
     this->ySize = size;
     this->parent = hwnd;
     this->backgroundOnColor.SetFromCOLORREF(RGB(34,108,224));
     this->backgroundOffColor.SetFromCOLORREF(RGB(50,50,50));
  }

  ToggleButton::~ToggleButton(){
     DestroyWindow(this->handle);
     LOG("ToggleButton destroyed");
  }

  void ToggleButton::setParent(const HWND& parent){
      this->parent = parent;
      this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

      SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
      SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)ToggleButton::callbackProcedure);

      LOG("toggleButton created");
  }


  void ToggleButton::setBackgroundOnColor(const int& r,const int& g,const int& b){
     
     this->backgroundOnColor.SetFromCOLORREF(RGB(r,g,b));
    update();
  }

  void ToggleButton::setBackgroundOffColor(const int& r,const int& g,const int& b){
     
     this->backgroundOffColor.SetFromCOLORREF(RGB(r,g,b));
    update();
  }

  void ToggleButton::setBackgroundOnColor(const int& r,const int& g,const int& b,const int& a){
     this->backgroundOnColor.SetValue(Color::MakeARGB(a,r,g,b));
     update();
  }

  void ToggleButton::setBackgroundOffColor(const int& r,const int& g,const int& b,const int& a){
     this->backgroundOnColor.SetValue(Color::MakeARGB(a,r,g,b));
     update();
  }

  bool ToggleButton::buttonState()const{
    return this->switchOn;
  } 
