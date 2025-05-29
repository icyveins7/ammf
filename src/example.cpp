#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "../include/linkedlistmedian.h"


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
  // Inherit all constructors
  using std::vector<T>::vector;

  void replace(const T& in, const T& out){
    for (size_t i = 0; i < this->size(); ++i){
      if (this->at(i) == out){
        this->at(i) = in;
        printf("Replaced at index %zd\n", i);
        break;
      }
    }
  }
};

// just a quick function to generate some data
std::vector<int> quickrandvec(size_t len){
  std::vector<int> x(len);
  // fill random values
  for (auto& v : x){
    v = std::rand() % 10;
  }
  return x;
}

// and a quick function to print a generic container
template <typename Iterator>
void quickprint(Iterator begin, Iterator end){
  for (auto it = begin; it != end; ++it){
    printf("%d ", *it);
  }
  printf("\n");
}

int main(int argc, char* argv[])
{
  printf("ammf\n");

  size_t len = 4;
  size_t medlen = 3;
  if (argc >= 2)
    len = (size_t)std::stoi(argv[1]);
  if (argc >= 3)
    medlen = (size_t)std::stoi(argv[2]);
  printf("Using length %zd\n", len);
  printf("Using median length %zd\n", medlen);
  size_t iterations = len - medlen + 1;

  std::vector<int> x = quickrandvec(len);
  quickprint(x.begin(), x.end());

  std::vector<int> truth(iterations);

  // benchmark ground truth
  printf("======= just nth_element \n");
  for (int i = 0; i < iterations; ++i){
    std::vector<int> y(
      x.begin() + i,
      x.begin() + i + medlen
    );

    quickprint(y.begin(), y.end());
    simpleMedian(y);
    printf("Median: %d\n", y.at(medlen/2));
    truth.at(i) = y.at(medlen/2);
  }

  printf("======= vector median filter\n");
  VectorMedianFilter<int> vmf(x.begin(), x.begin()+medlen);

  for (int i = 0; i < iterations; ++i){
    if (i >= 1)
      vmf.replace(x.at(i + medlen-1), x.at(i - 1));
    
    quickprint(vmf.begin(), vmf.end());

    simpleMedian(vmf);
    printf("Median: %d\n", vmf.at(medlen/2));
    if (truth.at(i) != vmf.at(medlen/2)){
      printf("ERROR: %d != %d\n", truth.at(i), vmf.at(medlen/2));
    }
  }

  printf("======= linked list derived class\n");
  LinkedListMedian<int> llmf(x.begin(), x.begin()+medlen);

  for (int i = 0; i < iterations; ++i){
    // fix the median length
    if (i == 0)
      llmf.median();

    else{
      llmf.replace(
        x.at(i+medlen-1),
        x.at(i-1)
      );
    }

    quickprint(llmf.begin(), llmf.end());

    int med = llmf.median();
    printf("Median: %d\n", med);
    if (truth.at(i) != med){
      printf("ERROR: %d != %d\n", truth.at(i), med);
    }
  }
  return 0;
}
