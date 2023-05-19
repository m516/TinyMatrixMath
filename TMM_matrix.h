#pragma once

#include "TMM_enable_if.h"
#ifdef ARDUINO
    #include <Arduino.h>
#endif


namespace tmm{


    typedef unsigned char Size;
    typedef float Scalar;

    template<Size n, Size m>
    class Matrix{
        public:

        Scalar data[n][m];

        Matrix<n,m>(){
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            data[i][j]=0;
        }

        Matrix<n,m>(const Scalar M[n][m]){
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            data[i][j]=M[i][j];
        }

        Matrix<n,m>(const Scalar M){
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            data[i][j]=M;
        }


        // Set this matrix to the value of another matrix
        void
        operator=(const Matrix<n,m> &M){
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
        Matrix<n, m+q>
        augmentAfter(const Matrix<n,q> &other) const
        {
            Matrix<n, m+q> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j+m]=other.data[i][j];

            return M;
        }


        template<Size q> 
        Matrix<n, m+q>
        augmentBefore(const Matrix<n,q> &other) const
        {
            Matrix<n, m+q> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j+q]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=other[i][j];

            return M;
        }


        template<Size p> 
        Matrix<n+p, m>
        augmentAbove(const Matrix<p,m> &other) const
        {
            Matrix<n+p, m> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i+p][j]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=other.data[i][j];

            return M;
        }



        template<Size p> 
        Matrix<n+p, m>
        augmentBelow(const Matrix<p,m> &other) const
        {
            Matrix<n+p, m> M;
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
        Matrix<n, q>
        operator *(const Matrix<m, q> &other) const
        {
            Matrix<n, q> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < q; j++) 
            for(Size k = 0; k < m; k++) 
            M[i][j]+=data[i][k]*other.data[k][j];
            return M;
        }

        


        // Elementwise multiplication
        Matrix<n, m>
        elementwise_times(const Matrix<n,m> &other) const
        {
            Matrix<n, m> M;
            for(Size i = 0; i < n; i++)
            for(Size j = 0; j < m; j++)
            M[i][j]=data[i][j]*other.data[i][j];
            return M;
        }

        


        // Elementwise multiplication
        Matrix<n, m>
        negate() const
        {
            Matrix<n, m> M;
            for(Size i = 0; i < n; i++)
            for(Size j = 0; j < m; j++)
            M[i][j]=-data[i][j];
            return M;
        }

        



        Matrix<n,m> 
        operator +(const Matrix<n,m> &other) const
        {
            Matrix<n,m> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]+other.data[i][j];
            return M;
        }



        Matrix<n,m> 
        operator -(const Matrix<n,m> &other) const
        {
            Matrix<n,m> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]+other.data[i][j];
            return M;
        }

        

        Matrix<n,m> 
        operator +(Scalar a) const
        {
            Matrix<n,m> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]+a;
            return M;
        }

        Matrix<n,m> 
        operator -(Scalar a) const
        {
            Matrix<n,m> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]-a;
            return M;
        }

        // Multiplication by a scalar
        Matrix<n,m> 
        operator *(Scalar a) const
        {
            Matrix<n,m> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]*a;
            return M;
        }
        Matrix<n,m> 
        operator /(Scalar a) const
        {
            Matrix<n,m> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j]/a;
            return M;
        }



        Matrix<m, n>
        transpose() const
        {
            Matrix<m, n> M;
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
        Matrix<p, q>
        get(Size c, Size d) const
        {
            Matrix<p, q> M;
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
        set(Size c, Size d, Matrix<p,q> newVal)
        {
            for(Size i = 0; i < p; i++) 
            for(Size j = 0; j < q; j++) 
            data[i+c][j+d] = newVal[i][j];    
        }



        Matrix<1, m>
        row(Size i) const
        {
            return get<1, m>(i, 0);
        }

        
        Matrix<1, m>
        column(Size j) const
        {
            return get<n,1>(0, j);
        }


        void
        copyTo(Matrix<n,m> &other) const
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
                    char buf[7];
                    dtostrf(data[i][j], 6, 3, buf);
                    serial.print(buf);
                    serial.print("\t");
                }
                serial.println();
            }
        }
        #endif



    };

    template<Size n>
    Matrix<n,n>
    Identity(){
        Matrix<n,n> I;
        for(Size i = 0; i < n; i++)
        I[i][i]=1;
        return I;
    }

    template<Size n, Size m>
    Matrix<n,m>
    Zeros(){
        Matrix<n,m> M;
        return M;
    }

    

}