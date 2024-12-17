#pragma once

#include <windows.h>

class Element{
  protected:
  int xPos;
  int yPos;
  int xSize;
  int ySize;
  bool disabled = false;
  HWND parent;
  HWND handle;
  bool gotParentBitmap = false;
  HBITMAP parentBitmap = NULL;

  virtual void updateParent() const;

  virtual LRESULT CALLBACK callbackProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp);
  
  static LRESULT CALLBACK callbackProcedure(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      Element* element = (Element*)(GetWindowLongPtr(hwnd,GWLP_USERDATA));

      if(element != nullptr){
         return element->callbackProcedureImplementation(hwnd, msg, wp, lp);
      }else{
         return DefWindowProcW(hwnd, msg, wp, lp);
      }
  }

  public:

  virtual void update() const;

  virtual void fullUpdate();

  virtual void setParent(const HWND& parent);

  virtual void changePosition(const int& x,const int& y);
  
  virtual void changeSize(const int& width,const int& height);

  virtual int getX() const;

  virtual int getY() const;

  virtual int getWidth() const;

  virtual int getHeight() const;

  virtual void show() const;

  virtual void hide() const;

  virtual void disable();

  virtual void enable();

  virtual ~Element();
};
