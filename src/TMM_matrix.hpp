#pragma once

#include "TMM_enable_if.hpp"
#ifdef ARDUINO
    #pragma weak dtostrf // for fixed-width float printing to serial to create uniform-looking matrices
    #include <Arduino.h>
#endif
#ifdef USING_STANDARD_LIBRARY // a macro defined in CMakeLists.txt
    #include <iostream>
    #include <iomanip>
#endif


namespace tmm{


    typedef unsigned char Size;

    template<Size n, Size m, typename Scalar = float>
    class Matrix{
        public:

        Scalar data[n][m];

        Matrix(){
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            data[i][j]=0;
        }

        Matrix(const float M[n][m]){
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            data[i][j]=M[i][j];
        }

        Matrix(const float M){
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            data[i][j]=M;
        }


        // Set this matrix to the value of another matrix
        void
        operator=(const Matrix<n,m,Scalar> &M){
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            data[i][j]=M.data[i][j];
        }


        // Set this matrix to the value of a 2D array of scalars
        void
        operator=(const Scalar M[n][m])
        {
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            data[i][j]=M[i][j];
        }


        // Set all values in the matrix to be a scalar
        void
        operator=(const Scalar value)
        {
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            data[i][j]=value;
        }


        Scalar* 
        operator[](Size i)
        {return data[i];}


        template<typename T = Scalar, typename = enable_if_t<(m==1&&n==1), T>> operator T() { 
            return data[0][0]; 
        }

        template<Size q> 
        Matrix<n,m+q,Scalar>
        augmentAfter(const Matrix<n,q,Scalar> &other) const
        {
            Matrix<n,m+q,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j+m]=other.data[i][j];

            return M;
        }


        template<Size q> 
        Matrix<n,m+q,Scalar>
        augmentBefore(const Matrix<n,q,Scalar> &other) const
        {
            Matrix<n,m+q,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j+q]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=other[i][j];

