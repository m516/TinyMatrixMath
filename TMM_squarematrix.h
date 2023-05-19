#pragma once

#include "TMM_matrix.h"


namespace tmm{

    template<Size n>
    class SquareMatrix: public Matrix<n,n>{
        public:
        SquareMatrix(): Matrix<n,n>(){};
        SquareMatrix(Matrix<n,n> A){ operator=(A); };
        SquareMatrix(const Scalar M): Matrix<n,n>(M){};
        using Matrix<n,n>::operator =;


        #ifndef CPL_DISABLE_RECURSIVE
        Scalar
        determinant() const
        {
            if(n == 0) { return 1; }
            if(n == 1) { return Matrix<n,n>::data[0][0]; }
            if(n == 2) { return Matrix<n,n>::data[0][0] * Matrix<n,n>::data[1][1] - Matrix<n,n>::data[0][1] * Matrix<n,n>::data[1][0]; }
            Scalar sign = 1;
            Scalar ret  = 0;
            for(Size i = 0; i < n; i++){
                // Get submatrix
                SquareMatrix<(n>0?n-1:n)> submatrix; 
                // the (n>0?n-1:n) trickery means decrement by one, 
                // stopping at zero (in wich case this code is unreachable)
                // It's a hack that can be replaced in newer C++ compilers


                for(Size p = 0; p < n-1; p ++){ // for each row in submatrix
                    for(Size q = 0; q < i; q++){ // for each column in submatrix until hitting the i'th column
                        submatrix[p][q]=Matrix<n,n>::data[p+1][q];
                    }
                    // skip the i'th column
                    for(Size q = i+1; q < n; q++){
                        submatrix[p][q]=Matrix<n,n>::data[p+1][q-1];
                    }
                }
                // end of code snippet
                ret = ret + sign * Matrix<n,n>::data[0][i] * submatrix.determinant();
                sign = -sign;
            }
            return ret;
        }





        SquareMatrix<n>
        cofactor() const
        {
            SquareMatrix<n> M;
            for(Size i = 0; i < n; i ++){
                for(Size j = 0; j < n; j++){
                    //  Take the determinant of the matrix
                    // formed by all elements that are not
                    // in the i'th row or the j'th column.
                    //
                    //  Construct a temporary matrix with these
                    // values so we can take the determinant 
                    // of it.
                    //
                    // TODO simplify this garbage
                    SquareMatrix<n-1> t;
                    for(Size p = 0; p < i; p ++){
                        for(Size q = 0; q < j; q++){
                            t[p][q]=Matrix<n,n>::data[p][q];
                        }
                        for(Size q = j+1; q < n; q++){
                            t[p][q]=Matrix<n,n>::data[p][q-1];
                        }
                    }
                    for(Size p = i+1; p < n; p ++){
                        for(Size q = 0; q < j; q++){
                            t[p-1][q]=Matrix<n,n>::data[p-1][q];
                        }
                        for(Size q = j+1; q < n; q++){
                            t[p-1][q]=Matrix<n,n>::data[p-1][q-1];
                        }
                    }
                    M[i][j] = t.determinant();

                    // Without this line, M would be a matrix of minors
                    if (i+j%2==1) M[i][j] = -M[i][j];
                }
            }
            return M;
        }// end cofactor





        SquareMatrix<n>
        inverse() const
        {
            // The matrix is the adjugate divided by the determinant,
            // where the adjugate is the cofactor transposed
            SquareMatrix<n> adjugate = cofactor().transpose();
            
            return adjugate / determinant();
        } // end inverse


    #endif // ifndef DISABLE_RECURSIVE 
        
    };
    
}// end namespace cpl