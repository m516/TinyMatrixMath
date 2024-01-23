<h1>
  <p align="center"> 
  <img src=https://raw.githubusercontent.com/m516/TinyMatrixMath/main/docs/banner.png> 
  </p>
</h1>

<p align="center"> 
  <img src=https://github.com/m516/TinyMatrixMath/actions/workflows/cmake_docs.yml/badge.svg>
  <img src=https://github.com/m516/TinyMatrixMath/actions/workflows/cmake_tests.yml/badge.svg>
</p>

<p align="center"> 
<img src=https://github.com/m516/TinyMatrixMath/actions/workflows/arduino_compile_examples.yml/badge.svg>
<img src=https://github.com/m516/TinyMatrixMath/actions/workflows/arduino_lint.yml/badge.svg>
</p>

<p align="center"> 
<img src=https://img.shields.io/github/issues/m516/TinyMatrixMath>
<img src=https://img.shields.io/github/forks/m516/TinyMatrixMath>
<img src=https://img.shields.io/github/stars/m516/TinyMatrixMath>
<img src=https://img.shields.io/github/license/m516/TinyMatrixMath>
<a href="https://registry.platformio.org/libraries/m516/TinyMatrixMath"><img src=https://badges.registry.platformio.org/packages/m516/library/TinyMatrixMath.svg></a>
</p>


This library is a collection of functions and classes 
for doing math on small matrices with less than 2kb of 
instruction memory and 1kb of RAM.

