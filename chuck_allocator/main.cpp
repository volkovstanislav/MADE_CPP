#include <iostream>
#include <vector>
#include "chunk_allocator.h"

using namespace std;

class A {
 public:
  A (int x, int y) {
    cout << x << " " << y << endl;
  }

  ~A() {
    cout << "dest" << endl;
  }

};

int main() {
//  task::Chunk_Allocator<A> alloc;
//  auto p = alloc.allocate(1);
//  alloc.construct(p, 1, 2);
//  alloc.destroy(p);
//  alloc.deallocate(p, 1);

  vector <A, task::Chunk_Allocator<A>> vec;

  auto a = A(100, 200);
  vec.push_back(a);

  return 0;
}