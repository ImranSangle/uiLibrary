#include <array>
#include <cstdlib>
#include <string>
#include <uilibrary.h>
#include <thread>

Frame* frame = new Frame(L"mainwindow",100,100,600,400,false);
LinearLayout* layout = new LinearLayout(NULL,280,10,300,300);

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


void quit(Button* button){
   
   PostQuitMessage(0);
}

int main(){

  // frame->setBackgroundImage(L"../../assets/girl.jpg");

  frame->hasTitlebar = false;

  Button* signin1 = new Button(NULL,10,270,80,25);
  signin1->setText(L"CANCEL");
  signin1->onClick = quit;
  signin1->setBackgroundColor(34,108,224,200);
  signin1->setFont(L"Roboto");
  signin1->setTextSize(10);
  
  frame->add(layout);
  frame->add(signin1);
  
  layout->setPadding(10);
  layout->setBackgroundColor(0,0,0,255);

  std::array<Checkbox*,20> toggles;
  std::array<StaticElement*,20> text;
  std::array<Group*,20> group;

  for(int i=0;i<toggles.size();i++){

    group[i] = new Group(NULL,5,0,280,50);
    layout->add(group[i]);
    group[i]->setBackgroundColor(40,40,40,255);
    toggles[i] = new Checkbox(NULL,group[i]->getWidth()-40,10,25);
    toggles[i]->setBackgroundOffColor(0,0,0,0);
    text[i] = new StaticElement(NULL,10,12,4,4);
    text[i]->setBackgroundColor(40,40,40,0);
    text[i]->setText(L"This is the button "+std::to_wstring(i));
    text[i]->setFont(L"Roboto");
    text[i]->setTextSize(12);
    group[i]->add(toggles[i]);
    group[i]->add(text[i]);
  }


  frame->start();

  return 0;
}
