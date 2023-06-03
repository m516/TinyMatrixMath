#include <TinyMatrixMath.hpp>



int  main() {
  // Creating a 3x3 identity matrix:
  tmm::Matrix<3,3,double> eye = tmm::Identity<3>();

  tmm::Matrix<3,3,double> eye2 = tmm::Identity<3>();

  auto eye3 = eye + eye2 + eye2 + eye + eye;
  auto eye4 = eye2 + eye;
  tmm::Matrix<3,3,double> eye5 = eye4 + eye2;

  eye5.printTo(std::cout);

  // tmm::Matrix<3,3,double> eye4 = ((eye + eye2) + eye2);
  
  
  // //------------------------------------//
  // // Creating a 3x3 matrix from a float array
  // const float A_raw[3][3] = {
  // {1, 2, 3},
  // {4, 5, 6},
  // {9, 8, 9}
  // };
  
  // tmm::Matrix<3,3> A(A_raw);
  
  
  // //------------------------------------//
  // // Getting the inverse of a square matrix
  // tmm::Matrix<3,3> A_inv = A.inverse();
  
  
  // //------------------------------------//
  // // Printing a matrix to Serial if the Arduio library is available
  // A.printTo(std::cout);
  
  
  // //------------------------------------//
  // // Getting the determinant of a square matrix
  // float det_A = A.determinant();
  
  
  // //------------------------------------//
  // // Use any sized matrix
  // tmm::Matrix<4,5> B;         // 4 rows, 5 columns
  // tmm::Matrix<5,2> C;         // 5 rows, 2 columns
  // tmm::Matrix<5,2> D;         // 5 rows, 2 columns
  
  // // Matrix-matrix multiplication
  // tmm::Matrix<4,2> E = B * C; // 4 rows, 2 columns
  // // Elementwise addition
  // tmm::Matrix<5,2> F = C + D;
  // // Elementwise multiplication
  // F = C.elementwise_times(D);
  // // Print the result of subtraction
  // (C + D).printTo(std::cout);
  
  
  // //------------------------------------//
  // // Invalid matrix multiplication is checked at compile-time.
  // // tmm::Matrix<4,2> G = C * B; // error: no match for 'operator*' (operand types are 'tmm::Matrix<5, 2>' and 'tmm::Matrix<4, 5>')
  
  
  // //------------------------------------//
  // // Elementwise scalar operations are supported
  // tmm::Matrix<2,2> H = 5;   // set all elements to 5
  // H = H+1; // add 1 to every element
  // H = H-2; // reduce every element by 2
  // H = H*3; // multiply every element by 3
  // H = H/4; // divide every element by 4
  
  
  // //------------------------------------//
  // // 1x1 matrices can be implicitly cast to Scalars
  // tmm::Matrix<1,1> I = 5;
  // float s = I; // s = 5
  // // s = H; // error: cannot convert 'tmm::Matrix<2>' to 'tmm::Scalar' in assignment


  // //------------------------------------//
  // // Using multiple datatypes
  // tmm::Matrix<2,2,long> J = 5;
  // tmm::Matrix<2,2,double> K = 5;
  // tmm::Matrix<2,2,int> L = J * K;
  // auto M = J * K;
}

void loop() {
// put your main code here, to run repeatedly:

}
