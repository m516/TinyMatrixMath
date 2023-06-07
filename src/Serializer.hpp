#pragma once

#ifdef USING_STANDARD_LIBRARY
#include <cstdint>
using uint8_t = std::uint8_t;
#endif

namespace serializer{

    template<typename T>
    class Serializer{
        public:

        Serializer(uint8_t id = 42) : _id(id){}

        uint8_t * new_array(){
            return new uint8_t[sizeof(T)];
        }

        serialize(uint8_t * buffer, uint8_t buffer_size){
            
        }

        private:
        uint8_t _id;
    };
}