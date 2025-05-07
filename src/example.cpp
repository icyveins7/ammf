#include <vector>
#include <algorithm>
#include <iostream>
#include <list>

template <typename T>
void simpleMedian(T& x)
{
  std::nth_element(
    x.begin(),
    x.begin() + x.size() / 2,
    x.end()
  );
}




int main()
{
  printf("ammf\n");

  std::vector<int> x = {1, 7, 5, 12};
  for (int i = 0; i < 2; ++i){
    std::vector<int> y(
      x.begin() + i,
      x.begin() + i + 3
    );

    simpleMedian(y);
    printf("%d\n", y.at(1));
  }

  return 0;
}
