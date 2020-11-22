#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>
#include <list>
#include "list.h"


size_t RandomUInt(size_t max = -1) {
  static std::mt19937 rand(std::random_device{}());

  std::uniform_int_distribution<size_t> dist{0, max};
  return dist(rand);
}

size_t RandomUInt(size_t min, size_t max) {
  return min + RandomUInt(max - min);
}

bool TossCoin() {
  return RandomUInt(1) == 0;
}


template <class T>
void RandomFill(T& container, size_t count, size_t max = -1) {
  while (count > 0) {
    container.push_back(RandomUInt(max));
    --count;
  }
}

struct Immovable {
  Immovable() = default;
  Immovable(const Immovable&) = delete;
  Immovable(Immovable&&) = delete;

  Immovable& operator=(const Immovable&) = delete;
  Immovable& operator=(Immovable&&) = delete;
};


struct MoveTester {
  std::string action;

  MoveTester() : action("DC") {}
  MoveTester(const MoveTester&) : action("CC") {}
  MoveTester(MoveTester&&) noexcept : action("MC") {}

  MoveTester& operator=(const MoveTester&) { action = "CA"; return *this; }
  MoveTester& operator=(MoveTester&&) noexcept { action = "MA"; return *this; }
};

struct ArgForwardTester {
  std::string actions;

  ArgForwardTester(MoveTester a, MoveTester b, MoveTester c) {
    actions = a.action + b.action + c.action;
  }
};


void FailWithMsg(const std::string& msg, int line) {
  std::cerr << "Test failed!\n";
  std::cerr << "[Line " << line << "] "  << msg << std::endl;
  std::exit(EXIT_FAILURE);
}

#define ASSERT_TRUE(cond) \
    if (!(cond)) {FailWithMsg("Assertion failed: " #cond, __LINE__);};

#define ASSERT_TRUE_MSG(cond, msg) \
    if (!(cond)) {FailWithMsg(msg, __LINE__);};

#define ASSERT_EQUAL_MSG(cont1, cont2, msg) \
    ASSERT_TRUE_MSG(std::equal(cont1.begin(), cont1.end(), cont2.begin(), cont2.end()), msg)


int main() {

  {
    task::list<int> list;
    list.push_back(42);
    ASSERT_TRUE(list.size() == 1)
    list.push_back(2);
    list.pop_front();
    ASSERT_TRUE(list.front() == 2)
    list.pop_back();
    ASSERT_TRUE(list.empty())
  }

  {
    task::list<std::string> list;
    list.push_front("test");
    ASSERT_TRUE(!list.empty())
    list.push_front("test2");
    ASSERT_TRUE(list.back() == "test")
    list.clear();
    list.clear();
    ASSERT_TRUE(list.size() == 0)
    list.resize(10);
    ASSERT_TRUE(list.size() == 10)
    ASSERT_TRUE(list.back() == "")
  }

  {
    const task::list<int> list(5);
    ASSERT_TRUE(list.front() == int())
    ASSERT_TRUE(list.back() == int())
  }

}
