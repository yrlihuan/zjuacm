#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include <ctime>
#include <random>

// a test according to http://en.wikipedia.org/wiki/Memory_barrier
void f1(volatile int &f, int &x)
{
  while (f == 0)
    ;

  std::cout << x << std::endl;
}

void f2(volatile int &f, int &x)
{
  std::default_random_engine generator;
  generator.seed(std::time(0));
  std::uniform_int_distribution<long> distribution(1000, 10000);
  long minloops = distribution(generator);

  //int minloops = int(float(std::rand()) / RAND_MAX * 1e5);
  for (long i = 0; i < minloops; ++i)
    minloops -= 1;

  x = minloops;
  f = 1;
}

int main()
{
  volatile int f = 0;
  int x = 0;
  std::thread t1(f1, std::ref(f), std::ref(x));
  std::thread t2(f2, std::ref(f), std::ref(x));
  t1.join();
  t2.join();

  return 0;
}
