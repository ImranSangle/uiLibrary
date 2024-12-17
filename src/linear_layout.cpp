
#include <linear_layout.h>
#include <iostream>
#include <windef.h>
#include <wingdi.h>
#include <thread>

  void LinearLayout::registerMouseCapure(HWND hwnd){
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);

  }

  void LinearLayout::getParentBitmap(){
     
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

  void LinearLayout::update(){
     InvalidateRect(this->handle,NULL,TRUE);
  }

  void LinearLayout::updateParent(){

    RECT rt;
    rt.top = this->yPos;
    rt.left = this->xPos;
    rt.right = this->xSize+this->xPos;
    rt.bottom = this->ySize+this->yPos;
    InvalidateRect(this->parent,&rt,TRUE);
  } 

  void LinearLayout::fullUpdate(){
    
     updateParent();
     this->gotParentBitmap = false;
     update();
  }

  void LinearLayout::updateScrollbar(){
     
     HDC dc = GetDC(this->handle);

     Graphics graphics(dc);

     SolidBrush scrollbarBrush(Color(250,250,250));
     SolidBrush backgroundBrush(Color(0,0,0));
     graphics.FillRectangle(&backgroundBrush,this->xSize-6,0,6,this->ySize);
     graphics.FillRectangle(&scrollbarBrush,this->xSize-5,(int)this->scrollbarYPosition,4,(int)(this->ySize/(this->yLength/this->ySize)));

     ReleaseDC(this->handle,dc);
  }

  void LinearLayout::scroll(WPARAM wp,int velocity){

         if((SHORT)HIWORD(wp) > 0){
           if(this->scrollbarYPosition > 0){
             for(int i=0;i<this->childs.size();i++){
                this->childs[i]->changePosition(this->childs[i]->getX(),this->childs[i]->getY()+velocity);
             }
             this->scrollbarYPosition-= this->ySize/(this->yLength/velocity);
             updateScrollbar();
           }
         }else{
           if(this->scrollbarYPosition+this->ySize/(this->yLength/this->ySize) <= this->ySize){
             for(int i=0;i<this->childs.size();i++){
                this->childs[i]->changePosition(this->childs[i]->getX(),this->childs[i]->getY()-velocity);
             }
             this->scrollbarYPosition+= this->ySize/(this->yLength/velocity); 
             updateScrollbar();
           }
         }
  }

  void LinearLayout::paint(HWND hwnd){      
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

  LRESULT CALLBACK LinearLayout::callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      switch(msg){
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
      break;
      case WM_MOUSEWHEEL:
         if(this->scrollVelocity == 0){
           std::thread worker(LinearLayout::scrollWorker,this);
           worker.detach();
         }
         if(wp != this->scrollDirection){
           this->scrollVelocity = 4;
         }else{
           this->scrollVelocity+=4;
         }
           this->scrollDirection = wp;
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
      case WM_SIZE:
        {
          if(this->onSize != nullptr){
             this->onSize(this);
          }
          for(int i =0;i<this->childs.size();i++){
             this->childs[i]->update();
          }
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
  

  LinearLayout::LinearLayout(HWND hwnd,int x,int y,int cx,int cy){
     this->xPos = x;
     this->yPos = y;
     this->xSize = cx;
     this->ySize = cy;
     this->parent = hwnd;
     this->backgroundColor.SetFromCOLORREF(RGB(30,30,30));
  }

  LinearLayout::~LinearLayout(){
     DestroyWindow(this->handle);
     std::cout<<"LinearLayout destroyed"<<std::endl;
  }

  int LinearLayout::getX(){
     return this->xPos;
  }

  int LinearLayout::getY(){
     return this->yPos;
  }

  int LinearLayout::getWidth(){
     return this->xSize;
  }

  int LinearLayout::getHeight(){
     return this->ySize;
  }

  std::vector<Element*> LinearLayout::getChilds(){
     return this->childs; 
  }

  void LinearLayout::setParent(HWND parent){
      this->parent = parent;
      this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,NULL,NULL,NULL);

      SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
      SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)LinearLayout::callbackProcedure);

      std::cout<<"LinearLayout created"<<std::endl;
  }

  void LinearLayout::add(Element* element){
      element->setParent(this->handle);

      this->childs.push_back(element);
      element->changePosition(element->getX(),this->yLength);
      this->yLength+= element->getHeight()+this->padding;
  }

  void LinearLayout::setBackgroundColor(int r,int g,int b,int a){
     
    this->backgroundColor.SetValue(Color::MakeARGB(a,r, g, b));
    update();
  }

  void LinearLayout::setBackgroundColor(int r,int g,int b){
    
    this->backgroundColor.SetFromCOLORREF(RGB(r,g,b));
    update();

  }

  void LinearLayout::setBackgroundImage(const std::wstring& path){
     this->backgroundImage = path;
    update();
  }

  void LinearLayout::setPadding(int paddingAmount){
    this->padding = paddingAmount;
  }

  void LinearLayout::changePosition(int x,int y){
    
    this->xPos = x;
    this->yPos = y;
    SetWindowPos(this->handle,NULL,this->xPos,this->yPos,0,0,SWP_NOSIZE);
    if(this->backgroundColor.GetA() < 255){
      fullUpdate();
    }
  }

  void LinearLayout::changeSize(int width,int height){
    this->xSize = width;
    this->ySize = height;
    SetWindowPos(this->handle,NULL,0,0,this->xSize,this->ySize,SWP_NOMOVE);
    fullUpdate();
  }

  void LinearLayout::show(){
    ShowWindow(this->handle,SW_SHOW);
  }

  void LinearLayout::hide(){
    ShowWindow(this->handle,SW_HIDE);
  }

  void LinearLayout::disable(){
    this->disabled = true;
    update();
  }

  void LinearLayout::enable(){
    this->disabled = false;
    update();
  }
