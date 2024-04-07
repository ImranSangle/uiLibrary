
#include <static_element.h>
#include <iostream>

  void StaticElement::registerMouseCapure(HWND hwnd){
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);

  }

  void StaticElement::getParentBitmap(){
     
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

  void StaticElement::update(){
     InvalidateRect(this->handle,NULL,TRUE);
  }

  void StaticElement::updateParent(){

    RECT rt;
    rt.top = this->yPos;
    rt.left = this->xPos;
    rt.right = this->xSize+this->xPos;
    rt.bottom = this->ySize+this->yPos;
    InvalidateRect(this->parent,&rt,TRUE);
  } 

  void StaticElement::fullUpdate(){
    
     updateParent();
     this->gotParentBitmap = false;
     update();
  }

  void StaticElement::paint(HWND hwnd){      
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

     Font font(this->font.c_str(),this->textSize);
 
     SolidBrush brush(this->textColor);

     StringFormat format;
     
     format.SetLineAlignment(StringAlignmentCenter);

     graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
  
     graphics.DrawString(this->text.c_str(),-1,&font,rect,&format,&brush);


    if(this->disabled){
      SolidBrush backbrush(Color(128,0,0,0));

      graphics.FillRectangle(&backbrush,rect);
    }
     
     if(this->autoResize){
       RectF mesureRect;
       RectF paddingRect;
       graphics.MeasureString(this->text.c_str(),-1,&font,PointF(0,0),&mesureRect);
       graphics.MeasureString(L"A",-1,&font,PointF(0,0),&paddingRect);
       this->xSize = mesureRect.Width+(paddingRect.Width/10);
       this->ySize = mesureRect.Height;
       SetWindowPos(this->handle,NULL,this->xPos,this->yPos,this->xSize,this->ySize,SWP_NOZORDER);
       if(this->backgroundColor.GetA() < 255){
         updateParent();
         this->gotParentBitmap = false;
       }
       update();
       this->autoResize = false;
     }
  
     BitBlt(dc,0,0,this->xSize,this->ySize,memoryDc,0,0,SRCCOPY);

     SelectObject(memoryDc,oldBitmap);
     DeleteObject(memoryBitmap);
     DeleteDC(memoryDc);
     EndPaint(hwnd,&ps);
  }

  LRESULT CALLBACK StaticElement::callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      switch(msg){
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
      break;
      case WM_LBUTTONDBLCLK:
      case WM_LBUTTONDOWN:
        if(this->pressed == false && this->disabled == false){
          this->pressed = true;
          SetFocus(hwnd);
          if(this->onClick != nullptr){
            onClick(this);
          }
        }
      break;
      case WM_LBUTTONUP:
        if(this->pressed == true  && this->disabled == false){
          this->pressed = false;
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
  

  StaticElement::StaticElement(HWND hwnd,int x,int y,int cx,int cy){
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

  StaticElement::~StaticElement(){
     DestroyWindow(this->parent);
     std::cout<<"StaticElement destroyed"<<std::endl;
  }

  int StaticElement::getX(){
     return this->xPos;
  }

  int StaticElement::getY(){
     return this->yPos;
  }

  int StaticElement::getWidth(){
     return this->xSize;
  }

  int StaticElement::getHeight(){
     return this->ySize;
  }

  void StaticElement::setParent(HWND parent){
      this->parent = parent;
      this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

      SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
      SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)StaticElement::callbackProcedure);

      std::cout<<"StaticElement created"<<std::endl;
  }

  void StaticElement::setText(const std::wstring& name){
    this->text = name; 
    this->autoResize = true; 
    update();
  }

  std::wstring StaticElement::getText(){
     return this->text;
  }

  void StaticElement::setTextColor(int r,int g,int b){
     
     this->textColor.SetFromCOLORREF(RGB(r,g,b));
    update();
  }

  void StaticElement::setTextSize(int size){
    this->textSize = size;
    update();
  }

  void StaticElement::setBackgroundColor(int r,int g,int b,int a){
     
    this->backgroundColor.SetValue(Color::MakeARGB(a,r, g, b));
    update();
  }

  void StaticElement::setBackgroundColor(int r,int g,int b){
    
    this->backgroundColor.SetFromCOLORREF(RGB(r,g,b));
    update();

  }

  void StaticElement::setBackgroundImage(const wchar_t* path){
     this->backgroundImage = path;
    update();
  }

  void StaticElement::setFont(const wchar_t* fontname){
    this->font = fontname;
    update();
  }

  void StaticElement::changePosition(int x,int y){
    
    this->xPos = x;
    this->yPos = y;
    SetWindowPos(this->handle,NULL,this->xPos,this->yPos,0,0,SWP_NOSIZE);
    if(this->backgroundColor.GetA() < 255){
      fullUpdate();
    }
  }

  void StaticElement::disable(){
    this->disabled = true;
    update();
  }

  void StaticElement::enable(){
    this->disabled = false;
    update();
  }
