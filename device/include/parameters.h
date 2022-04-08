#ifndef PARAMETERS_H 
#define PARAMETERS_H

// TASK PARAMETERS
#define TASK_STACK_SIZE             10000
#define TASK_PRIORITY               0

// GENERAL PARAMETERS
#define ARRAY_SIZE                  35

// OLED SCREEN
#define SCREEN_WIDTH                128 
#define SCREEN_HEIGHT               64 
#define OLED_RESET                  -1 
#define SCL_PIN                     22
#define SDA_PIN                     21
#define BUTTON_PIN                  2

// WIFI MODULE
#define SOFT_AP_MODE 1
#define STATION_MODE 2


#define SELECT     14 
#define CALIBRATE   27
#define GREEN   18
#define RED 19


enum calibration {ACTIVE = 0, CELL_A, CELL_B, CELL_C, CELL_D, CAL, SEND_DATA,BALANCE, SELECT_WEIGHT};
enum actualice{X = 0, A, B, C, D};



#endif