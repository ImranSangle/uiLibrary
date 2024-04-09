
#include <progressbar.h>
#include <thread>
#include <iostream>
#include <windef.h>
#include <wingdi.h>

  void Progressbar::registerMouseCapure(HWND hwnd){
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);

  }

  void Progressbar::getParentBitmap(){
     
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

  void Progressbar::update(){
     InvalidateRect(this->handle,NULL,TRUE);
  }

  void Progressbar::updateParent(){

    RECT rt;
    rt.top = this->yPos;
    rt.left = this->xPos;
    rt.right = this->xSize+this->xPos;
    rt.bottom = this->ySize+this->yPos;
    InvalidateRect(this->parent,&rt,TRUE);
  } 

  void Progressbar::fullUpdate(){
    
     updateParent();
     this->gotParentBitmap = false;
     update();
  }

  float Progressbar::remap(const float& value,const float& a,const float& b,const float& c,const float& d){
      
      float t = (value-a)/(b-a); 

      return (c*(1.0f-t))+(d*t);
  }

  void Progressbar::paint(HWND hwnd){      
     PAINTSTRUCT ps;
     HDC dc = BeginPaint(hwnd, &ps);
     HDC memoryDc = CreateCompatibleDC(dc);
     HBITMAP memoryBitmap = CreateCompatibleBitmap(dc,this->xSize,this->ySize);
     HBITMAP oldBitmap = (HBITMAP) SelectObject(memoryDc,memoryBitmap);

     Graphics graphics(memoryDc);

     Bitmap bitmap(this->parentBitmap,NULL);

     graphics.DrawImage(&bitmap,0,0,this->xSize,this->ySize);
  
     RectF rect = {0,0,(float)this->xSize,(float)this->ySize};

    if(this->backgroundImage.length() > 0){

      Bitmap bitmap(this->backgroundImage.c_str());

      graphics.DrawImage(&bitmap,0,0,this->xSize,this->ySize);
       
    }else{

      SolidBrush backbrush(this->backgroundColor);
      
      graphics.FillRectangle(&backbrush,rect);

    }
      RectF progressRect;

      if(this->vertical){
        if(this->reversed){
          progressRect = {0,0,rect.Width,remap(this->progress,this->min,this->max,0,rect.Height)};
        }else{
          progressRect = {0,remap(this->progress,this->min,this->max,rect.Height,0),rect.Width,rect.Height};
        }
      }else{
        if(this->reversed){
          progressRect = {remap(this->progress,this->min,this->max,this->xSize,0),0,rect.Width,rect.Height};
        }else{
          progressRect = {0,0,remap(this->progress,this->min,this->max,0,this->xSize),rect.Height};
        }
      }

      SolidBrush progressBrush(this->barColor);

      graphics.FillRectangle(&progressBrush,progressRect);
    
    if(this->disabled){
      SolidBrush backbrush(Color(128,0,0,0));

      graphics.FillRectangle(&backbrush,rect);
    }

     BitBlt(dc,0,0,this->xSize,this->ySize,memoryDc,0,0,SRCCOPY);

     SelectObject(memoryDc,oldBitmap);
     DeleteObject(memoryBitmap);
     DeleteDC(memoryDc);
     EndPaint(hwnd,&ps);
  }

  LRESULT CALLBACK Progressbar::callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      switch(msg){
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
      break;
      case WM_LBUTTONDBLCLK:
      case WM_LBUTTONDOWN:
        if(this->pressed == false && this->disabled == false){
          this->pressed = true;
          SetFocus(hwnd);
          }
      break;
      case WM_LBUTTONUP:
        if(this->pressed == true && this->disabled == false){
          this->pressed = false;
          if(this->onClick != nullptr){
            onClick(this);
          }
        }
      break;
      case WM_MOUSEMOVE:
        if(this->hover == false && this->disabled == false){
          registerMouseCapure(hwnd);
          this->hover = true;
        }
      break;
      case WM_MOUSELEAVE:
        if(this->hover == true && this->disabled == false){
          this->hover = false;
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
  

  Progressbar::Progressbar(HWND hwnd,int x,int y,int cx,int cy){
     this->xPos = x;
     this->yPos = y;
     this->xSize = cx;
     this->ySize = cy;
     this->parent = hwnd;
     this->backgroundColor.SetFromCOLORREF(RGB(100,100,100));
  }

  Progressbar::~Progressbar(){
     DestroyWindow(this->parent);
     std::cout<<"Progressbar destroyed"<<std::endl;
  }

  int Progressbar::getX(){
     return this->xPos;
  }

  int Progressbar::getY(){
     return this->yPos;
  }

  int Progressbar::getWidth(){
     return this->xSize;
  }

  int Progressbar::getHeight(){
     return this->ySize;
  }

  float Progressbar::getMin(){
     return this->min;
  }

  float Progressbar::getMax(){
     return this->max;
  }

  float Progressbar::getProgress(){
     return this->progress;
  }

  void Progressbar::setParent(HWND parent){
      this->parent = parent;
      this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

      SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
      SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)Progressbar::callbackProcedure);

      std::cout<<"button created"<<std::endl;
  }

  void Progressbar::setBackgroundColor(int r,int g,int b,int a){
     
    this->backgroundColor.SetValue(Color::MakeARGB(a,r, g, b));
    update();
  }

  void Progressbar::setBackgroundColor(int r,int g,int b){
    
    this->backgroundColor.SetFromCOLORREF(RGB(r,g,b));
    update();

  }

  void Progressbar::setBackgroundImage(const std::wstring& path){
     this->backgroundImage = path;
    update();
  }

  void Progressbar::setBarColor(int r,int g,int b,int a){
     this->barColor.SetValue(Color::MakeARGB(r, g, b, a));
     update();
  }

  void Progressbar::setBarColor(int r,int g,int b){
     this->barColor.SetFromCOLORREF(RGB(r,g,b));
     update();
  }

  void Progressbar::setMin(const float& value){
     this->min = value;
  }

  void Progressbar::setMax(const float& value){
     this->max = value;
  }

  void Progressbar::setProgress(const float& value){
     if(value > this->max){
        this->progress = this->max;
        update();
     }else if(value < this->min){
        this->progress = this->min;
        update();
     }else{
        this->progress = value;
        update();
     }
  }

  void Progressbar::changePosition(int x,int y){
    
    this->xPos = x;
    this->yPos = y;
    SetWindowPos(this->handle,NULL,this->xPos,this->yPos,0,0,SWP_NOSIZE);
    fullUpdate();
  }

  void Progressbar::changeSize(int width,int height){
    this->xSize = width;
    this->ySize = height;
    SetWindowPos(this->handle,NULL,0,0,this->xSize,this->ySize,SWP_NOMOVE);
    fullUpdate();
  }

  void Progressbar::show(){
    ShowWindow(this->handle,SW_SHOW);
  }

  void Progressbar::hide(){
    ShowWindow(this->handle,SW_HIDE);
  }

  void Progressbar::disable(){
    this->disabled = true;
    update();
  }

  void Progressbar::enable(){
    this->disabled = false;
    update();
  }
