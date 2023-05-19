#pragma once

#include "TMM_matrix.h"

namespace tmm{

    template<Size n>
    class Vector: public Matrix<n,1>{
        public:
        Vector(): Matrix<n,1>(){};
        Vector(Matrix<n,1> A){ operator=(A); };
        using Matrix<n,1>::operator =;
    };
    
}
