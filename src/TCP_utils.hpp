#pragma once

#if USING_STANDARD_LIBRARY

namespace tcp{

    // Subroutines to emulate the Arduino library with the Standard Library
    // Replicates Arduino's "millis()" if using the Standard Library
    unsigned long millis();

}

#endif
