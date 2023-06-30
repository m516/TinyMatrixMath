#pragma once

#include "TinyMatrixMath.hpp"
#include "TCP_utils.hpp"

namespace tcp{

    template<tmm::Size num_inputs, typename Scalar = float>
    class States{
        public:
        tmm::Matrix<num_inputs,  1, Scalar> states;

        // Timekeeping

        /// @brief Get the time since the last call to update() in milliseconds
        /// @return unsigned long
        unsigned long time_since_last_update_milliseconds(){
            return millis()-_timestamp;
        }

        /// @brief Get the time between the last two update() calls in milliseconds
        /// @return unsigned long
        unsigned long instantaneous_update_period_milliseconds(){return _instantaneous_update_period;}

        /// @brief Get the time since the last call to update() in seconds
        /// @return float
        float time_since_last_update_seconds(){
            return (float)time_since_last_update_seconds() / 1000.f;
        }

        /// @brief Get the time between the last two update() calls in seconds
        /// @return float
        float instantaneous_update_period_seconds(){
            return (float)instantaneous_update_period_milliseconds() / 1000.f;
        }

        /// @brief Call this at the end of your update() function to update the timekeeping variables
        void update_timestamp(){
            unsigned long now = millis(); 
            _instantaneous_update_period = now-_timestamp; 
            _timestamp = now;
        }

        private:
        /// @brief The value of millis() during the last time update() was called
        unsigned long _timestamp = 0;
        /// @brief The time between the last two update() calls in milliseconds
        unsigned long _instantaneous_update_period = 0;
    };
}