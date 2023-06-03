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














    template<Size n, Size m, typename Scalar>
    class Matrix;

    template<Size n, Size m, typename Scalar, typename ThisType>
    class AbstractMatrix;
    template<Size n, Size m, typename Scalar, typename Type_A, typename Type_B>
    class DeferredOperatorAddition;

    template<Size n, Size m, typename Scalar, typename ThisType>
    class AbstractMatrix{
        public:
        virtual Matrix<n,m,Scalar> evaluate() const = 0;
        virtual void evaluate(Matrix<n,m,Scalar> &dest) const = 0;


        

        operator Matrix<n,m,Scalar>() const {
            return evaluate();
        }
    };

    template<Size n, Size m, typename Scalar, typename Type_A, typename Type_B, typename ThisType>
    class DeferredOperatorMatrix: public AbstractMatrix<n,m,Scalar,DeferredOperatorMatrix<n,m,Scalar,Type_A,Type_B,ThisType>>{

        public:
        DeferredOperatorMatrix(const Type_A &a, const Type_B &b): a(a),b(b){}
        const Type_A &a;
        const Type_B &b;
        virtual Matrix<n,m,Scalar> evaluate() const = 0;
        virtual void evaluate(Matrix<n,m,Scalar> &dest) const = 0;
    };

    template<Size n, Size m, typename Scalar, typename Type_A, typename Type_B>
    class DeferredOperatorAddition: 
        public DeferredOperatorMatrix<n,m,Scalar,Type_A,Type_B,DeferredOperatorAddition<n,m,Scalar,Type_A,Type_B>>
    {
        public:
        DeferredOperatorAddition (const Type_A &a, const Type_B &b): 
        DeferredOperatorMatrix<n,m,Scalar,Type_A,Type_B,DeferredOperatorAddition<n,m,Scalar,Type_A,Type_B>>(a,b){}
        virtual Matrix<n,m,Scalar> evaluate() const {
            Matrix<n,m,Scalar> result;
            add(this->a.evaluate(), this->b.evaluate(), result);
            return result;
        }
        virtual void evaluate(Matrix<n,m,Scalar> &dest) const {
            // If incrementing ( A <- B + A )
            if (&dest == (Matrix<n,m,Scalar>*)&(this->a)){
                // If B is a Matrix type
                if  constexpr (SameType<Type_B, Matrix<n,m,Scalar>>::value) {
                    this->a.evaluate(dest);
                    add(dest, (Matrix<n,m,Scalar>)(this->b), dest);
                    return;
                }
                // If B is derived
                else {
                    this->a.evaluate(dest);
                    add(dest, this->b.evaluate(), dest);
                    return;
                }
            } // End if incrementing A
            // TODO check if dest is b
            // a.evaluate(dest);
            // b.evaluate(dest);
            add(this->a.evaluate(), this->b.evaluate(), dest);
        }
    };

    // Let Matricies get added together
    template<Size n, Size m, typename Scalar, typename Type1, typename Type2>
    DeferredOperatorAddition<n, m, Scalar, AbstractMatrix<n,m,Scalar,Type1>, AbstractMatrix<n,m,Scalar,Type2>>
    operator +(const AbstractMatrix<n,m,Scalar,Type1> &a, const AbstractMatrix<n,m,Scalar,Type2> &b) {
        return DeferredOperatorAddition<n, m, Scalar, AbstractMatrix<n,m,Scalar,Type1>, AbstractMatrix<n,m,Scalar,Type2>>(a, b);
    } 







    // template<Size n, Size m, typename Scalar_A, typename Scalar_B>
    // class matrix_add {
    //     public:
    //     matrix_add(Matrix<n,m,Scalar_A> const& a, Matrix<n,m,Scalar_B> const& b) : a(a), b(b) { }
    //     operator Matrix<n,m,Scalar_A>() const {
    //         Matrix<n,m,Scalar_A> result;
    //         add(a, b, result);
    //         return result;
    //     }
    //     private:
    //     Matrix<n,m,Scalar_A> const& a;
    //     Matrix<n,m,Scalar_B> const& b;
    // };
    // template<Size n, Size m, typename Scalar_A = float, typename Scalar_B = float>
    // matrix_add<n, m, Scalar_A, Scalar_B> operator +(const Matrix<n,m,Scalar_A> &a, Matrix<n,m,Scalar_B> const& b) {
    //     return matrix_add(a, b);
    // }






















    template<Size n, Size m, typename Scalar = float>
    class Matrix: public AbstractMatrix<n,m,Scalar,Matrix<n,m,Scalar>>{

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


        // // Set this matrix to the value of another matrix
        // template<typename Other_Scalar_Type = float> 
        // void
        // operator=(const Matrix<n,m,Other_Scalar_Type> &M){
        //     for(Size i = 0; i < n; i++) 
        //     for(Size j = 0; j < m; j++) 
        //     data[i][j]=M.data[i][j];
        // }





        template<typename T>
        void
        operator=(AbstractMatrix<n,m,Scalar,T> &other){
            other.evaluate(*this);
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

        
        /// @brief Casts a matrix to a matrix of a different type.
        /// For example, a Matrix<2,2,float> can be cast to a Matrix<2,2,double>.
        /// If no template arguments are given, the default is to cast to a Matrix of floats.
        /// @tparam Other_Scalar_Type 
        template<typename Other_Scalar_Type = float> 
        operator Matrix<n,m,Other_Scalar_Type>() const
        {
            Matrix<n,m,Other_Scalar_Type> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j];
            return M;
        }


        /// @brief Concatenates two matrices horizontally. The other matrix is placed to the right of (after) this one.
        /// @tparam Other_Scalar_Type 
        /// @tparam q 
        /// @param other The matrix to concatenate to the right of this one
        /// @return The augmented matrix
        template<Size q, typename Other_Scalar_Type> 
        Matrix<n,m+q,Scalar>
        augmentAfter(const Matrix<n,q,Other_Scalar_Type> &other) const
        {
            Matrix<n,m+q,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j+m]=other.data[i][j];

            return M;
        } // end augmentAfter



        /// @brief Concatenates two matrices horizontally. The other matrix is placed to the left of (before) this one.
        /// @tparam Other_Scalar_Type 
        /// @tparam q 
        /// @param other The matrix to concatenate to the right of this one
        /// @return The augmented matrix
        template<Size q, typename Other_Scalar_Type> 
        Matrix<n,m+q,Scalar>
        augmentBefore(const Matrix<n,q,Other_Scalar_Type> &other) const
        {
            Matrix<n,m+q,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j+q]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=other[i][j];

            return M;
        } // end augmentBefore




        /// @brief Concatenates two matrices horizontally. The other matrix is placed to the right of (after) this one.
        /// @tparam Other_Scalar_Type 
        /// @tparam q 
        /// @param other The matrix to concatenate to the right of this one
        /// @return The augmented matrix
        template<Size p, typename Other_Scalar_Type> 
        Matrix<n+p,m,Scalar>
        augmentAbove(const Matrix<p,m,Other_Scalar_Type> &other) const
        {
            Matrix<n+p,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i+p][j]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=other.data[i][j];

            return M;
        } // end augmentAbove


        /// @brief Concatenates two matrices vertically. The other matrix is placed below this one.
        /// @tparam Other_Scalar_Type 
        /// @tparam q 
        /// @param other The matrix to concatenate below this one
        /// @return The augmented matrix
        template<Size p, typename Other_Scalar_Type> 
        Matrix<n+p,m,Scalar>
        augmentBelow(const Matrix<p,m,Other_Scalar_Type> &other) const
        {
            Matrix<n+p,m,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i][j]=data[i][j];

            
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[i+n][j]=other.data[i][j];

            return M;
        }  // end augmentBelow













        // Elementwise negation
        Matrix<n,m,Scalar>
        negate() const
        {
            Matrix<n,m,Scalar> M;
            for(Size i = 0; i < n; i++)
            for(Size j = 0; j < m; j++)
            M[i][j]=-data[i][j];
            return M;
        } // end negate

        



        Matrix<m,n,Scalar>
        transpose() const
        {
            Matrix<m,n,Scalar> M;
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            M[j][i]=data[i][j];
            return M;
        } // end transpose






















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
        } // end get

        void
        set(Size i, Size j, Scalar newVal)
        {
            data[i][j] = newVal;
        } // end set

        
        template<Size p, Size q>
        void
        set(Size c, Size d, Matrix<p,q,Scalar> newVal)
        {
            for(Size i = 0; i < p; i++) 
            for(Size j = 0; j < q; j++) 
            data[i+c][j+d] = newVal[i][j];    
        } // end set



        Matrix<1,m,Scalar>
        row(Size i) const
        {
            return get<1, m>(i, 0);
        } // end row

        
        Matrix<1,m,Scalar>
        column(Size j) const
        {
            return get<n,1>(0, j);
        } // end column


        void
        copyTo(Matrix<n,m,Scalar> &other) const
        {
            for(Size i = 0; i < n; i++) 
            for(Size j = 0; j < m; j++) 
            other[i][j]=data[i][j];
        } // end copyTo

        /// @brief Returns a copy of this matrix. Required for AbstractMatrix
        /// @return a copy of this matrix
        virtual Matrix<n,m,Scalar> evaluate() const {
            return *this;
        }

        /// @brief Returns a copy of this matrix. Required for AbstractMatrix
        /// @return a copy of this matrix
        virtual void evaluate(Matrix<n,m,Scalar> &other) const {
            copyTo(other);
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
        } // end printTo
        #endif






















        #ifndef TMM_DISABLE_RECURSIVE

        /// @brief Calculates the determinant of a square matrix
        /// @return the determinant of the matrix
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




        /// @brief Calculates the cofactor matrix of a square matrix
        /// @return The cofactor matrix of the matrix
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




        /// @brief Calculates the inverse of a square matrix (WIP)
        /// @return The inverse of the matrix
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



    /// @brief Elementwise addition of two matrices
    /// @tparam Scalar_A      The type of the first matrix
    /// @tparam Scalar_B      The type of the second matrix
    /// @tparam Scalar_Dest   The type of the result matrix
    /// @tparam n             The number of rows in each matrix
    /// @tparam m             The number of columns in each matrix
    /// @param A              The first matrix
    /// @param B              The second matrix
    /// @param dest           A reference to the Matrix to store the result of A + B
    template<Size n, Size m, typename Scalar_A = float, typename Scalar_B = float, typename Scalar_Dest = float>
    void
    add(const Matrix<n,m,Scalar_A> &A, const Matrix<n,m,Scalar_B> &B, Matrix<n,m,Scalar_Dest> &dest){
        for(Size i = 0; i < n; i++)
        for(Size j = 0; j < m; j++)
        dest[i][j]=A.data[i][j]+B.data[i][j];
    } // end add

    /// @brief Elementwise addition of two matrices
    /// @tparam Scalar_A      The source datatype
    /// @tparam Scalar_Dest   The type of the result matrix
    /// @tparam n             The number of rows in each matrix
    /// @tparam m             The number of columns in each matrix
    /// @param A              The first matrix
    /// @param B              The second matrix
    /// @param dest           A reference to the Matrix to store the result of A + B
    template<Size n, Size m, typename Scalar_A = float, typename Scalar_Dest = float>
    void
    add(const Matrix<n,m,Scalar_A> &A, const Scalar_A &B, Matrix<n,m,Scalar_Dest> &dest){
        for(Size i = 0; i < n; i++)
        for(Size j = 0; j < m; j++)
        dest[i][j]=A[i][j]+B;
    } // end add


    /// @brief Elementwise subtraction of two matrices
    /// @tparam Scalar_A      The type of the first matrix
    /// @tparam Scalar_B      The type of the second matrix
    /// @tparam Scalar_Dest   The type of the result matrix
    /// @tparam n             The number of rows in each matrix
    /// @tparam m             The number of columns in each matrix
    /// @param A              The first matrix
    /// @param B              The second matrix
    /// @param dest           A reference to the Matrix to store the result of A - B
    template<Size n, Size m, typename Scalar_A = float, typename Scalar_B = float, typename Scalar_Dest = float>
    void
    subtract(const Matrix<n,m,Scalar_A> &A, const Matrix<n,m,Scalar_B> &B, Matrix<n,m,Scalar_Dest> &dest){
        for(Size i = 0; i < n; i++)
        for(Size j = 0; j < m; j++)
        dest[i][j]=A[i][j]-B[i][j];
    } // end subtract


    /// @brief Elementwise multiplication of two matrices
    /// @tparam Scalar_A      The type of the first matrix
    /// @tparam Scalar_B      The type of the second matrix
    /// @tparam Scalar_Dest   The type of the result matrix
    /// @tparam n             The number of rows in each matrix
    /// @tparam m             The number of columns in each matrix
    /// @param A              The first matrix
    /// @param B              The second matrix
    /// @param dest           A reference to the Matrix to store the result of A .* B
    template<Size n, Size m, typename Scalar_A = float, typename Scalar_B = float, typename Scalar_Dest = float>
    void
    elementwise_multiply(const Matrix<n,m,Scalar_A> &A, const Matrix<n,m,Scalar_B> &B, Matrix<n,m,Scalar_Dest> &dest){
        for(Size i = 0; i < n; i++)
        for(Size j = 0; j < m; j++)
        dest[i][j]=A[i][j]*B[i][j];
    } // end elementwise_multiply


    /// @brief Elementwise division of two matrices
    /// @tparam Scalar_A      The type of the first matrix
    /// @tparam Scalar_B      The type of the second matrix
    /// @tparam Scalar_Dest   The type of the result matrix
    /// @tparam n             The number of rows in each matrix
    /// @tparam m             The number of columns in each matrix
    /// @param A              The first matrix
    /// @param B              The second matrix
    /// @param dest           A reference to the Matrix to store the result of A ./ B
    template<Size n, Size m, typename Scalar_A = float, typename Scalar_B = float, typename Scalar_Dest = float>
    void
    elementwise_divide(const Matrix<n,m,Scalar_A> &A, const Matrix<n,m,Scalar_B> &B, Matrix<n,m,Scalar_Dest> &dest){
        for(Size i = 0; i < n; i++)
        for(Size j = 0; j < m; j++)
        dest[i][j]=A[i][j]/B[i][j];
    } // end elementwise_divide


    /// @brief Matrix-matrix multiplication
    /// @tparam n the number of rows in the first matrix
    /// @tparam m the number of columns in the first matrix and the number of rows in the second matrix
    /// @tparam q the number of columns in the second matrix
    /// @tparam Scalar_A the type of the first matrix
    /// @tparam Scalar_B the type of the second matrix
    /// @tparam Scalar_Dest the type of the result matrix
    /// @param A the first matrix
    /// @param B the second matrix
    /// @param dest The result of A*B
    template<Size n, Size m, Size q, typename Scalar_A = float, typename Scalar_B = float, typename Scalar_Dest = float>
    void
    multiply(const Matrix<n,m,Scalar_A> &A, const Matrix<m,q,Scalar_B> &B, Matrix<n,q,Scalar_Dest> &dest){
        for(Size i = 0; i < n; i++)
        for(Size j = 0; j < q; j++){
            dest[i][j]=0;
            for(Size k = 0; k < m; k++)
                dest[i][j]+=A[i][k]*B[k][j];
        }
    } // end multiply






    
} // end namespace tmm