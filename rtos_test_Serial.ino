#include <Arduino_FreeRTOS.h>

TaskHandle_t TaskController_Handler;
TaskHandle_t TaskButton_Handler;
TaskHandle_t TaskOg_Handler;

void TaskController();
void TaskButton();
void TaskOg();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  xTaskCreate(TaskController, "Joystick Input", 128, NULL, 1, &TaskController_Handler);
  xTaskCreate(TaskButton, "Button Interrupt", 128, NULL, 1, &TaskButton_Handler);
  xTaskCreate(TaskOg, "Original Task", 128, NULL, 1, &TaskOg_Handler);

  vTaskStartScheduler();

  vTaskSuspend(TaskButton_Handler);
}

void TaskOg(){
  Serial.println("Original Task");
  vTaskDelay( 500 / portTICK_PERIOD_MS );
}

void TaskController(){
  if(Serial.available()){
    if(Serial.read()=="a"){
      vTaskSuspend(TaskOg_Handler);
      Serial.println("Moving to Button Oriented Task");
      vTaskResume(TaskButton_Handler);
    }
  }
  
}

void TaskButton(){
  int t = millis();
  Serial.println("Button Oriented Task");
  if(millis()-t == 5000){
    vTaskResume(TaskOg_Handler);
    vTaskSuspend(TaskButton_Handler);
  }
  vTaskDelay( 500 / portTICK_PERIOD_MS );
}

void loop() {
  // Keep Empty
}
