#include <TinyMatrixMath.hpp>

#include <chrono>
#include <random>


template<unsigned char n>
tmm::Matrix<n,n,float> random(){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,10); // distribution in range [1, 10]
    tmm::Matrix<n,n,float> r = tmm::Zeros<n,n,float>();
    for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) r.data[i][j] = dist(rng);
    return r;
}

// Adapted from: https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
template<unsigned char n>
void benchmark_adds(){

  const int num_trials = 1000000;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    // Create the matrices
    std::vector<tmm::Matrix<n,n,float>> matrices(num_trials);
    for(int i = 0; i < num_trials; i++) matrices.push_back(random<n>());

    // Create the counter
    tmm::Matrix<n,n,float> counter = random<n>();
    

    auto t1 = high_resolution_clock::now();

    // Benchmark
    for(int i = 0; i < num_trials; i++) counter = counter + matrices[i] + tmm::Identity<n>();

    // End of benchmark
    auto t2 = high_resolution_clock::now();

    counter.printTo(std::cout);

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
}


int  main() {
  std::cout << "---------------------------- 3 -------------------------" << std::endl;
  benchmark_adds<3>();



  std::cout << "---------------------------- 4 -------------------------" << std::endl;
  benchmark_adds<4>();



  std::cout << "---------------------------- 5 -------------------------" << std::endl;
  benchmark_adds<5>();



  std::cout << "---------------------------- 6 -------------------------" << std::endl;
  benchmark_adds<6>();



  std::cout << "---------------------------- 7 -------------------------" << std::endl;
  benchmark_adds<7>();



  std::cout << "---------------------------- 8 -------------------------" << std::endl;
  benchmark_adds<8>();



  std::cout << "---------------------------- 9 -------------------------" << std::endl;
  benchmark_adds<9>();



  std::cout << "---------------------------- 10 -------------------------" << std::endl;
  benchmark_adds<10>();






  return 0;



}
