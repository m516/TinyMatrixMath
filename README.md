# The Matrix Math library

This library is a collection of functions and classes 
for doing math on small matrices with less than 2kb of 
instruction memory and 1kb of RAM.

Matrices with less than 256 rows and 256 columns are 
supported by this library (if your device can store
a matrix with a whopping 263kb RAM!). Larger matrices
might be processed more efficiently with Eigen.

Here are some examples of how to use the library:


**Creating a 3x3 identity matrix:**
```cpp
  TMM::SquareMatrix<3> eye = TMM::Identity<3>();
```
-------------
**Creating a 3x3 matrix from a float array**
```cpp
  const TMM::Scalar A_raw[3][3] = {
  {1, 2, 3},
  {4, 5, 6},
  {9, 8, 9}
  };

  TMM::SquareMatrix<3> A(A_raw);
```
-------------
**Getting the inverse of a square matrix**
```cpp
  TMM::SquareMatrix<3> A_inv = A.inverse();
```
-------------
**Printing a matrix to Serial if the Arduio library is available**
```cpp
  A.printTo(Serial);
```
-------------
**Getting the determinant of a square matrix**
```cpp
  TMM::Scalar det_A = A.determinant();
```
-------------
**Use any sized matrix**
```cpp
  TMM::Matrix<4,5> B;         // 4 rows, 5 columns
  TMM::Matrix<5,2> C;         // 5 rows, 2 columns
  TMM::Matrix<5,2> D;         // 5 rows, 2 columns

  // Matrix-matrix multiplication
  TMM::Matrix<4,2> E = B * C; // 4 rows, 2 columns
  // Elementwise addition
  TMM::Matrix<5,2> F = C + D;
  // Elementwise multiplication
  F = C.elementwise_times(D);
  // Elementwise subtraction, and print the result to Serial
  (C - D).printTo(Serial);
```
-------------
**Invalid matrix multiplication is checked at compile-time.**
```cpp
  TMM::Matrix<4,2> G = C * B; // error: no match for 'operator*' (operand types are 'TMM::Matrix<5, 2>' and 'TMM::Matrix<4, 5>')
```
-------------
**Elementwise scalar operations are supported**
```cpp
  TMM::SquareMatrix<2> H = 5;   // set all elements to 5
  H = H+1; // add 1 to every element
  H = H-2; // reduce every element by 2
  H = H*3; // multiply every element by 3
  H = H/4; // divide every element by 4
```
-------------
**1x1 matrices can be implicitly cast to Scalars**
```cpp
  TMM::Matrix<1,1> I = 5;
  TMM::Scalar s = I; // s = 5
  s = H; // error: cannot convert 'TMM::SquareMatrix<2>' to 'TMM::Scalar' in assignment
```