            return M;
        }


        template<Size p> 
        Matrix<n+p,m,Scalar>
        augmentAbove(const Matrix<p,m,Scalar> &other) const
        {
            Matrix<n+p,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i+p][j]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=other.data[i][j];

            return M;
        }



        template<Size p> 
        Matrix<n+p,m,Scalar>
        augmentBelow(const Matrix<p,m,Scalar> &other) const
        {
            Matrix<n+p,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i+n][j]=other.data[i][j];

            return M;
        }


        // Matrix-matrix multiplication
        // For elementwise multiplication, use elementwise_multiplication(...)
        template<Size q> 
        Matrix<n,q,Scalar>
        operator *(const Matrix<m,q,Scalar> &other) const
        {
            Matrix<n,q,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < q; j++) 
            for(Size k = 0; k < m; k++) 
            M[i][j]+=data[i][k]*other.data[k][j];
            return M;
        }

        


        // Elementwise multiplication
        Matrix<n,m,Scalar>
        elementwise_times(const Matrix<n,m,Scalar> &other) const
        {
            Matrix<n,m,Scalar> M;
            for(Size i = 0; i < n; i++)
            for(Size j = 0; j < m; j++)
            M[i][j]=data[i][j]*other.data[i][j];
            return M;
        }

        


        // Elementwise multiplication
        Matrix<n,m,Scalar>
        negate() const
        {
            Matrix<n,m,Scalar> M;
            for(Size i = 0; i < n; i++)
            for(Size j = 0; j < m; j++)
            M[i][j]=-data[i][j];
            return M;
        }

        



        Matrix<n,m,Scalar> 
        operator +(const Matrix<n,m,Scalar> &other) const
        {
            Matrix<n,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]+other.data[i][j];
            return M;
        }



        Matrix<n,m,Scalar> 
        operator -(const Matrix<n,m,Scalar> &other) const
        {
            Matrix<n,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]+other.data[i][j];
            return M;
        }

        

        Matrix<n,m,Scalar> 
        operator +(Scalar a) const
        {
            Matrix<n,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]+a;
            return M;
        }

        Matrix<n,m,Scalar> 
        operator -(Scalar a) const
        {
            Matrix<n,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]-a;
            return M;
        }

        // Multiplication by a scalar
        Matrix<n,m,Scalar> 
        operator *(Scalar a) const
        {
            Matrix<n,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]*a;
            return M;
        }
        Matrix<n,m,Scalar> 
        operator /(Scalar a) const
        {
            Matrix<n,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]/a;
            return M;
        }



        Matrix<m,n,Scalar>
        transpose() const
        {
            Matrix<m,n,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[j][i]=data[i][j];
            return M;
        }


        // Scalar
        // get(Size i, Size j) const
        // {
        //     return data[i][j];
        // }

        
        template<Size p, Size q>
        Matrix<p,q,Scalar>
        get(Size c, Size d) const
        {
            Matrix<p,q,Scalar> M;
            for(Size i = 0; i < p; i++) 
            for(Size j = 0; j < q; j++) 
            M[i][j]=data[i+c][j+d];     
            return M;
        }

        void
        set(Size i, Size j, Scalar newVal)
        {
            data[i][j] = newVal;
        }

        
        template<Size p, Size q>
        void
        set(Size c, Size d, Matrix<p,q,Scalar> newVal)
        {
            for(Size i = 0; i < p; i++) 
            for(Size j = 0; j < q; j++) 
            data[i+c][j+d] = newVal[i][j];    
        }



        Matrix<1,m,Scalar>
        row(Size i) const
        {
            return get<1, m>(i, 0);
        }

        
        Matrix<1,m,Scalar>
        column(Size j) const
        {
            return get<n,1>(0, j);
        }


        void
        copyTo(Matrix<n,m,Scalar> &other) const
        {
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            other[i][j]=data[i][j];
        }

        // Implementation-specific functions
        #ifdef ARDUINO
        void
        printTo(Print &serial) const
        {
            for(Size i = 0; i < n; i++) 
            {
                for(Size j = 0; j < m; j++) 
                {
                    if(dtostrf){
                        char buf[7];
                        dtostrf(data[i][j], 6, 3, buf);
                        serial.print(buf);
                    }
                    else{
                        serial.print(data[i][j]);
                    }
                    serial.print("\t");
                }
                serial.println();
            }
        } // end printTo
        #endif
        #ifdef USING_STANDARD_LIBRARY
        void
        printTo(std::ostream &out) const
        {
            for(Size i = 0; i < n; i++) 
            {
                for(Size j = 0; j < m; j++) 
                {
                    out << std::setw(6) << data[i][j];
                    out << "\t";
                }
                out << std::endl;
            }
        }
        #endif


        #ifndef TMM_DISABLE_RECURSIVE
        template <typename T = Scalar>
        enable_if_t<(m==n), T>
        determinant() const
        {
            if(n == 0) { return 1; }
            if(n == 1) { return Matrix<n,n,Scalar>::data[0][0]; }
            if(n == 2) { return Matrix<n,n,Scalar>::data[0][0] * Matrix<n,n,Scalar>::data[1][1] - Matrix<n,n,Scalar>::data[0][1] * Matrix<n,n,Scalar>::data[1][0]; }
            Scalar sign = 1;
            Scalar ret  = 0;
            for(Size i = 0; i < n; i++){
                // Get submatrix
                Matrix<(n>0?n-1:n),(n>0?n-1:n),Scalar> submatrix; 
                // the (n>0?n-1:n) trickery means decrement by one, 
                // stopping at zero (in wich case this code is unreachable)
                // It's a hack that can be replaced in newer C++ compilers


                for(Size p = 0; p < n-1; p ++){ // for each row in submatrix
                    for(Size q = 0; q < i; q++){ // for each column in submatrix until hitting the i'th column
                        submatrix[p][q]=Matrix<n,n,Scalar>::data[p+1][q];
                    }
                    // skip the i'th column
                    for(Size q = i+1; q < n; q++){
                        submatrix[p][q]=Matrix<n,n,Scalar>::data[p+1][q-1];
                    }
                }
                // end of code snippet
                ret = ret + sign * Matrix<n,n,Scalar>::data[0][i] * submatrix.determinant();
                sign = -sign;
            }
            return ret;
        } // end determinant





        template <typename T = Matrix<n,n,Scalar>>
        enable_if_t<(m==n), T>
        cofactor() const
        {
            Matrix<n,n,Scalar> M;
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
                    Matrix<n-1,n-1,Scalar> t;
                    for(Size p = 0; p < i; p ++){
                        for(Size q = 0; q < j; q++){
                            t[p][q]=Matrix<n,n,Scalar>::data[p][q];
                        }
                        for(Size q = j+1; q < n; q++){
                            t[p][q]=Matrix<n,n,Scalar>::data[p][q-1];
                        }
                    }
                    for(Size p = i+1; p < n; p ++){
                        for(Size q = 0; q < j; q++){
                            t[p-1][q]=Matrix<n,n,Scalar>::data[p-1][q];
                        }
                        for(Size q = j+1; q < n; q++){
                            t[p-1][q]=Matrix<n,n,Scalar>::data[p-1][q-1];
                        }
                    }
                    M[i][j] = t.determinant();

                    // Without this line, M would be a matrix of minors
                    if (i+j%2==1) M[i][j] = -M[i][j];
                }
            }
            return M;
        }// end cofactor





        template <typename T = Matrix<n,n,Scalar>>
        enable_if_t<(m==n), T>
        inverse() const
        {
            // The matrix is the adjugate divided by the determinant,
            // where the adjugate is the cofactor transposed
            Matrix<n,n,Scalar> adjugate = cofactor().transpose();
            
            return adjugate / determinant();
        } // end inverse


    #endif // ifndef DISABLE_RECURSIVE 



    }; // end Matrix class

    template<Size n, typename Scalar = float>
    Matrix<n,n,Scalar>
    Identity(){
        Matrix<n,n,Scalar> I;
        for(Size i = 0; i < n; i++)
        I[i][i]=1;
        return I;
    }

    template<Size n, Size m, typename Scalar = float>
    Matrix<n,m,Scalar>
    Zeros(){
        Matrix<n,m,Scalar> M;
        return M;
    }

    

}