#include "float_eq.hpp"

bool float_eq(float a, float b){
    float diff = a-b;
    if(diff < 0) diff = -diff;
    return diff < FLOAT_TOLERANCE;
}