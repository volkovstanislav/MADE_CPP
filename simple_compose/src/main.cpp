#include <iostream>
#include <functional>

typedef std::function<int (int)> Op;

// Рекурсивная реализация композиции анонимных функций
Op compose (size_t n, Op ops[]) {
    if (n == 0) {
        return [] (int x) {return x;};
    }
    else if (n == 1) {
        return ops[0];
    } else {
        Op *src = new Op[n - 1];
        for (int i = 1; i < n; i++) {
            src[i - 1] = ops[i];
        }
        return [=] (auto x) { return ops[0](compose(n - 1, src)(x)); };

    }
}


int main () {
    /// Simple tests:

    Op op1 =
            [] (int x) -> int {
                return x + 1;
            };

    auto op2 =
            [] (int p) -> Op {
                return [p] (int x) {
                    return p * x;
                };
            };

    // Добавим еще одну функцию для примера
    auto op3 =
            [] (int a, int b) -> Op {
                return [a, b] (int x) {
                    return a * x + b;
                };
            };

    {
        Op ops[4] = {op1, op2(2), op1, op2(2)};
        if (compose(4, ops)(2) != 11) {
            std::cout << "FAILED AT TEST 1" << std::endl;
            return 0;
        }
    }

    {
        Op ops[8] = {op1, op2(2), op1, op1, op1, op2(2), op2(2), op1};
        if (compose(8, ops)(5) != 55) {
            std::cout << "FAILED AT TEST 2" << std::endl;
            return 0;
        }
    }

    {
        Op ops[1] = {op1};
        if (compose(1, ops)(3) != 4) {
            std::cout << "FAILED AT TEST 3" << std::endl;
            return 0;
        }
    }

    {
        Op ops[0] = {};
        if (compose(0, ops)(4) != 4) {
            std::cout << "FAILED AT TEST 4" << std::endl;
            return 0;
        }
    }

    {
        Op ops[4] = {op2(2), op2(3), op2(4), op2(5)};
        if (compose(4, ops)(1) != 120) {
            std::cout << "FAILED AT TEST 5" << std::endl;
            return 0;
        }
    }

    // Добавлен еще один пример для тестирования
    {
        Op ops[7] = {op1, op1, op2(2), op2(3), op2(4), op2(5), op3(2,3)};
        if (compose(7, ops)(1) != 602) {
            std::cout << "FAILED AT TEST 6" << std::endl;
            return 0;
        }
    }

    std::cout << "ALL TESTS PASSED" << std::endl;

}
