
#include <iostream>

#include <frame.h>

  void Frame::updateFrame(){
     InvalidateRect(this->handle,NULL,TRUE);
  }

  void Frame::paint(HWND hwnd){
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(hwnd,&ps);

    Graphics graphics(dc);

    RectF rect = {0,0,(float)this->xSize,(float)this->ySize};

    if(this->backgroundImage.length() > 0){

      Bitmap bitmap(this->backgroundImage.c_str());

      graphics.DrawImage(&bitmap,0,0,this->xSize,this->ySize);
       
    }else{

      SolidBrush backbrush(this->backgroundColor);
      
      graphics.FillRectangle(&backbrush,rect);

    }

    EndPaint(hwnd,&ps);

  }

 
  LRESULT CALLBACK Frame::frameProcedureImplementation(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
    switch(msg){
      case WM_NCHITTEST:
        {
            POINT pt;
             pt.x = GET_X_LPARAM(lp);
             pt.y = GET_Y_LPARAM(lp);
            if(WindowFromPoint(pt) == hwnd){
               return HTCAPTION;
            }else{
               return DefWindowProcW(hwnd,msg,wp,lp);
            }
        }
      break;
      case WM_LBUTTONDOWN:
        SetFocus(hwnd);
      break;
      case WM_USER:
        if(wp == true){
          this->isFocused = false;
        }
      break;
      case WM_SETFOCUS:
        SendMessageW((HWND)wp,WM_USER,true,0);
        if(onFocus != nullptr){
          onFocus(this);
        }
      break;
      case WM_SIZE:
      break;
      case WM_PAINT:
        this->paint(hwnd);
      break;
      case WM_ERASEBKGND:
      break;
      case WM_CREATE:
      break;
      case WM_DESTROY:
        PostQuitMessage(0);
      break;
      default:
        return DefWindowProcW(hwnd,msg,wp,lp);
    }
    return (LRESULT) nullptr;
  }

  Frame::Frame(const std::wstring& name,int x,int y,int cx,int cy,bool titlebar){
    this->xPos = x;
    this->yPos = y;
    this->xSize = cx;
    this->ySize = cy;
    this->hasTitlebar = titlebar;

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&(this->gdiplusToken),&gdiplusStartupInput,NULL);

    WNDCLASSW wc = {0};
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.lpszClassName = name.c_str();
    wc.lpfnWndProc = frameProcedure;

    if(!RegisterClassW(&wc)){
      std::cout<<"Failed to Register class closing.."<<std::endl;
      return;
    }

    DWORD windowStyles;

    if(hasTitlebar){
      windowStyles = WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE | WS_OVERLAPPEDWINDOW;
    }else{
      windowStyles = WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE | WS_POPUP ;
    }

    this->handle = CreateWindowExW(0,name.c_str(),L"mainWindow",windowStyles,this->xPos,this->yPos,this->xSize,this->ySize,NULL,NULL,NULL,NULL);

    SetWindowLongPtr(this->handle,GWLP_USERDATA,(LONG_PTR)this);
  }

  Frame::~Frame(){
      DestroyWindow(this->handle);
     
      GdiplusShutdown(this->gdiplusToken);
  }


  void Frame::start(){
    MSG msg;
    while(GetMessageW(&msg,0,0,0)){
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  void Frame::add(Element* element){
      element->setParent(this->handle);
  }

  void Frame::setBackgroundColor(int r,int g,int b){
     
     this->backgroundColor.SetFromCOLORREF(RGB(r,g,b));
    updateFrame();
  }

  void Frame::setBackgroundImage(const wchar_t* path){
     this->backgroundImage = path;
    updateFrame();
  }
  
  HWND Frame::getHandle(){
     return this->handle;
  }

  int Frame::getWidth(){
     return this->xSize;
  }

  int Frame::getHeight(){
     return this->ySize;
  }
