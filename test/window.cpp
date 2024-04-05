#include <string>
#include <uilibrary.h>
#include <thread>

Frame* frame = new Frame(100,100,600,400,false);
Button* signin = new Button(NULL,200,270,80,25);
InputBox* username = new InputBox(NULL,200,150,200,30);
StaticElement* terms = new StaticElement(NULL,220,235,200,15);
Checkbox* checkbox = new Checkbox(NULL,200,235,15);
StaticElement* logmessages = new StaticElement(NULL,10,frame->height()-30,2,2);

void threadprocess(){

  for(int i = 0;i<500;i++){
    logmessages->setText(L"Uploading");
    Sleep(10);
  }

}

std::wstring openfile(HWND hwnd,bool save){   
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

void click(Button* button){

   std::thread worker(threadprocess);
   worker.detach();

}

void checkfunction(Checkbox* checkbox){

    if(checkbox->buttonState()){
       terms->setText(L"press sign in to continue.");
    }else{
       terms->setText(L"Accept terms and Conditions.");
    }

}

void quit(Button* button){
   
    PostQuitMessage(0);
}


int main(){

  frame->setBackgroundImage(L"../../assets/girl.jpg");

  frame->hasTitlebar = false;

  signin->setText(L"SIGN IN");
  signin->setBackgroundColor(34,108,224,200);
  signin->setFont(L"Roboto");
  signin->setTextSize(10);
  signin->onClick = click;

  Button* signin1 = new Button(NULL,320,270,80,25);
  signin1->setText(L"CANCEL");
  signin1->onClick = quit;
  signin1->setBackgroundColor(34,108,224,200);
  signin1->setFont(L"Roboto");
  signin1->setTextSize(10);

  username->setFont(L"Roboto");
  username->alignCenter = true;
  username->setBackgroundColor(0,0,0,100);
  username->setHint(L"username");
  username->setTextSize(10);

  InputBox* input = new InputBox(NULL,200,195,200,30);
  input->setFont(L"Roboto");
  input->alignCenter = true;
  input->setTextSize(10);
  input->setBackgroundColor(0,0,0,100);
  input->setHint(L"password");
  input->hidden = true;


  checkbox->setBackgroundOffColor(0, 0, 0, 0);
  checkbox->onChange = checkfunction;

  terms->setText(L"Accept Terms and Conditions.");
  terms->setFont(L"Roboto");
  terms->setTextSize(8);
  terms->setBackgroundColor(0,0,0,0);


  logmessages->setText(L"you will get the log messages here.");
  logmessages->setTextSize(10);
  logmessages->setBackgroundColor(0,0,0,0);
  
  frame->add(signin);
  frame->add(signin1);
  frame->add(username);
  frame->add(input);
  frame->add(checkbox);
  frame->add(terms);
  frame->add(logmessages);

  frame->start();

  return 0;
}
