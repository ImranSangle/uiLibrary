#include "log.h"
#include <element.h>
#include <windows.h>

void Element::registerMouseCapure(const HWND& hwnd){
  TRACKMOUSEEVENT tme;
  tme.cbSize = sizeof(TRACKMOUSEEVENT);
  tme.dwFlags = TME_LEAVE;
  tme.hwndTrack = hwnd;
  TrackMouseEvent(&tme);

}

void Element::update() const {
  InvalidateRect(this->handle,NULL,TRUE);
}

void Element::updateParent() const {

  RECT rt;
  rt.top = this->yPos;
  rt.left = this->xPos;
  rt.right = this->xSize+this->xPos;
  rt.bottom = this->ySize+this->yPos;
  InvalidateRect(this->parent,&rt,TRUE);
} 

void Element::fullUpdate(){
     updateParent();
     this->gotParentBitmap = false;
     update();
}

void Element::align(const int& value){
  switch(value){
    case Element::AlignCenterInParent:
      this->alignCenterInParent = true;
      this->alignLeftInParent = false;
      this->alignRightInParent = false;
      this->alignTopInParent = false;
      this->alignBottomInParent = false;
      this->alignHorizontalCenterInParent = false;
      this->alignVerticalCenterInParent = false;
    break;
    case Element::AlignHorizontalCenterInParent:
      this->alignHorizontalCenterInParent = true;
      this->alignLeftInParent = false;
      this->alignRightInParent = false;
    break;
    case Element::AlignVerticalCenterInParent:
      this->alignVerticalCenterInParent = true;
      this->alignTopInParent = false;
      this->alignBottomInParent = false;
    break;
    case Element::AlignLeftInParent:
      this->alignLeftInParent = true;
      this->alignCenterInParent = false;
      this->alignRightInParent = false;
      this->alignHorizontalCenterInParent = false;
    break;
    case Element::AlignRightInParent:
      this->alignRightInParent = true;
      this->alignCenterInParent = false;
      this->alignLeftInParent = false;
      this->alignHorizontalCenterInParent = false;
    break;
    case Element::AlignTopInParent:
      this->alignTopInParent = true;
      this->alignCenterInParent = false;
      this->alignBottomInParent = false;
      this->alignVerticalCenterInParent = false;
    break;
    case Element::AlignBottomInParent:
      this->alignBottomInParent = true;
      this->alignCenterInParent = false;
      this->alignTopInParent = false;
      this->alignVerticalCenterInParent = false;
    break;

  }
}

void Element::alignElement(){
  if(this->matchParentWidth){
    this->xSize = this->parentPtr->xSize-(this->marginLeft+this->marginRight);
  }
  if(this->matchParentHeight){
    this->ySize = this->parentPtr->ySize-(this->marginTop+this->marginBottom);
  }
  if(this->alignLeftInParent){
    this->xPos = 0+this->marginLeft;
  }
  if(this->alignRightInParent){
    this->xPos = this->parentPtr->xSize-this->xSize-this->marginRight;
  }
  if(this->alignTopInParent){
    this->yPos = 0+this->marginTop;
  }
  if(this->alignBottomInParent){
    this->yPos = this->parentPtr->ySize-this->ySize-this->marginBottom;
  }
  if(this->alignCenterInParent || this->alignHorizontalCenterInParent){
    this->xPos = this->parentPtr->xSize/2-this->xSize/2;
  }
  if(this->alignCenterInParent || this->alignVerticalCenterInParent){
    this->yPos = this->parentPtr->ySize/2-this->ySize/2;
  }
  SetWindowPos(this->handle,NULL,this->xPos,this->yPos,this->xSize,this->ySize,SWP_SHOWWINDOW);
}

void Element::setMarginLeft(const int& value){
  this->marginLeft = value;
}

void Element::setMarginRight(const int& value){
  this->marginRight = value;
}

void Element::setMarginTop(const int& value){
  this->marginTop = value;
}

void Element::setMarginBottom(const int& value){
  this->marginBottom = value;
}

void Element::setParent(const HWND& parent){
     this->parent = parent;
     this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

     SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
     SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)Element::callbackProcedure);

     LOG("Element Created");
};

void Element::setParentPtr(Element* const ptr){
   this->parentPtr = ptr; 
   this->alignElement();
}

LRESULT CALLBACK Element::callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
   return DefWindowProcW(hwnd,msg,wp,lp);
}

void Element::changePosition(const int& x,const int& y){
    this->xPos = x;
    this->yPos = y;
    SetWindowPos(this->handle,NULL,this->xPos,this->yPos,0,0,SWP_NOSIZE);
    fullUpdate();
};
 
void Element::changeSize(const int& width,const int& height){
    this->xSize = width;
    this->ySize = height;
    SetWindowPos(this->handle,NULL,0,0,this->xSize,this->ySize,SWP_NOMOVE);
    fullUpdate();
};

Element* Element::getParentPtr() const {
  return this->parentPtr;
}

int Element::getX() const{
    return this->xPos;
};

int Element::getY() const{
    return this->yPos;
};

int Element::getWidth() const{
    return this->xSize;
};

int Element::getHeight() const{
    return this->ySize;
};

void Element::show() const{
    ShowWindow(this->handle,SW_SHOW);
}

void Element::hide() const{
    ShowWindow(this->handle,SW_HIDE);
}

void Element::disable(){
    this->disabled = true;
    update();
}

void Element::enable(){
    this->disabled = false;
    update();
}

Element::~Element(){
   DestroyWindow(this->handle);
   LOG("Element Destroyed");
}
