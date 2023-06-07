#pragma once

#include "TinyMatrixMath.hpp"

#ifdef USING_STANDARD_LIBRARY
#include <chrono> // for emulating millis()
#endif

namespace tcp{

    template<tmm::Size num_inputs, tmm::Size num_outputs, typename Scalar = float>
    class ControlPolicy{
        public:
        tmm::Matrix<1, num_inputs,  Scalar> inputs;
        tmm::Matrix<1, num_outputs, Scalar> outputs;
        virtual void update() = 0;



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

        protected:
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

        // Subroutines to emulate the Arduino library with the Standard Library
        #ifdef USING_STANDARD_LIBRARY
        // Replicates Arduino's "millis()" if using the Standard Library
        unsigned long millis(){
            namespace sc = std::chrono;
            auto time = sc::system_clock::now();
            auto since_epoch = time.time_since_epoch();
            auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
            long now = millis.count();
        }
        #endif
    };
}