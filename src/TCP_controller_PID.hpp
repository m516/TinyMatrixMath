#pragma once

#include "TCP_control_policy.hpp"

namespace tcp{

    template<tmm::Size num_inputs_and_outputs, typename Scalar = float>
    class Controller_PID: public ControlPolicy<num_inputs_and_outputs, num_inputs_and_outputs, Scalar>{
        public:
        tmm::Matrix<1, num_inputs_and_outputs, Scalar> Kp, Ki, Kd;
        virtual void update(){
            float deltaTime = time_since_last_update_seconds();
            _sum = _sum + input*deltaTime;
            output = 
                Kp.elementwise_times(inputs)                     + 
                Ki.elementwise_times(_sum)                       +
                Kd.elementwise_times(inputs-_previous)/deltaTime ;
            update_timestamp();
        }
        private:
        tmm::Matrix<1, num_inputs_and_outputs, Scalar> _previous, _sum;
    };
}