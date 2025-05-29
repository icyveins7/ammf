#pragma once

#include <list>
#include <algorithm>
#include <stdexcept>
#include <iostream>

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

#ifndef NDEBUG
    std::cout << "Removing " << oldV << std::endl;
    std::cout << "Inserting " << newV << std::endl;
#endif
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
