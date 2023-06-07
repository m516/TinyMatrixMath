#include <gtest/gtest.h>
#include "TinyMatrixMath.hpp"

/// @brief Edge case: create a 0x0 matrix
TEST(TMMTests, Create_0x0_Matrix) {
  tmm::Matrix<0,0> zero = tmm::Zeros<0,0>();
}


/// @brief Edge case: create a 1x1 matrix
TEST(TMMTests, Create_1x1_Matrix) {
  tmm::Matrix<1,1> zero = tmm::Zeros<1,1>();
}



/// @brief A helper function to test the zeros function
/// @tparam n the dimension of the matrix
template<tmm::Size n>
void test_zeros(){
  tmm::Matrix<n,n> zero = tmm::Zeros<n,n>();
  for(tmm::Size i = 0; i < n; i++){
    for(tmm::Size j = 0; j < n; j++){
      ASSERT_EQ(zero[i][j], 0);
    }
  }
}


/// @brief Ensure the zeros function works for all sizes
TEST(TMMTests, Check_Zeros) {
  test_zeros<0>();
  test_zeros<1>();
  test_zeros<2>();
  test_zeros<3>();
  test_zeros<4>();
  test_zeros<5>();
  test_zeros<10>();
}




/// @brief A helper function to test the identity function
/// @tparam n the dimension of the matrix
template<tmm::Size n>
void test_identity(){
  tmm::Matrix<n,n> I = tmm::Identity<n>();
  for(tmm::Size i = 0; i < n; i++){
    for(tmm::Size j = 0; j < n; j++){
      ASSERT_EQ(I[i][j], i==j?1:0);
    }
  }
}

/// @brief Ensure the Identity function works for all sizes
TEST(TMMTests, Check_Identity) {
  test_identity<0>();
  test_identity<1>();
  test_identity<2>();
  test_identity<3>();
  test_identity<4>();
  test_identity<5>();
  test_identity<10>();
}


/// @brief Tests creating a matrix from an array of scalars
TEST(TMMTests, From_Array){
  const float A_raw[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
  };

  tmm::Matrix<3,3> A(A_raw);

  for(tmm::Size i = 0; i < 3; i++){
    for(tmm::Size j = 0; j < 3; j++){
      ASSERT_EQ(A[i][j], A_raw[i][j]);
    }
  }
}

