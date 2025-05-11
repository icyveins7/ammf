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

template <typename T>
class VectorMedianFilter : public std::vector<T>
{
public:
  void replace(const T& in, const T& out){
    for (auto it = begin(); it != end(); ++it){
      if (*it == out)
        *it = in;
    }
  }
};


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

  printf("======= vector median filter\n");
  VectorMedianFilter<int> vmf(x.begin(), x.begin()+3);

  for (int i = 0; i < 2; ++i){
    if (i > 1)
      vmf.replace(x.at(i + 2), x.at(i - 1));
    simpleMedian(vmf);
    printf("%d\n", vmf.at(1));
  }

  return 0;
}
