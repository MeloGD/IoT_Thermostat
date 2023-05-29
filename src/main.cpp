#include "UI/mcu_ui.h"
#include "RTOS/tasks.h"

// setup y main
void setup(void) {
  Serial.begin(115200);

  pinMode(8, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);  
  pinMode(9, INPUT);
  pinMode(46, INPUT);
  pinMode(6, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //pinMode(18, OUTPUT);

  createTasks();
  launchUi();
  //drawWIFIMenu();

  Serial.println( "Setup done" );
}

void loop() {
  //lv_timer_handler(); /* let the GUI do its work */
  //delay(5);
}