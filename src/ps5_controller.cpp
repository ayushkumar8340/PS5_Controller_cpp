#include "ps5_controller.h"

bool PS5Controller::setupController()
{
    is_running_.store(false,std::memory_order_release);
    joy_fd_ = open(PS5_DEVICE, O_RDONLY | O_NONBLOCK);

    if(joy_fd_ < 0)
    {
        std::cout<<"Unable to connect to controller"<<std::endl;
        return false;
    }
    char name[128];

    ioctl(joy_fd_,JSIOCGAXES, &output_data_.data.num_axes);
    ioctl(joy_fd_,JSIOCGBUTTONS, &output_data_.data.num_buttons);
    ioctl(joy_fd_,JSIOCGNAME(sizeof(name)), name);

    output_data_.data.name = std::string(name);

    output_data_.data.print();

    return true;
}

bool PS5Controller::startController()
{
    if(is_running_.load(std::memory_order_acquire))
    {
        std::cout<<"Already running"<<std::endl;
        return false;
    }

    is_running_.store(true,std::memory_order_release);
    worker_th_ = std::thread(&PS5Controller::work_,this);


    return true;
}

bool PS5Controller::stopController()
{
    is_running_.store(false,std::memory_order_release);
    worker_th_.join();

    return true;
}

void PS5Controller::work_()
{
    while(!is_running_.load(std::memory_order_acquire))
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(10000));
    }

    struct js_event js;
    int num_axes = output_data_.data.num_axes;
    int num_buttons = output_data_.data.num_buttons;

    while(is_running_.load(std::memory_order_acquire))
    {
        ssize_t bytes = read(joy_fd_,& js,sizeof(js_event));
        if(bytes == sizeof(js_event))
        {
            js.type &= ~JS_EVENT_INIT;
        
            if(js.type == JS_EVENT_AXIS && js.number < num_axes)
            {
                fillAxisData_(js.number,(js.value / PS5_VAL_OFFSET));
            }
            else if(js.type == JS_EVENT_BUTTON && js.number < num_buttons)
            {
                fillButtons_(js.number,js.value);
            }
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(10000));
    }
}

bool PS5Controller::getControllerData(ControllerData& output)
{
    if(!is_running_.load(std::memory_order_acquire))
    {
        return false;
    }

    mut_.lock();
    output = output_data_;
    mut_.unlock();

    return true;
}

void PS5Controller::fillAxisData_(const int& num, const double& val)
{
    switch(num)
    {
        case 0:
            mut_.lock();
            output_data_.left_stick.x = val;
            mut_.unlock();
            break;
        case 1:
            mut_.lock();
            output_data_.left_stick.y = val;
            mut_.unlock();
            break;

        case 2:
            mut_.lock();
            output_data_.triggers.left = val;
            mut_.unlock();
            break;
        case 3:
            mut_.lock();
            output_data_.right_stick.x = val;
            mut_.unlock();
            break;

        case 4:
            mut_.lock();
            output_data_.right_stick.y = val;
            mut_.unlock();
            break;

        case 5:
            mut_.lock();
            output_data_.triggers.right = val;
            mut_.unlock();
            break;

        case 6:
            mut_.lock();
            output_data_.dpad.x = val;
            mut_.unlock();
            break;

        case 7:
            mut_.lock();
            output_data_.dpad.y = val;
            mut_.unlock();
            break;
    }
}

void PS5Controller::fillButtons_(const int& num, const double& val)
{
    switch(num)
    {
        case 0:
            mut_.lock();
            output_data_.buttons[0] = val;
            mut_.unlock();
            break;

        case 1:
            mut_.lock();
            output_data_.buttons[1] = val;
            mut_.unlock();
            break;

        case 2:
            mut_.lock();
            output_data_.buttons[2] = val;
            mut_.unlock();
            break;

        case 3: 
            mut_.lock();
            output_data_.buttons[3] = val;
            mut_.unlock();
            break;

        case 4:
            mut_.lock();
            output_data_.bumpers.left = val;
            mut_.unlock();
            break;

        case 5:
            mut_.lock();
            output_data_.bumpers.right = val;
            mut_.unlock();
            break;

        case 8:
            mut_.lock();
            output_data_.buttons[4] = val;
            mut_.unlock();
            break;

        case 9:
            mut_.lock();
            output_data_.buttons[5] = val;
            mut_.unlock();
            break;

        case 11:
            mut_.lock();
            output_data_.sticks.left = val;
            mut_.unlock();
            break;  

        case 12:
            mut_.lock();
            output_data_.sticks.right = val;
            mut_.unlock();
            break;
    }
}