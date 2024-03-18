#include <iostream>
#include <uilibrary.h>

InputBox* username = new InputBox(NULL,200,150,200,30);
Frame* frame = new Frame(100,100,600,400);

LPWSTR openfile(HWND hwnd,bool save){   
    wchar_t file[100];

    OPENFILENAMEW ofn;
    ZeroMemory(&ofn,sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = file;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = L"AllFiles\0*.*\0textfile\0*.txt\0";
    ofn.nFilterIndex = 1;
    
    if(save){
     GetSaveFileNameW(&ofn);
      return ofn.lpstrFile;
    }else{
      GetOpenFileNameW(&ofn);
      return ofn.lpstrFile;
    }
}

void buttonClick(Button* button){
   
  PostQuitMessage(0);
}

void dotclick(Button* button){

  username->setText(openfile(frame->getHandle(),false));
}


int main(){

  frame->setBackgroundImage(L"../assets/server.jpg");

  frame->hasTitlebar = false;

  Button* signin = new Button(NULL,200,270,80,25);
  signin->setText(L"SIGN IN");
  signin->setBackgroundColor(34,108,224);
  signin->setFont(L"Roboto");
  signin->setTextSize(10);
  // signin->onClick = pressedcallback;

  Button* signin1 = new Button(NULL,320,270,80,25);
  signin1->setText(L"CANCEL");
  signin1->setBackgroundColor(34,108,224);
  signin1->setFont(L"Roboto");
  signin1->setTextSize(10);
  signin1->onClick = buttonClick;

  username->setFont(L"Roboto");
  username->alignCenter = true;
  username->setBackgroundColor(0,0,0);
  username->setHint(L"username");
  username->setTextSize(10);

  Button* pathlocator = new Button(NULL,410,150,30,30);
  pathlocator->setText(L"...");
  pathlocator->setTextColor(255, 255, 255);
  pathlocator->setBackgroundColor(34, 108 , 224);
  pathlocator->onClick = dotclick;

  InputBox* input = new InputBox(NULL,200,195,200,30);
  input->setFont(L"Roboto");
  input->alignCenter = true;
  input->setTextSize(10);
  input->setBackgroundColor(0,0,0);
  input->setHint(L"password");
  input->hidden = true;
  
  frame->add(signin);
  frame->add(signin1);
  frame->add(username);
  frame->add(pathlocator);
  frame->add(input);

  frame->start();

  return 0;
}
