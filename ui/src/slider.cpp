

#include "static_element.h"
#include <slider.h>
#include <iostream>
#include <string>

  void Slider::registerMouseCapure(HWND hwnd){
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);

  }
  
  void Slider::getParentBitmap(){
     
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

  void Slider::update(){
     InvalidateRect(this->handle,NULL,TRUE);
  }

  void Slider::updateParent(){

    RECT rt;
    rt.top = this->yPos;
    rt.left = this->xPos;
    rt.right = this->xSize+this->xPos;
    rt.bottom = this->ySize+this->yPos;
    InvalidateRect(this->parent,&rt,TRUE);
  } 

  void Slider::fullUpdate(){
    
     updateParent();
     this->gotParentBitmap = false;
     update();
  }

  float Slider::remap(const float& value,const float& a,const float& b,const float& c,const float& d){
      
      float t = (value-a)/(b-a); 

      return (c*(1.0f-t))+(d*t);
  }

  void Slider::DrawCircle(Graphics& graphics,SolidBrush& brush,const Point& center, float radius){
      // Calculate the top-left corner of the bounding rectangle
      float x = center.X - radius;
      float y = center.Y - radius;

      RectF boundingRect(x, y, 2 * radius, 2 * radius);
      graphics.FillEllipse(&brush,boundingRect);
  }

  void Slider::paint(HWND hwnd){      
     PAINTSTRUCT ps;
     HDC dc = BeginPaint(hwnd, &ps);
     HDC memoryDc = CreateCompatibleDC(dc);
     HBITMAP memoryBitmap = CreateCompatibleBitmap(dc,this->xSize,this->ySize);
     HBITMAP oldBitmap = (HBITMAP) SelectObject(memoryDc,memoryBitmap);

     Graphics graphics(memoryDc);

     Bitmap bitmap(this->parentBitmap,NULL);

     graphics.DrawImage(&bitmap,0,0,this->xSize,this->ySize);

     graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

     Rect rect = {6,8,this->xSize-12,4};

     int borderRadius = 2; // Adjust border radius as needed

     GraphicsPath path;
     path.AddArc(rect.X, rect.Y, 2 * borderRadius, 2 * borderRadius, 90, 180); // Left arc
     path.AddArc(rect.GetRight() - 2 * borderRadius, rect.Y, 2 * borderRadius, 2 * borderRadius, 270, 180); // Right arc
     path.CloseFigure();

     SolidBrush railBrush(this->backgroundColor);

     graphics.FillPath(&railBrush,&path);

    float knobRadius = this->ySize/4.0f;

    if(this->hover){

      SolidBrush hoverBrush(Color(70,255,255,255));

      graphics.FillPath(&hoverBrush,&path);

      knobRadius = this->ySize/3.0f; 
      
    }

    SolidBrush knobOnBrush(this->knobColor);
    Point center(std::max(6,std::min(this->knobPosition,this->xSize-10)),this->ySize/2);
    this->position = remap(center.X,6,this->xSize-10,this->min,this->max);
    DrawCircle(graphics,knobOnBrush,center,knobRadius);

    if(this->disabled){
      SolidBrush disabledBrush(Color(128,0,0,0));

      graphics.FillPath(&disabledBrush,&path);
      DrawCircle(graphics,disabledBrush,center,knobRadius);
    }
     
     BitBlt(dc,0,0,this->xSize,this->ySize,memoryDc,0,0,SRCCOPY);

     SelectObject(memoryDc,oldBitmap);
     DeleteObject(memoryBitmap);
     DeleteDC(memoryDc);
     EndPaint(hwnd,&ps);
  }

  LRESULT CALLBACK Slider::callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      switch(msg){
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
      break;
      case WM_LBUTTONDOWN:
      case WM_LBUTTONDBLCLK:
          if(this->disabled == false){
           this->latched = true;
           this->knobPosition = GET_X_LPARAM(lp);

           if(isHint){
             this->hint = new StaticElement(NULL,this->knobPosition,this->yPos-this->ySize,2,2);
             hint->setTextSize(12);
             hint->setBackgroundColor(this->backgroundColor.GetR(),this->backgroundColor.GetG(),this->backgroundColor.GetB());
             hint->setParent(this->parent);
             hint->setText(std::to_wstring(this->position));
           }

           update();
           SetFocus(hwnd);
             if(this->onChange != nullptr){
               this->onChange(this);
             }
          }
      break;
      case WM_LBUTTONUP:
        if(this->disabled == false){
          this->latched = false;
          if(this->hint != nullptr){
            delete this->hint;
            this->hint = nullptr;
          }
        }
      break;
      case WM_MOUSEMOVE:
        if(this->hover == false && this->disabled == false){
          registerMouseCapure(hwnd);
          this->hover = true;
          update();
        }
        if(this->latched){
          this->knobPosition = GET_X_LPARAM(lp);
          if(this->hint != nullptr){
            this->hint->setText(std::to_wstring(this->position));
          }
          update();
           if(this->onChange != nullptr){
             this->onChange(this);
           }
        }
      break;
      case WM_MOUSELEAVE:
        if(this->hover == true && this->disabled == false){
          this->hover = false;
          this->latched = false;
          if(this->hint != nullptr){
            delete this->hint;
            this->hint = nullptr;
          }
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
  

  Slider::Slider(HWND hwnd,int x,int y,int size){
     this->xPos = x;
     this->yPos = y;
     this->xSize = size;
     this->ySize = 20;
     this->parent = hwnd;
     this->backgroundColor.SetFromCOLORREF(RGB(30,30,30));
     this->knobColor.SetFromCOLORREF(RGB(34,108,224));
  }

  Slider::~Slider(){
     DestroyWindow(this->handle);
     std::cout<<"Slider destroyed"<<std::endl;
  }

  int Slider::getX(){
     return this->xPos;
  }

  int Slider::getY(){
     return this->yPos;
  }

  int Slider::getWidth(){
     return this->xSize;
  }

  int Slider::getHeight(){
     return this->ySize;
  }

  float Slider::getMin(){
     return this->min;
  }

  float Slider::getMax(){
     return this->max;
  }

  float Slider::getPosition(){
     return this->position;
  }

  void Slider::setParent(HWND parent){
      this->parent = parent;
      this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

      SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
      SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)Slider::callbackProcedure);

      std::cout<<"toggleButton created"<<std::endl;
  }


  void Slider::setBackgroundColor(int r,int g,int b){
     
     this->backgroundColor.SetFromCOLORREF(RGB(r,g,b));
    update();
  }

  void Slider::setKnobColor(int r,int g,int b){
     
     this->knobColor.SetFromCOLORREF(RGB(r,g,b));
    update();
  }

  void Slider::setBackgroundColor(int r,int g,int b,int a){
     this->backgroundColor.SetValue(Color::MakeARGB(a,r,g,b));
     update();
  }

  void Slider::setKnobColor(int r,int g,int b,int a){
     this->knobColor.SetValue(Color::MakeARGB(a,r,g,b));
     update();
  }

  void Slider::setMin(const float& value){
     this->min = value;
  }

  void Slider::setMax(const float& value){
     this->max = value;
  }

  void Slider::changePosition(int x,int y){
    
    this->xPos = x;
    this->yPos = y;
    SetWindowPos(this->handle,NULL,this->xPos,this->yPos,0,0,SWP_NOSIZE);
    fullUpdate();
  }

  void Slider::changeSize(int width,int height){
    this->xSize = width;
    this->ySize = height;
    SetWindowPos(this->handle,NULL,0,0,this->xSize,this->ySize,SWP_NOMOVE);
    fullUpdate();
  }

  void Slider::show(){
    ShowWindow(this->handle,SW_SHOW);
  }

  void Slider::hide(){
    ShowWindow(this->handle,SW_HIDE);
  }

  void Slider::disable(){
    this->disabled = true;
    update();
  }

  void Slider::enable(){
    this->disabled = false;
    update();
  }

  void Slider::enableHint(){
    this->isHint = true;
  }

  void Slider::disableHint(){
    this->isHint = false;
  }
