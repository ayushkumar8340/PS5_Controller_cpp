#include "ps5_controller.h"

int main()
{
    std::unique_ptr<PS5Controller> controller = std::make_unique<PS5Controller>();
    if(!controller->setupController())
    {
        std::cout<<"Controller setup failed"<<std::endl;
        return -1;
    }
    if(!controller->startController())
    {   
        std::cout<<"Start failed"<<std::endl;
        return -1;
    }
    
    while(1)
    {
        ControllerData data;
        controller->getControllerData(data);

        std::cout<<"left sticks ";
        data.left_stick.print();

        // std::cout<<"right sticks ";
        // data.right_stick.print();

        // std::cout<<"buttons: ";
        // data.printButtons();

        // std::cout<<"Triggers: ";
        // data.triggers.print();

        // std::cout<<"Bumpers: ";
        // data.bumpers.print();

        // std::cout<<"sticks: ";
        // data.sticks.print();

        usleep(100000);

    }
}