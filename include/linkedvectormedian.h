#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <type_traits>

#define DIRECTION_BACKWARD_INDEX 0
#define DIRECTION_FORWARD_INDEX  1
template<typename Tval, typename Tidx>
struct DoublyLinkedVectorNode {
  // We use negative values to specify invalid jumps, so indices must be signed
  static_assert(std::is_signed_v<Tidx>, "Tindex must be signed");
  static_assert(std::is_integral_v<Tidx>, "Tindex must be an integer type");

  Tval value;
  // 0: prev index, 1: next index
  Tidx neighbours[2];

  Tidx& prev() { return neighbours[DIRECTION_BACKWARD_INDEX]; }
  Tidx& next() { return neighbours[DIRECTION_FORWARD_INDEX]; }

  void reset() {
    neighbours[0] = -1;
    neighbours[1] = -1;
  }

  /**
   * @brief Gets the index of the current node with reference to the beginning
   * of the vector's memory
   *
   * @param begin Pointer to 0-index of the vector's memory
   * @return The index of the current node
   */
  Tidx index(DoublyLinkedVectorNode<Tval, Tidx>* begin) const {
    return static_cast<Tidx>(this - begin);
  }
};


template<typename Tval, typename Tidx>
class DoublyLinkedVectorMedian 
{
public:
  explicit DoublyLinkedVectorMedian(std::size_t size) : m_nodes(size)
  {}

  template<typename Iterator>
  void initialize(Iterator begin, Iterator end)
  {
    std::vector<Tval> sorted_values(begin, end);
    std::size_t count = sorted_values.size();
    if (count != m_nodes.size()) {
      throw std::invalid_argument(
        "Number of elements should exactly match size of vector (expected " + std::to_string(m_nodes.size()) + " but found " + std::to_string(count) + ")"
      );
    }
    // Sort in the temporary first
    std::sort(sorted_values.begin(), sorted_values.end());

    // Then now place into our nodes, initial order is simply forwards
    for (std::size_t i = 0; i < count; ++i) {
      m_nodes[i].value = sorted_values[i];
      m_nodes[i].prev = (i == 0) ? static_cast<Tidx>(-1) : static_cast<Tidx>(i - 1);
      m_nodes[i].next = (i == count - 1) ? static_cast<Tidx>(-1) : static_cast<Tidx>(i + 1);
    }

    // Initialize the median index
    m_medianIdx = static_cast<Tidx>(count / 2);
  }

  bool isInitialized() const { return m_medianIdx != -1; }

  DoublyLinkedVectorNode<Tval, Tidx> insertAfter(
    DoublyLinkedVectorNode<Tval, Tidx>& existing
  ){
    // Existing is left, find the right neighbour
    // TODO: take care of the case where existing is the last node
    DoublyLinkedVectorNode<Tval, Tidx>& right = m_nodes[existing.next()];

    // Create a new node
    DoublyLinkedVectorNode<Tval, Tidx> newNode;
    // We set our new node to always be 'outside' the existing memory
    // Modify neighbours and the new node to point to each other
    newNode.prev() = existing.index(m_nodes.data());
    newNode.next() = right.index(m_nodes.data());

    // So this points 'outside' the current memory for now
    existing.next() = (Tidx)m_nodes.size();
    // TODO: take care of this as well
    right.prev() = (Tidx)m_nodes.size();

    return newNode;
  }

  void replace(const Tval newV, const Tval oldV) {
    // Cannot replace if not initialized
    if (!isInitialized()) {
      throw std::runtime_error("Cannot replace if not initialized");
    }

    // Find which side the new value is on
  }


private:
  std::vector<DoublyLinkedVectorNode<Tval, Tidx>> m_nodes;
  Tidx m_medianIdx = -1;
  Tidx m_freeIdx = -1;
};


// template <typename Tval, typename Tidx>
// struct SinglyLinkedVectorNode {
//   // Require negative values for invalid jumps, so enforce signed integer types for indices
//   static_assert(std::is_signed_v<Tidx>, "Tidx must be signed");
//   static_assert(std::is_integral_v<Tidx>, "Tidx must be an integer type");
//
//   Tval value;
//   Tidx next;
// };
//
// template <typename Tval, typename Tidx>
// class SinglyLinkedVectorMedian
// {
// public:
//   explicit SinglyLinkedVectorMedian(std::size_t size) : m_nodes(size)
//   {}
//
//   template <typename Iterator>
//   void initialize(Iterator begin, Iterator end)
//   {
//     std::vector<Tval> sorted_values(begin, end);
//     std::size_t count = sorted_values.size();
//     if (count != m_nodes.size()) {
//       throw std::invalid_argument(
//         "Number of elements should exactly match size of vector (expected " + std::to_string(m_nodes.size()) + " but found " + std::to_string(count) + ")"
//       );
//     }
//     // Sort in the temporary first
//     std::sort(sorted_values.begin(), sorted_values.end());
//
//     // Insert in sorted manner
//     for (std::size_t i = 0; i < count; ++i) {
//       m_nodes[i].value = sorted_values[i];
//       // For first to 2nd last, set next to the next index
//       if (i < count - 1)
//         m_nodes[i].next = i + 1;
//       // For the last one, set next to -1
//       else
//         m_nodes[i].next = -1;
//     }
//     // Define our start index (which is 0)
//     m_startIdx = 0;
//   }
//
//   bool isInitialized() const { return m_startIdx != -1; }
//
//   // static constexpr unsigned char VAL_IS_HIGHER = 0b00000100;
//   // static constexpr unsigned char VAL_IS_EQUAL  = 0b00000010;
//   // static constexpr unsigned char VAL_IS_LOWER  = 0b00000001;
//   // unsigned char compare(const Tval value, const Tval pivot) const {
//   //   if (value > pivot)
//   //     return VAL_IS_HIGHER;
//   //   else if (value == pivot)
//   //     return VAL_IS_EQUAL;
//   //   else
//   //     return VAL_IS_LOWER;
//   // }
//
//   void replace(const Tval newV, const Tval oldV){
//     // Determine how the median should change after deletion
//     signed char medianMovement = 0;
//     if (oldV < m_nodes[m_medianIdx].value)
//       medianMovement += 1;
//     else if (oldV > m_nodes[m_medianIdx].value)
//       medianMovement -= 1;
//
//     if (newV < m_nodes[m_medianIdx].value)
//       medianMovement -= 1;
//     else if (newV > m_nodes[m_medianIdx].value)
//       medianMovement += 1;
//
//     // This is unnecessary
//     // if (newV == m_nodes[m_medianIdx].value)
//     //   medianMovement *= 1;
//     if (oldV == m_nodes[m_medianIdx].value)
//       medianMovement = 0;
//
//
//   }
//
// private:
//   std::vector<SinglyLinkedVectorNode<Tval, Tidx>> m_nodes;
//   Tidx m_startIdx = -1;
//   Tidx m_medianIdx = -1;
//
// };





