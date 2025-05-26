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
    // we cannot insert once we have settled on the median length
    if (m_medianLength >= 0)
      throw std::runtime_error("cannot insert once median length fixed. use replace instead");


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
    // if we haven't found insertion then push to end
    this->push_back(val);
  }

  void replace(const T toIns, const T toRem){
    if (m_medianLength < 0)
      throw std::runtime_error("must call median() at least once to finalise median length before replacing");

    std::cout << "Removing " << toRem << std::endl;
    std::cout << "Inserting " << toIns << std::endl;
    // convention is to use geq
    const T oldMed = this->median();
    const bool insIsHigher = toIns >= oldMed;
    const bool remIsHigher = toRem >= oldMed;
    printf("inserted >= median ? %s\n", insIsHigher ? "true" : "false");
    printf("removed >= median ? %s\n", remIsHigher ? "true" : "false");
    bool inserted = false, removed = false;
    // 4 cases
    // a. both are below median, median does not change
    if (!insIsHigher && !remIsHigher) {
      for (auto it = this->begin(); it != m_medianIt;){
        if (!inserted && *it >= toIns){
          inserted = true;
          this->insert(it, toIns);
          std::cout << "Inserting before " << *it << std::endl;
          ++it;
        }
        if (!removed && *it == toRem){
          removed = true;
          it = this->erase(it);
        }
        if (inserted && removed)
          break; // early exit if possible
      }

      // if not yet inserted, insert
      if (!inserted)
        this->insert(m_medianIt, toIns);
      // if not yet removed, remove
      if (!removed) // TODO: maybe check if it's equal?
        this->erase(m_medianIt);
    }

    // b. both are above median, median does not change
    else if (insIsHigher && remIsHigher) {
      for (auto it = m_medianIt; it != this->end();){
        if (!inserted && *it >= toIns){
          inserted = true;
          this->insert(it, toIns);
          ++it;
        }
        if (!removed && *it == toRem){
          removed = true;
          it = this->erase(it);
        }
        if (inserted && removed)
          break; // early exit if possible
      }

      // if not yet inserted, insert
      if (!inserted)
        this->insert(m_medianIt, toIns);
      // if not yet removed, remove
      if (!removed) // TODO: maybe check if it's equal?
        this->erase(m_medianIt);
    }

    // c. new is below, old is above, median must be decremented
    else if (!insIsHigher && remIsHigher) {
      // insert below
      for (auto it = this->begin(); it != m_medianIt; ++it){
        if (*it >= toIns){
          this->insert(it, toIns);
          break;
        }
      }
      // if not yet inserted, insert
      if (!inserted)
        this->insert(m_medianIt, toIns);
      // remove above
      for (auto it = m_medianIt; it != this->end(); ++it){
        if (*it == toRem){
          it = this->erase(it);
          break;
        }
      }
      // if not yet removed, remove
      if (!removed) // TODO: maybe check if it's equal?
        this->erase(m_medianIt);
      // move median back
      m_medianIt--;
    }

    // d. new is above, old is below, median must be incremented
    else if (insIsHigher && !remIsHigher) {
      // remove below
      for (auto it = this->begin(); it != m_medianIt; ++it){
        if (*it == toRem){
          this->erase(it);
          break;
        }
      }
      // if not yet removed, remove
      if (!removed) // TODO: maybe check if it's equal?
        this->erase(m_medianIt);
      // insert above
      for (auto it = m_medianIt; it != this->end(); ++it){
        if (*it >= toIns){
          this->insert(it, toIns);
          break;
        }
      }
      // if not yet inserted, insert
      if (!inserted)
        this->insert(m_medianIt, toIns);
      // move median forward
      m_medianIt++;
    }

  }

  T median(){
    // walk halfway on first call?
    if (m_medianLength < 0){
      // sort on first run
      this->sort();
      // walk
      m_medianIt = this->begin();
      std::advance(m_medianIt, this->size()/2);
      // flag that median length is set
      m_medianLength = this->size();
    }
    return *m_medianIt;
  }

private:
  typename std::list<T>::iterator m_medianIt;
  int m_medianLength = -1;
  
};

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

  printf("======= just nth_element \n");
  for (int i = 0; i < iterations; ++i){
    std::vector<int> y(
      x.begin() + i,
      x.begin() + i + medlen
    );

    quickprint(y.begin(), y.end());
    simpleMedian(y);
    printf("Median: %d\n", y.at(medlen/2));
  }

  printf("======= vector median filter\n");
  VectorMedianFilter<int> vmf(x.begin(), x.begin()+medlen);

  for (int i = 0; i < iterations; ++i){
    if (i >= 1)
      vmf.replace(x.at(i + medlen-1), x.at(i - 1));
    
    quickprint(vmf.begin(), vmf.end());

    simpleMedian(vmf);
    printf("Median: %d\n", vmf.at(medlen/2));
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
  }
  return 0;
}
