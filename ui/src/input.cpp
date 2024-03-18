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

  void InputBox::updateInputBox(){
     InvalidateRect(this->handle,NULL,TRUE);
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
          updateInputBox();
          if(onInput != nullptr){
             onInput(this);
          }
        }else if(asciiResult[0] > 31 && stringLock == false){ 
          this->text.push_back(asciiResult[0]);
          updateInputBox();
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
          updateInputBox();
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
        handleKeyboardEvents(hwnd,wp);
      break;
      case WM_LBUTTONDOWN:
        // if(this->isFocused == false){
          this->isFocused = true;
          SetFocus(hwnd);
          updateInputBox();
        // }
        // else if(this->pressed == true && this->latchInputBox == true){
        //   this->pressed = false;
        //   updateInputBox();
        // }
      break;
      // case WM_LBUTTONUP:
      //   if(this->pressed == true && this->latchInputBox == false){
      //     this->pressed = false;
      //     updateInputBox();
      //     if(this->onClick != nullptr){
      //       onClick(this);
      //     }
      //   }
      // break;
      case WM_MOUSEMOVE:
        registerMouseCapure(hwnd);
        if(this->hover == false){
          this->hover = true;
          std::thread worker(InputBox::hoverAnimation,this);
          worker.detach();
          // updateInputBox();
        }
      break;
      case WM_MOUSELEAVE:
        if(this->hover == true){
          this->hover = false;
          updateInputBox();
        }
      break;
      case WM_PAINT:
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
    updateInputBox();
  }
  
  std::wstring InputBox::getText(){
      return this->text;
  }

  void InputBox::setHint(const wchar_t* hint){
    this->hint = hint;
    updateInputBox();
  }

  void InputBox::setTextColor(int r,int g,int b){
     
     this->textColor.SetFromCOLORREF(RGB(r,g,b));
    updateInputBox();
  }

  void InputBox::setTextSize(int size){
    this->textSize = size;
    updateInputBox();
  }

  void InputBox::setBackgroundColor(int r,int g,int b){
     
     this->backgroundColor.SetFromCOLORREF(RGB(r,g,b));
    updateInputBox();
  }

  void InputBox::setBackgroundImage(const wchar_t* path){
     this->backgroundImage = path;
    updateInputBox();
  }

  void InputBox::setFont(const wchar_t* fontname){
    this->font = fontname;
    updateInputBox();
  }

  bool InputBox::isInputFocused(){
    return this->isFocused;
  }
