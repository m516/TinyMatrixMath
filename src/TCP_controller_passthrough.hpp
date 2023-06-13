#pragma once

#include "TCP_control_policy.hpp"

namespace tcp{

    /// @brief A trivial controller that passes the input directly to the output.
    template<tmm::Size num_states, typename Scalar = float>
    class Controller_Passthrough: public ControlPolicy<num_states, num_states, Scalar>{
        public:
        using ControlPolicy<num_states, num_states, Scalar>::update_timestamp;
        virtual void update(){
            this->outputs = this->inputs;
            update_timestamp();
        } // update()
    }; // class State
} // namespace tcp