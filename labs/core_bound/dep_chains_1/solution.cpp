#include "solution.hpp"
#include <array>
#include <iostream>

unsigned getSumOfDigits(unsigned n) {
  unsigned sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

// Task: lookup all the values from l2 in l1.
// For every found value, find the sum of its digits.
// Return the sum of all digits in every found number.
// Both lists have no duplicates and elements placed in *random* order.
// Do NOT sort any of the lists. Do NOT store elements in a hash_map/sets.

// Hint: Traversing a linked list is a long data dependency chain:
//       to get the node N+1 you need to retrieve the node N first.
//       Think how you can execute multiple dependency chains in parallel.

#ifdef SOLUTION
template <int M> unsigned solution(List *l1, List *l2) {
  unsigned retVal = 0;
  List *head2 = l2;
  List *head1 = l1;

  int length1 = 0;
  while (l1) {
    length1++;
    l1 = l1->next;
  }

  l1 = head1;

  // Simultaneously lookup M elements in l1.
  for (int i = 0; i < length1 / M; i++) {
    std::array<unsigned, M> vals;
    // remember M values from l1
    for (int j = 0; j < M; j++) {
      vals[j] = l1->value;
      l1 = l1->next;
    }
    // traverse l2 and lookup M elements from vals at the same time
    l2 = head2;
    int found = 0;
    while (l2) {
      for (int j = 0; j < M; j++) {
        if (l2->value == vals[j]) {
          retVal += getSumOfDigits(l2->value);
          // stop if all M values found
          if (++found == M)
            break;
        }
      }
      l2 = l2->next;
    }
  }

  // Process the remainder with sequential algorithm
  // O(N^2) algorithm:
  while (l1) {
    unsigned v = l1->value;
    l2 = head2;
    while (l2) {
      if (l2->value == v) {
        retVal += getSumOfDigits(v);
        break;
      }
      l2 = l2->next;
    }
    l1 = l1->next;
  }

  return retVal;
}

unsigned solution(List *l1, List *l2) { return solution<8>(l1, l2); }

#else

unsigned solution(List *l1, List *l2) {
  unsigned retVal = 0;

  List *head2 = l2;
  // O(N^2) algorithm:
  while (l1) {
    unsigned v = l1->value;
    l2 = head2;
    while (l2) {
      if (l2->value == v) {
        retVal += getSumOfDigits(v);
        break;
      }
      l2 = l2->next;
    }
    l1 = l1->next;
  }

  return retVal;
}

#endif
