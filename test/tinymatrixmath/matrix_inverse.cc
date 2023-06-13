#include <gtest/gtest.h>
#include "TinyMatrixMath.hpp"
#include "float_eq.hpp"



/* TODO when inverse is implemented better

/// @brief Test identity matrix inversion on a 2x2 matrix
TEST(TMMTests, Matrix_Inversion_2x2_Identity){
  tmm::Matrix<2,2> A = tmm::Identity<2>();
  tmm::Matrix<2,2> A_inv = A.inverse();
  A_inv.printTo(std::cout);
  // Compare each element of the inverse to the expected value
  for(tmm::Size i = 0; i < 2; i++){
    for(tmm::Size j = 0; j < 2; j++){
      ASSERT_TRUE(float_eq(A_inv[i][j], i==j?1:0));
    }
  }
}



/// @brief Test matrix inversion on a 2x2 matrix
TEST(TMMTests, Matrix_Inversion_2x2){
  const float A_raw[2][2] = {
    {1, 2},
    {3, 5}
  };
  tmm::Matrix<2,2> A(A_raw);
  const float A_inv_raw[2][2] = {
    {-5,  2},
    { 3, -1}
  };
  tmm::Matrix<2,2> A_inv = A.inverse();
  A_inv.printTo(std::cout);
  // Compare each element of the inverse to the expected value
  for(tmm::Size i = 0; i < 2; i++){
    for(tmm::Size j = 0; j < 2; j++){
      ASSERT_TRUE(float_eq(A_inv[i][j], A_inv_raw[i][j]));
    }
  }
}




/// @brief Test identity matrix inversion on a 3x3 matrix
TEST(TMMTests, Matrix_Inversion_3x3_Identity){
  tmm::Matrix<3,3> A = tmm::Identity<3>();
  tmm::Matrix<3,3> A_inv = A.inverse();
  // Compare each element of the inverse to the expected value
  for(tmm::Size i = 0; i < 3; i++){
    for(tmm::Size j = 0; j < 3; j++){
      ASSERT_TRUE(float_eq(A_inv[i][j], i==j?1:0));
    }
  }
}



/// @brief Test matrix inversion on a 3x3 matrix
TEST(TMMTests, Matrix_Inversion_3x3){
  const float A_raw[3][3] = {
    {2, 0, -1},
    {5, 1,  0},
    {0, 1,  3}
  };
  tmm::Matrix<3,3> A(A_raw);
  const float A_inv_raw[3][3] = {
    {  3, -1,  1},
    {-15,  6, -5},
    {  5, -2,  2}
  };
  tmm::Matrix<3,3> A_inv = A.inverse();
  // Compare each element of the inverse to the expected value
  for(tmm::Size i = 0; i < 3; i++){
    for(tmm::Size j = 0; j < 3; j++){
      ASSERT_TRUE(float_eq(A_inv[i][j], A_inv_raw[i][j]));
    }
  }
}




/// @brief Test identity matrix inversion on a 4x4 matrix
TEST(TMMTests, Matrix_Inversion_4x4_Identity){
  tmm::Matrix<4,4> A = tmm::Identity<4>();
  tmm::Matrix<4,4> A_inv = A.inverse();
  // Compare each element of the inverse to the expected value
  for(tmm::Size i = 0; i < 4; i++){
    for(tmm::Size j = 0; j < 4; j++){
      ASSERT_TRUE(float_eq(A_inv[i][j], i==j?1:0));
    }
  }
}



/// @brief Test matrix inversion on a 4x4 matrix
TEST(TMMTests, Matrix_Inversion_4x4){
  const float A_raw[4][4] = {
    {1, 0, 0, 0},
    {2, 3, 4, 0},
    {2, 0, 2, 1},
    {1, 2, 3, 1}
  };
  tmm::Matrix<4,4> A(A_raw);
  const float A_inv_raw[4][4] = {
    { 1,    0,    0,    0  },
    { 6/5, -1/5, -4/5,  4/5},
    {-7/5,  2/5,  3/5, -3/5},
    { 4/5, -4/5, -1/5,  6/5},
  };
  tmm::Matrix<4,4> A_inv = A.inverse();
  // Compare each element of the inverse to the expected value
  for(tmm::Size i = 0; i < 4; i++){
    for(tmm::Size j = 0; j < 4; j++){
      ASSERT_TRUE(float_eq(A_inv[i][j], A_inv_raw[i][j]));
    }
  }
}


*/