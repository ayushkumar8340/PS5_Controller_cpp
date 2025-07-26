#ifndef PS5_CONTROLLER_H
#define PS5_CONTROLLER_H

#include "controller_data_types.h"



class PS5Controller
{
    public:
        bool setupController();
        bool startController();
        bool getControllerData(ControllerData& );
        bool stopController();
        bool isControllerRunning(){return is_running_.load(std::memory_order_acquire);}
    
    private:    
        
        ControllerData output_data_;
        std::atomic<bool> is_running_;
        std::thread worker_th_;
        std::mutex mut_;

        void work_();
        void fillAxisData_(const int& num, const double& val);
        void fillButtons_(const int& num, const double& val);

        int joy_fd_;
};


#endif