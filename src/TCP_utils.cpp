#include "TCP_utils.hpp"

#ifdef USING_STANDARD_LIBRARY

#include <chrono> // for emulating millis()

namespace tcp{

    // Subroutines to emulate the Arduino library with the Standard Library
    // Replicates Arduino's "millis()" if using the Standard Library
    unsigned long millis(){
        namespace sc = std::chrono;
        auto time = sc::system_clock::now();
        auto since_epoch = time.time_since_epoch();
        auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
        long now = millis.count();
        return now;
    }

}

#endif