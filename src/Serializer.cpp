#include "Serializer.hpp"

namespace serializer{


uint8_t create_unique_id(){
    static uint8_t id = 1;
    return id++;
    #ifdef USING_STANDARD_LIBRARY
    if(id == 0){
        throw std::runtime_error("Too many objects have been serialized. The maximum number of unique objects is 255.");
    }
    #endif
}

}