It is available as:
* a library in the [Arduino Library Manager](https://www.arduino.cc/reference/en/libraries/tinymatrixmath/) and the [PlatformIO registry](https://registry.platformio.org/libraries/m516/TinyMatrixMath)
* a CMake library (`TARGET_LINK_LIBRARIES (${EXECUTABLE_NAME} tinymatrixmath)`) compatible with C++ 11+

It sacrifices some of the power of more feature-rich libraries like 
[Basic Linear Algebra](https://github.com/tomstewart89/BasicLinearAlgebra)
or [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) to 
minimize memory footprint and standardize matrix storage across devices, 
possibly simplifying the process of passing matrices around across devices.

The [Sheldrake](https://github.com/m516/sheldrake) library employs this library to send matrices between
desktop environments and microcontrollers.


--------------------

<h2>
  <p align="center"> Features </p>
</h2>

Despite this library being "tiny", it supports many common matrix operations including:

- matrix-scalar
  - addition
  - subtraction
  - multiplication
  - division
- matrix-matrix
  - addition
  - subtraction
  - multiplication
  - elementwise multiplication
- negation
- transpose
- cofactor
- determinant
- 🚧 inverse (implemented, not working) 
- 🚧 eigenvalues and eigenvectors
- 🚧 characteristic polynomial

*Elements with 🚧 are not yet stable or implemented.*

Most common mistakes with matrix operations are checked for at compile-time,
so there's no need to worry about deploying code with the following issues:
* matrices with invalid dimensions for matrix-matrix multiplication
* matrices with mismatching dimensions for elementwise operators
* taking the determinant, cofactor, and inverse of non-square matrices.

Matrices with less than 256 rows and 256 columns are 
supported by this library (if your device can store
a matrix with a whopping 263kb RAM!). Larger matrices
might be processed more efficiently with Eigen.


--------------------

<h2>
  <p align="center"> Examples </p>
</h2>

Here are some examples of how to use the library:

**Including the library**
```cpp
#include "TinyMatrixMath.hpp"
```

**Creating a 3x3 identity matrix:**
```cpp
  tmm::Matrix<3,3> eye = tmm::Identity<3>();
```

-------------

**Creating a 3x3 matrix from a float array**
```cpp
  const tmm::Scalar A_raw[3][3] = {
  {1, 2, 3},
  {4, 5, 6},
  {9, 8, 9}
  };

  tmm::Matrix<3,3> A(A_raw);
```

-------------

**Getting the inverse of a square matrix**
```cpp
  tmm::Matrix<3,3> A_inv = A.inverse();
```

-------------

**Printing a matrix to Serial if the Arduio library is available**
```cpp
  A.printTo(Serial); // Arduino
  A.printTo(std::cout); // CMake
```

-------------

**Getting the determinant of a square matrix**
```cpp
  tmm::Scalar det_A = A.determinant();
```

-------------

**Use any sized matrix**
```cpp
  tmm::Matrix<4,5> B;         // 4 rows, 5 columns
  tmm::Matrix<5,2> C;         // 5 rows, 2 columns
  tmm::Matrix<5,2> D;         // 5 rows, 2 columns

  // Matrix-matrix multiplication
  tmm::Matrix<4,2> E = B * C; // 4 rows, 2 columns
  // Elementwise addition
  tmm::Matrix<5,2> F = C + D;
  // Elementwise multiplication
  F = C.elementwise_times(D);
  // Elementwise subtraction, and print the result to Serial
  (C - D).printTo(Serial);    // Arduino
  (C - D).printTo(std::cout); // CMake
```

-------------

**Invalid matrix multiplication is checked at compile-time.**
```cpp
  tmm::Matrix<4,2> G = C * B; // error: no match for 'operator*' (operand types are 'tmm::Matrix<5, 2, float>' and 'tmm::Matrix<4, 5, float>')
```

-------------

**Elementwise scalar operations are supported**
```cpp
  tmm::Matrix<2> H = 5;   // set all elements to 5
  H = H+1; // add 1 to every element
  H = H-2; // reduce every element by 2
  H = H*3; // multiply every element by 3
  H = H/4; // divide every element by 4
```

-------------

**1x1 matrices can be implicitly cast to Scalars**
```cpp
  tmm::Matrix<1,1> I = 5;
  tmm::Scalar s = I; // s = 5
  s = H; // error: cannot convert 'tmm::Matrix<2,2,float>' to 'float' in assignment
```



<h2>
  <p align="center"> More examples </p>
</h2>

* Arduino examples can be found in the "examples" folder
* Examples for CMake project can be found in the "cmake_examples" folder


<h2>
  <p align="center"> Using this Repo as a Template </p>
</h2>

The following projects were initially generated from this template:
* [TinySerializer](https://github.com/m516/TinySerializer)
* [TinyControlPolicies](https://github.com/m516/TinyControlPolicies)

For making your own CMake/Arduino library from this template, complete the following steps:
* Hit the "Use this template" button on the [GitHub repo](https://github.com/m516/TinyMatrixMath) to copy this project to your account.
* Delete the source code (under `src`) and add your own.
* In the root-level CMakeLists.txt
  * On the line containing `project ("tinymatrixmath" C CXX)`, replace `tinymatrixmath` wtih the name of your project. We'll use this name later.
  * There is a list of files under the line containing `add_library (${PROJECT_NAME}`. Replace that list with a list of the files that belong to your new library.
* In `.github/workflows/cmake_docs.yml`, there is a line containing `make tinymatrixmath_doxygen -j $(nproc)`. Replace `tinymatrixmath` with the name of your project.
* For automatically publishing documentation:
  * In the root folder of a clone of your new repo, Run the following code snippet to make a new, blank `docs` branch:
    ```sh
    git switch --orphan docs
    git commit --allow-empty -m "Initial commit on orphan branch"
    git push -u origin docs
    ```
  * On GitHub, in this repo,
    * under `settings -> Pages -> Build and deployment -> Branch`, select the `docs` branch.
    * under `settings -> Actions -> General -> Workflow Permissions`, select the `Read and write permissions`  radio button to allow the docs workflow to push to the `docs` branch.
* Clear the `examples` folder and place all your Arduino example sketches there.
* Clear the `cmake_examples` folder and place all your CMake example executables there. Update `cmake_examples/CmakeLists.txt` accordingly.
* Clear the `test` folder and place all your unit tests there. Update `test/CMakeLists.txt` accordingly.
* After testing your new library thoroughly, add it to the Arduino Library Manager by following the [official instructions](https://github.com/arduino/library-registry#adding-a-library-to-library-manager)

And you're finally done!

If you run into any issues along the way, file them [here](https://github.com/m516/TinyMatrixMath/issues)
