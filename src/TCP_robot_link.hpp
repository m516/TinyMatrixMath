    #pragma once
    #include "TMM_matrix.hpp"
    

    /// @brief Emulate a link of a robot arm, using modified DH parameters. 
    ///  Can be cast into a Matrix to get its transformation.
    ///  It's slightly out of scope, but I made it for another project 
    ///  earlier and liked it
    /// @tparam Scalar the data type to use for the parameters
    template<typename Scalar>
    class Link{
        public:
        Scalar distance;
        Scalar twist;
        Scalar offset;
        Scalar angle;

        operator Matrix<4,4,Scalar>() const{
            Matrix<4,4,Scalar> t;
            t[0][0]=cos(angle);                      t[0][1]=-sin(angle);                    t[0][2]=0;                t[0][3]=distance;
            t[1][0]=sin(angle)*cos(twist);           t[1][1]=cos(angle)*cos(twist);          t[1][2]=-sin(twist);      t[1][3]=-offset*sin(twist);
            t[2][0]=sin(angle)*sin(twist);           t[2][1]=cos(angle)*sin(twist);          t[2][2]=cos(twist);       t[2][3]=offset*cos(twist);
            t[3][0]=0;                               t[3][1]=0;                              t[3][2]=0;                t[3][3]=1;
            return t;
        }
    };