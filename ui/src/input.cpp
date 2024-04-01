  #include <input.h>
  #include <thread>
  #include <iostream>

  void InputBox::registerMouseCapure(HWND hwnd){
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);

  }

  void InputBox::getParentBitmap(){
     
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

  void InputBox::update(){
     InvalidateRect(this->handle,NULL,TRUE);
  }

  void InputBox::updateParent(){

    RECT rt;
    rt.top = this->yPos;
    rt.left = this->xPos;
    rt.right = this->xSize+this->xPos;
    rt.bottom = this->ySize+this->yPos;
    InvalidateRect(this->parent,&rt,TRUE);
  } 

  void InputBox::handleKeyboardEvents(HWND hwnd,WPARAM wp){
        BYTE keyboardState[256];
        GetKeyboardState(keyboardState);

        BYTE asciiResult[2] = { 0 };
        UINT result = ToAscii((UINT)(wp), 0, keyboardState,(LPWORD)asciiResult, 0);

        if(wp == VK_BACK){
          if(this->text.length() > 0){
            this->text.pop_back();
          }
          update();
          if(onInput != nullptr){
             onInput(this);
          }
        }else if(asciiResult[0] > 31 && stringLock == false){ 
          this->text.push_back(asciiResult[0]);
          update();
          if(onInput != nullptr){
             onInput(this);
          }
        }
  }

  void InputBox::paint(HWND hwnd){      
     PAINTSTRUCT ps;
     HDC dc = BeginPaint(hwnd, &ps);
     HDC memoryDc = CreateCompatibleDC(dc);
     HBITMAP memoryBitmap = CreateCompatibleBitmap(dc,this->xSize,this->ySize);
     HBITMAP oldBitmap =(HBITMAP)SelectObject(memoryDc, memoryBitmap);

     Graphics Graphics(memoryDc);

     Bitmap bitmap(this->parentBitmap,NULL);

     Graphics.DrawImage(&bitmap,0,0,this->xSize,this->ySize);

     RectF rect = {0,0,(float)this->xSize,(float)this->ySize};

    if(this->backgroundImage.length() > 0){

      Bitmap bitmap(this->backgroundImage.c_str());

      Graphics.DrawImage(&bitmap,0,0,this->xSize,this->ySize);
       
    }else{

      SolidBrush backbrush(this->backgroundColor);
      
      Graphics.FillRectangle(&backbrush,rect);

    }

     Font font(this->font.c_str(),this->textSize);
     
     SolidBrush brush(this->textColor);

     StringFormat format(StringFormatFlagsLineLimit);
     
     if(this->alignCenter){
       format.SetAlignment(StringAlignmentCenter);
    }
     
     format.SetLineAlignment(StringAlignmentCenter);

      if(this->text.length() == 0){
       SolidBrush hintBrush(Color(128,255,255,255));
       Graphics.DrawString(this->hint.c_str(),-1,&font,rect,&format,&hintBrush);
      }

      RectF stringRect;

      if(this->hidden == true){
        std::wstring hiddenstring(this->text.length(),L'*');
        Graphics.DrawString(hiddenstring.c_str(),-1,&font,rect,&format,&brush);
        Graphics.MeasureString(hiddenstring.c_str(),-1,&font,PointF(0,0),&stringRect);
      }else{
        Graphics.DrawString(this->text.c_str(),-1,&font,rect,&format,&brush);
        Graphics.MeasureString(this->text.c_str(),-1,&font,PointF(0,0),&stringRect);
      }


      if(this->isFocused){
        if(this->alignCenter){
          int caretCenterAlignPosition = (this->xSize/2.0f)+stringRect.Width/2.0f;

          if(caretCenterAlignPosition < this->xSize-3){
            stringLock = false;
            SetCaretPos(caretCenterAlignPosition,8);
          }else{
            stringLock = true; 
          }
        
        }else{
          int caretLeftAlignPosition = stringRect.Width;
          
          if(caretLeftAlignPosition < this->xSize-3){
            stringLock = false;
            SetCaretPos(caretLeftAlignPosition+2,8);
          }else{
            stringLock = true;
          }
        }
      
      }
      

    if(this->hover){

      SolidBrush backbrush(Color(50,255,255,255));

      Graphics.FillRectangle(&backbrush,rect);
    }
     
    if(this->isFocused){

      Pen pen(Color(255,255,255),2);
      Rect strokeRect = {0,0,this->xSize,this->ySize};

      Graphics.DrawRectangle(&pen,strokeRect);
    }


    if(this->disabled){
      SolidBrush disableBrush(Color(128,0,0,0));
       
      Graphics.FillRectangle(&disableBrush,rect);
    }
     
     BitBlt(dc,0,0,this->xSize,this->ySize,memoryDc,0,0,SRCCOPY);
     SelectObject(memoryDc,oldBitmap);
     DeleteObject(memoryBitmap);
     DeleteDC(memoryDc);
     EndPaint(hwnd,&ps);
  }

  LRESULT CALLBACK InputBox::inputProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
      switch(msg){
      case WM_USER:
        if(wp == true){
          this->isFocused = false;
          update();
          DestroyCaret();
        }
      break;
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
        CreateCaret(this->handle,NULL,1,this->ySize-16);
        SetCaretPos(this->xSize/2,8);
        ShowCaret(this->handle);
        if(onFocus != nullptr){
          onFocus(this);
        }
      break;
      case WM_KEYDOWN:
        if(this->disabled == false){
          handleKeyboardEvents(hwnd,wp);
        }
      break;
      case WM_LBUTTONDOWN:
        // if(this->isFocused == false){
          if(this->disabled == false){
            this->isFocused = true;
            SetFocus(hwnd);
            update();
          }
        // }
        // else if(this->pressed == true && this->latchInputBox == true){
        //   this->pressed = false;
        //   update();
        // }
      break;
      // case WM_LBUTTONUP:
      //   if(this->pressed == true && this->latchInputBox == false){
      //     this->pressed = false;
      //     update();
      //     if(this->onClick != nullptr){
      //       onClick(this);
      //     }
      //   }
      // break;
      case WM_MOUSEMOVE:
        registerMouseCapure(hwnd);
        if(this->hover == false && this->disabled == false){
          this->hover = true;
          std::thread worker(InputBox::hoverAnimation,this);
          worker.detach();
          // update();
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
  

  InputBox::InputBox(HWND hwnd,int x,int y,int cx,int cy){
     this->xPos = x;
     this->yPos = y;
     this->xSize = cx;
     this->ySize = cy;
     this->parent = hwnd;
     this->backgroundColor.SetFromCOLORREF(RGB(0,0,0));
     this->textColor.SetFromCOLORREF(RGB(255,255,255));
     this->font = L"Roboto";
     this->textSize = 12;

  }

  InputBox::~InputBox(){
     DestroyWindow(this->parent);
     std::cout<<"inputbox destroyed"<<std::endl;
  }

  void InputBox::setParent(HWND parent){
     this->parent = parent;
     this->handle = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"",WS_VISIBLE | WS_CHILD,this->xPos,this->yPos,this->xSize,this->ySize,this->parent,(HMENU)2,NULL,NULL);

     SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
     SetWindowLongPtr(this->handle,GWLP_WNDPROC,(LONG_PTR)InputBox::inputProcedure);

     std::cout<<"inputbox created"<<std::endl;
  }

  void InputBox::setText(const wchar_t* name){
     this->text = name; 
    update();
  }
  
  std::wstring InputBox::getText(){
      return this->text;
  }

  void InputBox::setHint(const wchar_t* hint){
    this->hint = hint;
    update();
  }

  void InputBox::setTextColor(int r,int g,int b){
     
     this->textColor.SetFromCOLORREF(RGB(r,g,b));
    update();
  }

  void InputBox::setTextSize(int size){
    this->textSize = size;
    update();
  }

  void InputBox::setBackgroundColor(int r,int g,int b,int a){

    this->backgroundColor.SetValue(Color::MakeARGB(a, r, g, b));
    update();
  }

  void InputBox::setBackgroundColor(int r,int g,int b){

     this->backgroundColor.SetFromCOLORREF(RGB(r,g,b));
     update();
  }

  void InputBox::setBackgroundImage(const wchar_t* path){
     this->backgroundImage = path;
    update();
  }

  void InputBox::setFont(const wchar_t* fontname){
    this->font = fontname;
    update();
  }

  void InputBox::changePosition(int x,int y){
    
    this->xPos = x;
    this->yPos = y;
    SetWindowPos(this->handle,NULL,this->xPos,this->yPos,0,0,SWP_NOSIZE);
    updateParent();
    this->gotParentBitmap = false;
    update();
  }
  
  void InputBox::disable(){
    this->disabled = true;
    update();
  }

  void InputBox::enable(){
    this->disabled = false;
    update();
  }

  bool InputBox::isInputFocused(){
    return this->isFocused;
  }
