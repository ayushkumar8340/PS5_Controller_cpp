#ifndef CONTROLLER_DATA_TYPES_H
#define CONTROLLER_DATA_TYPES_H

#include "string"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <vector>
#include <string.h>
#include <atomic>
#include <thread>
#include <mutex>


#define PS5_DEVICE "/dev/input/js0"
#define PS5_VAL_OFFSET 32767.0f

typedef struct
{
    double x,y;

    void print()
    {
        std::cout<<"X: "<<x<<", Y: "<<y<<"\n";
    }
}Axis;

typedef struct
{
    double left,right;
    void print()
    {
        std::cout<<"Left: "<<left<<", Right: "<<right<<std::endl;
    }
}Sides;

typedef struct
{
    std::string name;
    int num_axes;
    int num_buttons;

    void print()
    {
        std::cout<<"Name: "<<name<<std::endl;
        std::cout<<"Num axes: "<<num_axes<<", Num Buttons: "<<num_buttons<<std::endl;
    }
}MetaData;

typedef struct
{
    MetaData data;
    Axis left_stick;
    Axis right_stick;
    Axis dpad;
    
    Sides triggers;
    Sides bumpers;
    Sides sticks;
    int buttons[6];

    void printButtons()
    {
        std::cout<<buttons[0]<<", "<<buttons[1]<<", "<<buttons[2]<<", "<<buttons[3]<<", "<<buttons[4]<<", "<<buttons[5]<<std::endl;
    }

}ControllerData;


#endif
