#include <string>
#include <uilibrary.h>
#include <thread>

Frame* frame = new Frame(L"Facebook",10,10,600,400,true);
Progressbar* progress = new Progressbar(NULL,frame->getWidth()/2,frame->getHeight()/2,200,10);
StaticElement* text = new StaticElement(NULL,frame->getWidth()/2+210,frame->getHeight()/2,4,4);

void threadfun(){

  for(int i=0;i<1000;i++){
    progress->setProgress(i);
    text->setText(std::to_wstring((int)progress->getProgress()));
    Sleep(10);
  }

}

void prog(Progressbar* p){
  
  static int pro = 0;
  p->setProgress(pro);
  pro++;
}


int main(){

  frame->setBackgroundColor(20,20,20);

  
  progress->setBackgroundColor(30,30,30);
  progress->setBarColor(34,108,227);
  progress->setMax(1000);
  // progress->vertical = true;
  // progress->reversed = true;

  progress->onClick = prog;

  text->setTextSize(12);
  text->setBackgroundColor(20,20,20);

  frame->add(progress);
  frame->add(text);

  std::thread worker(threadfun);

  worker.detach();

  frame->start();

 
  return 0;
}
