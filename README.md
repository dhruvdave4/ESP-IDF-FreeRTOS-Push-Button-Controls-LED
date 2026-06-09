# ESP-IDF FreeRTOS: Push Button Controls LED

## Overview

This project demonstrates FreeRTOS Queue communication in ESP-IDF.

A push button is used to send an event through a FreeRTOS Queue. When the button is pressed, a message is sent to the LED task, which turns the LED ON.

## Features

* FreeRTOS Queue
* Task-to-task communication
* GPIO input handling
* GPIO output control
* ESP-IDF and FreeRTOS implementation

## Hardware Requirements

* ESP32 Development Board
* Push Button
* LED
* USB Cable

## Project Architecture

```text
Push Button Task
        |
        | ISR 
        v
   FreeRTOS  task
        |
        |ISR 
        v
      LED Task
        |
        v
      LED ON
```

## FreeRTOS APIs Used

```c

xTaskCreate()
vTaskDelay()
```

## Working Principle

1. The LED starts in the OFF state.
2. The Button Task continuously monitors the push button.
3. When the button is pressed, an event is sent to the queue.
4. The LED Task waits for queue messages.
5. Upon receiving the event, the LED is turned ON.

## Build and Flash

```bash
idf.py set-target esp32
idf.py build
idf.py flash
idf.py monitor
```

## Learning Objectives

* Understand FreeRTOS Queue operations
* Learn task-to-task communication
* Work with GPIO input and output
* Develop event-driven applications using ESP-IDF

## Author

Dhruv Dave
