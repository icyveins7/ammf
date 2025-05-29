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

  template <typename Iterator>
  void insertSorted(const T val, Iterator begin, Iterator end){
    for (auto it = begin; it != end; ++it){
      if (*it >= val){
        this->insert(it, val);
        return;
      }
    }
    // if we haven't found insertion then push to end
    this->insert(end, val);
  }

  template <typename Iterator>
  void eraseFirstOccurrence(const T val, Iterator begin, Iterator end){
    for (auto it = begin; it != end; ++it){
      if (*it == val){
        this->erase(it);
        return;
      }
    }
    // If we didn't find one, throw
    throw std::runtime_error("value not found in list for eraseFirstOccurrence");
  }

  void replace(const T newV, const T oldV){
    if (m_medianLength < 0)
      throw std::runtime_error("must call median() at least once to finalise median length before replacing");

    std::cout << "Removing " << oldV << std::endl;
    std::cout << "Inserting " << newV << std::endl;
    // convention is to use geq
    constexpr unsigned char NEW_IS_HIGHER = 0b00100000;
    constexpr unsigned char NEW_IS_EQUAL  = 0b00010000;
    constexpr unsigned char NEW_IS_LOWER  = 0b00001000;
    constexpr unsigned char OLD_IS_HIGHER = 0b00000100;
    constexpr unsigned char OLD_IS_EQUAL  = 0b00000010;
    constexpr unsigned char OLD_IS_LOWER  = 0b00000001;
    unsigned char state = 0;
    const T oldMed = this->median();
    if (newV > oldMed)
      state |= NEW_IS_HIGHER;
    else if (newV == oldMed)
      state |= NEW_IS_EQUAL;
    else
      state |= NEW_IS_LOWER;

    if (oldV > oldMed)
      state |= OLD_IS_HIGHER;
    else if (oldV == oldMed)
      state |= OLD_IS_EQUAL;
    else
      state |= OLD_IS_LOWER;

    // There are 9 total cases
    switch (state)
    {
      // Both of these do not involve median changes
      case NEW_IS_HIGHER | OLD_IS_HIGHER: {
        this->insertSorted(newV, std::next(this->m_medianIt), this->end());
        this->eraseFirstOccurrence(oldV, std::next(this->m_medianIt), this->end());
        break;
      }
      case NEW_IS_LOWER  | OLD_IS_LOWER: {
        this->insertSorted(newV, this->begin(), this->m_medianIt);
        this->eraseFirstOccurrence(oldV, this->begin(), this->m_medianIt);
        break;
      }
      // Both of these involve median changes
      case NEW_IS_HIGHER | OLD_IS_LOWER: {
        this->insertSorted(newV, std::next(this->m_medianIt), this->end());
        this->eraseFirstOccurrence(oldV, this->begin(), this->m_medianIt);
        // Move median forward
        this->m_medianIt = std::next(this->m_medianIt);
        break;
      }
      case NEW_IS_LOWER  | OLD_IS_HIGHER: {
        this->insertSorted(newV, this->begin(), this->m_medianIt);
        this->eraseFirstOccurrence(oldV, std::next(this->m_medianIt), this->end());
        // Move median backward
        this->m_medianIt = std::prev(this->m_medianIt);
        break;
      }
      // When new is equal to median, we can choose insert location
      // so as to not invalidate the median iterator
      case NEW_IS_EQUAL  | OLD_IS_LOWER: {
        // Insert before the current median
        this->insert(m_medianIt, newV);
        this->eraseFirstOccurrence(oldV, this->begin(), this->m_medianIt);
        break;
      }
      case NEW_IS_EQUAL  | OLD_IS_HIGHER: {
        // Insert after the median
        this->insert(std::next(m_medianIt), newV);
        this->eraseFirstOccurrence(oldV, std::next(this->m_medianIt), this->end());
        break;
      }
      // When old is equal to median, we need to invalidate median iterator
      case NEW_IS_LOWER  | OLD_IS_EQUAL: {
        this->insertSorted(newV, this->begin(), this->m_medianIt);
        this->m_medianIt = this->erase(this->m_medianIt);
        // Move median back
        this->m_medianIt = std::prev(this->m_medianIt);
        break;
      }
      case NEW_IS_HIGHER | OLD_IS_EQUAL: {
        this->insertSorted(newV, std::next(this->m_medianIt), this->end());
        this->m_medianIt = this->erase(this->m_medianIt);
        // No need to move the median, it is already correct
        break;
      }
      // This is a trivial case, no need to do anything at all
      case NEW_IS_EQUAL | OLD_IS_EQUAL: {
        break;
      }
      default: {
        throw std::runtime_error("Invalid state in replace()\n");
      }

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
