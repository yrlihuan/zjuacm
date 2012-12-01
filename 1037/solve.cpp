#include <stdio.h>
#include <iostream>

using namespace std;

int main()
{
  int n;

  cin >> n;
  for (int i = 1; i <= n; ++i) {
    printf("Scenario #%d:\n", i);

    int a;
    int b;
    cin >> a >> b;

    if (a % 2 == 0 || b % 2 == 0) {
      printf("%.2f\n", (double)a*b);
    }
    else {
      printf("%.2f\n", a*b+0.41);
    }

    cout << endl;
  }
}
