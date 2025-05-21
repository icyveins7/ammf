#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <list>

template <typename T>
class LinkedListMedian : public std::list<T>
{
public:
  using std::list<T>::list;

  void insertSorted(const T val){
    if (this->size() == 0) {
      this->push_back(val);
      return;
    }
    else {
      // walk to find insertion point
      for (auto it = this->begin(); it != this->end(); ++it) {
        if (*it >= val) {
          // insert before this
          this->insert(it, val);
          return;
        }
      }
    }
    // TODO: move iterator based on inserted position

    // if we haven't found insertion then push to end
    this->push_back(val);

    // we only need to move iterator when the new size is even
    if (m_medianItExists && this->size() % 2 == 0)
      m_medianIt++;
      
  }

  void eraseOne(const T val) {
    auto it = std::find(this->begin(), this->end(); val);
    if (it == this->end())
      throw std::runtime_error("eraseFirst failed to find element");
    this->erase(it);

    // TODO: move iterator based on erased position

    // move the iterator if new size is odd
    if (m_medianItExists && this->size() % 2 == 1)
      m_medianIt--;
  }

  T median() const {
    // walk halfway on first call?
    if (!m_medianItExists){
      m_medianIt = this->begin();
      std::advance(m_medianIt, this->size()/2);
      m_medianItExists = true;
    }
    return *m_medianIt;
  }

private:
  std::list<T>::iterator m_medianIt;
  bool m_medianItExists;
  
}

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
      }
    }
  }
};


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

  std::vector<int> x(len);
  // fill random values
  for (auto& v : x){
    v = std::rand() % 10;
    printf("%d ", v);
  }
  printf("\n");


  printf("======= just nth_element \n");
  for (int i = 0; i < iterations; ++i){
    std::vector<int> y(
      x.begin() + i,
      x.begin() + i + medlen
    );

    simpleMedian(y);
    printf("Median: %d\n", y.at(medlen/2));
  }

  printf("======= vector median filter\n");
  VectorMedianFilter<int> vmf(x.begin(), x.begin()+medlen);

  for (int i = 0; i < iterations; ++i){
    if (i >= 1)
      vmf.replace(x.at(i + medlen-1), x.at(i - 1));
    for (int j = 0; j < vmf.size(); ++j)
      printf("%d ", vmf.at(j));
    printf("\n");

    simpleMedian(vmf);
    printf("Median: %d\n", vmf.at(medlen/2));
  }

  printf("======= linked list derived class\n");
  LinkedListMedian<int> llmf(x.begin(), x.begin()+medlen);

  for (int i = 0; i < iterations; ++i){
    if (i >= 1){
      lmmf.eraseOne(x.at(i - 1));
      lmmf.insertSorted(x.at(i + medlen - 1));
    }
  }
  return 0;
}
