#pragma once

#include "TCP_control_policy.hpp"

namespace tcp{

    /// @brief The classic discrete-time PID controller. 
    ///        It uses the system's time to compensate for function calls at irregular intervals (units are in seconds).
    /// @tparam Scalar the common datatype to use for all operations.
    /// @tparam num_inputs_and_outputs if not one, creates a vector of parallel PID controllers
    template<tmm::Size num_inputs_and_outputs = 1, typename Scalar = float>
    class Controller_PID: public ControlPolicy<num_inputs_and_outputs, num_inputs_and_outputs, Scalar>{
        public:
        tmm::Matrix<1, num_inputs_and_outputs, Scalar> Kp, Ki, Kd;
        using ControlPolicy<num_inputs_and_outputs, num_inputs_and_outputs, Scalar>::time_since_last_update_seconds;
        using ControlPolicy<num_inputs_and_outputs, num_inputs_and_outputs, Scalar>::update_timestamp;
        virtual void update(){
            float deltaTime = time_since_last_update_seconds();
            _sum = _sum + this->inputs*deltaTime;
            this->outputs = 
                Kp.elementwise_times(this->inputs)                     + 
                Ki.elementwise_times(_sum)                             +
                Kd.elementwise_times(this->inputs-_previous)/deltaTime ;
            update_timestamp();
        }
        private:
        tmm::Matrix<1, num_inputs_and_outputs, Scalar> _previous, _sum;
    };
}