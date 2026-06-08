#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"


static const char *TAG = "blynk_led_driver_with_button_and_freertos_task";
// define gpio pin numer for the LED, you can change it according to your board
#define BLINK_GPIO 2    
#define Button_Gpio 4
# define BLINK_PERIOD_MS 1000     
#define BLINK_PERIOD_TICKS (BLINK_PERIOD_MS / portTICK_PERIOD_MS)  //1000ms/10ms --> 100 tick in 1s more slow tick
#define  Blink_Period_Sec 2 
#define BLINK_PERIOD_TICKS_2 (Blink_Period_Sec  / portTICK_PERIOD_MS) // 20ms/10ms --> 2 tick in 1s more slow tick
// s_led_state is used to keep track of the current state of the LED (ON or OFF)
static uint8_t s_led_state = 0;  
static bool button_state =1;  
static uint8_t current_status ; // 1 for released, 0 for pressed  
static uint8_t previous_status=1 ; // 1 for released, 0 for pressed


static void configure_button(void) 
{  
  
    ESP_LOGI(TAG, "configured to read GPIO Button!"); 
    /* Set the GPIO as an input with pull-up resistor */
        gpio_set_direction(Button_Gpio, GPIO_MODE_INPUT);   
        gpio_set_pull_mode(Button_Gpio, GPIO_PULLUP_ONLY);

    // gpio_set_level(Button_Gpio, button_state);
    // gpio_set_pull_mode(Button_Gpio, GPIO_PULLUP_ONLY); 
}   
static void configure_led(void)
{
    ESP_LOGI(TAG, "configured to blink GPIO LED!");
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}  
//```````````````````````````````````````with freertos task programming Begin`````````````````````````````````` // 
void led_task(void *pvParameters) 
{ 
    while (1) 
    { 
        if(button_state==0) 
        { 
             gpio_set_level(BLINK_GPIO, !s_led_state);  
             ESP_LOGI(TAG, "Turning the LED ON \r\n");
              vTaskDelay(BLINK_PERIOD_TICKS); 
        } 
        else  
        { 
                gpio_set_level(BLINK_GPIO, s_led_state);  
               ESP_LOGI(TAG, "Turning the LED off \r\n");
                 vTaskDelay(BLINK_PERIOD_TICKS); 
        }
        // gpio_set_level(BLINK_GPIO, s_led_state); 
        // ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        // vTaskDelay(BLINK_PERIOD_TICKS); // Delay for the specified period
    }   
} 
void button_task(void *pvParameters) 
{ 
    while (1) 
    { 
        if (gpio_get_level(Button_Gpio) == 0) 
        {   
            current_status = gpio_get_level(Button_Gpio);
            ESP_LOGI(TAG, "button_pressed!\r\n");   
            // button_state = 0;     
            if(current_status ==0 && previous_status ==1)
            {  
                 vTaskDelay(BLINK_PERIOD_TICKS_2);  
                 current_status = gpio_get_level(Button_Gpio);
                    if(current_status ==0) 
                    {  
                        button_state = 0;  
                        ESP_LOGI(TAG, "button pressed!\r\n");
                        previous_status = current_status;
                        
                    } 
                    
            }
        } 
        else 
        { 
            // button_state = 1;    
             button_state = 1;   
                        current_status = gpio_get_level(Button_Gpio);    
                        ESP_LOGI(TAG, "button_released!\r\n");    
                        previous_status = current_status;
            // ESP_LOGI(TAG, "system_reset!\r\n");    
        } 
        //    vTaskDelay(100 / portTICK_PERIOD_MS); // Check the button state every 100ms
    }   
}   





//```````````````````````````````````````with freertos task programming End```````````````````````````````

// ```````````````without freertos task normal progrmamming  Begin`````````````````````````````````//
// static void toggle_led(void) 
// {   
//     //    s_led_state = !s_led_state;   
//     if (gpio_get_level(Button_Gpio) == 0) 
//     {  
//             ESP_LOGI(TAG, "button_pressed!\r\n");   
//             s_led_state = 1;  
//               ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
//     } 
//      else 
//      { 
//           s_led_state = 0;   
//           ESP_LOGI(TAG, "button_released!\r\n");   
//             ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");           
//     }
//     gpio_set_level(BLINK_GPIO, s_led_state);   
   
//     // ESP_LOGI(TAG, "Turning the LED OFF!");  
//     // s_led_state = 1;     
// }    
  
// ```````````````without freertos task normal progrmamming End`````````````````````````````````//

void app_main(void)
{
    configure_led();
    configure_button();
//  with  freertos task programmingm 
xTaskCreate(led_task, "LED Task", 2048, NULL, 5, NULL);
xTaskCreate(button_task, "Button Task", 2048, NULL, 5, NULL);

}
        



    // without freertos task programmingm
    // while (1) {
        // ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        // blink_led(); 
        // toggle_led(); 
            // toggle_button(); 
            // button_read();    
        /* Toggle the LED state */
        // s_led_state = !s_led_state; 

        // vTaskDelay(BLINK_PERIOD_TICKS); // Delay for the specified period
        // 1000ms /100ms = 10  tick 
        // we give  dely of ht e 2000ms  -->2s  
        // 2000/10 --> 200 tick in i sec   

        // 1000ms/1000--> 1 tick    
        //delay of 1000ms --> 1s    
        //1000ms/1 --> 1000 tick in 1s more fast tick 
        // 

        // 1000ms/10 --> 100 tick in 1s more slow tick  
        // 1000ms/100 --> 10 tick in 1s more slow tick 


    // }
