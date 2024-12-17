#include <iostream>
#include <uilibrary.h>

Progressbar* progress = new Progressbar(NULL,10,350,200,10);

int main(){

  Frame* frame = new Frame(L"mainWindow",100,100,600,400,true);

  Button* button = new Button(NULL,10,10,200,20);
  button->setText(L"Press");

  Checkbox* check = new Checkbox(NULL,50,50,15);

  progress->setProgress(50);

  check->onChange = [](Checkbox* value){
    std::cout<<value->buttonState()<<std::endl;
  };

  InputBox* input = new InputBox(NULL,10,300,200,30);
  input->setTextColor(255, 255, 255);
  input->setHint(L"enter your name");

  Slider* slider = new Slider(NULL,10,0,100);
  slider->enableHint();
  slider->setMax(100);
  slider->onChange = [](Slider* slide){
      progress->setProgress(slide->getPosition());
  };

  ToggleButton* toggle = new ToggleButton(NULL,0,0,20);

  LinearLayout* layout = new LinearLayout(NULL,10,10,400,400);
  layout->setPadding(15);

  frame->add(layout);
  layout->add(button);
  layout->add(check);
  layout->add(toggle);
  layout->add(slider);
  layout->add(progress);
  layout->add(input);
  frame->start();
  
  return 0;
}
