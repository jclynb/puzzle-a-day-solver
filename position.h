#ifndef POSITION 
#define POSITION

#include <iostream>

typedef struct position {
  int x;
  int y;
} position;

struct position_comparator {
  // Check if two positions are equal
  bool operator() (const position &a, const position &b) const {
    return (a.x == b.x) && (a.y == b.y);
  }
};

struct position_hash {
  // Hash tuple
public:
  size_t operator() (const position &p) const {
    auto hash1 = std::hash<int>()(p.x);
    auto hash2 = std::hash<int>()(p.y);

    if (hash1 != hash2) {
      return hash1 ^ hash2;
    }

    return hash1;
  }
};

#endif
