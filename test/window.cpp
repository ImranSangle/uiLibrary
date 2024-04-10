#include "toggleButton.h"
#include <iostream>
#include <string>
#include <thread>
#include <uilibrary.h>

Frame* frame = new Frame(L"Facebook",10,10,600,400,true);
StaticElement* text = new StaticElement(NULL,310,50,3,3);

void slidercnange(Slider* slidu){

  float position = slidu->getPosition();
  std::cout<<"Slider's position is "<<position<<std::endl;

}

void threadproc(){

  MSG msg;
  while(PeekMessage(&msg,0,0,0,PM_NOREMOVE)){
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
    std::cout<<msg.message<<std::endl;
  }
}

void click(Button* button){

  frame->setBackgroundColor(255,0,0);

}

int main(){

  frame->setBackgroundColor(20,20,20);

  Slider* slider = new Slider(NULL,50,200,50);
  slider->setMin(0);
  slider->setMax(10);
  slider->setBackgroundColor(50,50,50);

  slider->onChange = slidercnange;

  text->setTextSize(12);
  text->setBackgroundColor(20,20,20);

  frame->add(slider);

  std::thread worker(threadproc);

  worker.detach();

  frame->start();

  return 0;
}
