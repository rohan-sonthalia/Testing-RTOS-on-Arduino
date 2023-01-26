#include <PS2X_lib.h> 
#include <Arduino_FreeRTOS.h>

TaskHandle_t TaskController_Handler;
TaskHandle_t TaskButton_Handler;
TaskHandle_t TaskOg_Handler;

void TaskController();
void TaskButton();
void TaskOg();

PS2X ps2x; 

#define clk 13
#define command 11
#define attention 10
#define data 12

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  xTaskCreate(TaskController, "Joystick Input", 128, NULL, 1, &TaskController_Handler);
  xTaskCreate(TaskButton, "Button Interrupt", 128, NULL, 1, &TaskButton_Handler);
  xTaskCreate(TaskOg, "Original Task", 128, NULL, 1, &TaskOg_Handler);

  vTaskStartScheduler();

  vTaskSuspend(TaskButton_Handler);

  int error = ps2x.config_gamepad(clk,command,attention,data, true, true); 
  if(error){
    Serial.println("Error setting up Controller");
    vTaskSuspend(TaskOg_Handler);
    vTaskSuspend(TaskButton_Handler);
    vTaskSuspend(TaskController_Handler);
  }
}

void TaskOg(){
  Serial.println("Original Task");
  vTaskDelay( 500 / portTICK_PERIOD_MS );
}

void TaskController(){
  int xLJoystick = ps2x.Analog(PSS_LX);
  Serial.print("JoyStick data: ");
  Serial.println(xLJoystick);
  if(ps2x.Button(PSB_CIRCLE)){
    Serial.println("Button Pressed");
    Serial.println("Moving to Button Oriented Task");
    vTaskSuspend(TaskOg_Handler);
    vTaskResume(TaskButton_Handler);
  }
  vTaskDelay( 500 / portTICK_PERIOD_MS );
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
  // put your main code here, to run repeatedly:
}
