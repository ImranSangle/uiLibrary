
#include <button.h>
#include <thread>
#include <iostream>
#include <windef.h>
#include <wingdi.h>

  void Button::registerMouseCapure(HWND hwnd){
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);

  }

  void Button::getParentBitmap(){
     
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

  void Button::update(){
     InvalidateRect(this->handle,NULL,TRUE);
  }

  void Button::updateParent(){

    RECT rt;
    rt.top = this->yPos;
    rt.left = this->xPos;
    rt.right = this->xSize+this->xPos;
    rt.bottom = this->ySize+this->yPos;
    InvalidateRect(this->parent,&rt,TRUE);
  } 

  void Button::fullUpdate(){
    
     updateParent();
     this->gotParentBitmap = false;
     update();
  }

  void Button::paint(HWND hwnd){      
     PAINTSTRUCT ps;
     HDC dc = BeginPaint(hwnd, &ps);
     HDC memoryDc = CreateCompatibleDC(dc);
     HBITMAP memoryBitmap = CreateCompatibleBitmap(dc,this->xSize,this->ySize);
     HBITMAP oldBitmap = (HBITMAP) SelectObject(memoryDc,memoryBitmap);

     Graphics graphics(memoryDc);

     graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

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

     Font font(this->font.c_str(),this->textSize);
     
     SolidBrush brush(this->textColor);

     StringFormat format;
     
     format.SetAlignment(StringAlignmentCenter);
     
     format.SetLineAlignment(StringAlignmentCenter);

     graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

     graphics.DrawString(this->text.c_str(),-1,&font,rect,&format,&brush);

    if(this->hover){

      SolidBrush backbrush(Color(75,255,255,255));

      graphics.FillRectangle(&backbrush,rect);
    }
    
    if(this->pressed){
      SolidBrush backbrush(Color(100,255,255,255));

      graphics.FillRectangle(&backbrush,rect);
    }

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

  LRESULT CALLBACK Button::callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      switch(msg){
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
      break;
      case WM_LBUTTONDBLCLK:
      case WM_LBUTTONDOWN:
        if(this->pressed == false && this->disabled == false){
          this->pressed = true;
          SetFocus(hwnd);
          update();

        }else if(this->pressed == true && this->latchButton == true && this->disabled == false){
          this->pressed = false;
          SetFocus(hwnd);
          update();
        }
      break;
      case WM_LBUTTONUP:
        if(this->pressed == true && this->latchButton == false && this->disabled == false){
          this->pressed = false;
          update();
          if(this->onClick != nullptr){
            onClick(this);
          }
        }
      break;
      case WM_MOUSEMOVE:
        registerMouseCapure(hwnd);
        if(this->hover == false && this->disabled == false){
          this->hover = true;
          std::thread worker(Button::hoverAnimation,this);
          worker.detach();
          // updateButton();
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
  }

  Button::~Button(){
     DestroyWindow(this->parent);
     std::cout<<"button destroyed"<<std::endl;
  }

  int Button::getX(){
     return this->xPos;
  }

  int Button::getY(){
     return this->yPos;
  }

  int Button::getWidth(){
     return this->xSize;
  }

  int Button::getHeight(){
     return this->ySize;
  }

  void Button::setParent(HWND parent){
      this->parent = parent;
      this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

      SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
      SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)Button::callbackProcedure);

      std::cout<<"button created"<<std::endl;
  }

  void Button::setText(const std::wstring& name){
     this->text = name; 
    update();
  }

  std::wstring Button::getText(){
     return this->text;
  }

  void Button::setTextColor(int r,int g,int b){
     
     this->textColor.SetFromCOLORREF(RGB(r,g,b));
    update();
  }

  void Button::setTextSize(int size){
    this->textSize = size;
    update();
  }

  void Button::setBackgroundColor(int r,int g,int b,int a){
     
    this->backgroundColor.SetValue(Color::MakeARGB(a,r, g, b));
    update();
  }

  void Button::setBackgroundColor(int r,int g,int b){
    
    this->backgroundColor.SetFromCOLORREF(RGB(r,g,b));
    update();

  }

  void Button::setBackgroundImage(const std::wstring& path){
     this->backgroundImage = path;
    update();
  }

  void Button::setFont(const std::wstring& fontname){
    this->font = fontname;
    update();
  }

  void Button::changePosition(int x,int y){
    
    this->xPos = x;
    this->yPos = y;
    SetWindowPos(this->handle,NULL,this->xPos,this->yPos,0,0,SWP_NOSIZE);
    fullUpdate();
  }

  void Button::disable(){
    this->disabled = true;
    update();
  }

  void Button::enable(){
    this->disabled = false;
    update();
  }

  bool Button::buttonState(){
    return this->pressed;
  }
