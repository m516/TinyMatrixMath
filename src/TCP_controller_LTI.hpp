#pragma once

#include "TCP_control_policy.hpp"

namespace tcp{

    /// @brief The classic discrete-time linear time-invariant system: X_dot = A X + B U; Y = C X + D <br>
    /// It uses the system's time to compensate for function calls at irregular intervals (units are in seconds).
    /// @tparam Scalar the common datatype to use for all operations.
    /// @tparam num_inputs          the size of u
    /// @tparam num_outputs         the size of y
    /// @tparam num_internal_states the size of x
    template<tmm::Size num_inputs, tmm::Size num_outputs, tmm::Size num_internal_states, typename Scalar = float>
    class Controller_LTI: public ControlPolicy<num_inputs, num_outputs, Scalar>{
        public:
        tmm::Matrix<num_internal_states, num_internal_states, Scalar> A;
        tmm::Matrix<num_internal_states, num_inputs,          Scalar> B;
        tmm::Matrix<num_outputs,         num_internal_states, Scalar> C;
        tmm::Matrix<num_outputs,         num_inputs,          Scalar> D;
        tmm::Matrix<num_internal_states, 1,                   Scalar> X;
        using ControlPolicy<num_inputs, num_outputs, Scalar>::time_since_last_update_seconds;
        using ControlPolicy<num_inputs, num_outputs, Scalar>::update_timestamp;
        virtual void update(){
            float deltaTime = time_since_last_update_seconds();
            tmm::Matrix<num_internal_states, 1, Scalar> 
                    X_dot = A*X + B*this->inputs;
                        X =   X + X_dot*(Scalar)deltaTime;
            this->outputs = C*X + D*this->inputs;
            update_timestamp();
        }
    };
}