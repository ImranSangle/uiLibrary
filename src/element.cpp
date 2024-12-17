#include "log.h"
#include <element.h>
#include <windows.h>

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
};

void Element::setParent(const HWND& parent){
     this->parent = parent;
     this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

     SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
     SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)Element::callbackProcedure);

     LOG("Element Created");
};

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
