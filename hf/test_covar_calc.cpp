#include <iostream>
#include <string>
#include <random>

using std::string;

class CumSum {
  public:
    double add(double v) {
        return cumsum_ += v;
    }

    double get() const {
        return cumsum_;
    }

  private:
    double cumsum_ = 0.0;
};

int main(int argc, char* argv[]) {
    CumSum s00;
    CumSum s11;
    CumSum s01;
    CumSum s10;

    int n = 100000;
    int seed = 0;
    int v1 = 123;
    if (argc > 1) {
        n = std::atoi(argv[1]);
    }

    if (argc > 2) {
        seed = std::atoi(argv[2]);
    } else {
        std::random_device rd;
        seed = rd();
    }

    if (argc > 3) {
        v1 = std::atoi(argv[3]);
    }

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(0, 1000000);

    for (int i = 0; i < n; ++i) {
        int x = dist(gen);
        int y = v1;
        s00.add(1);
        s11.add(x * y);
        s01.add(x);
        s10.add(y);
    }

    double covar = (s11.get() * s00.get() - s01.get() * s10.get()) / (n * n);

    std::cout << "covar: " << covar << std::endl;
    std::cout << "covar is zero: " << (covar == 0) << std::endl;

    return 0;

}
