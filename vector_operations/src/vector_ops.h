#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>


namespace task {
    using std::vector;
    using std::cin;
    using std::cout;
    using std::endl;
    using std::istream;
    using std::ostream;
    using std::swap;

    // Сложение векторов
    vector < double > operator +(const vector < double > &a, const vector < double > &b) {
        vector < double > c;
        for ( int i = 0; i < a.size(); i++) {
            c.push_back(a[i] + b[i]);
        }
        return c;
    }

    // Унарный плюс
    vector < double > operator +(const vector < double > &a) {
        return a;
    }

    // Разность векторов
    vector < double > operator -(const vector < double > &a, const vector < double > &b) {
        vector < double > c;
        for ( int i = 0; i < a.size(); i++) {
            c.push_back(a[i] - b[i]);
        }
        return c;
    }

    // Унарный минус
    vector < double > operator -(const vector < double > &a) {
        vector < double > c;
        for (int i = 0; i < a.size(); i++) {
            c.push_back(-a[i]);
        }
        return c;
    }

    //Скалярное произведение
    double operator *(const vector < double > &a, const vector < double > &b) {
        double s = 0;
        for ( int i = 0; i < a.size(); i++) {
            s += a[i] * b[i];
        }
        return s;
    }

    // Векторное произведение
    vector < double > operator %(const vector < double > &a, const vector < double > &b) {
        vector < double > c;
        if (a.size() == 3) {
            c.push_back(a[1] * b[2] - a[2] * b[1]);
            c.push_back(a[2] * b[0] - a[0] * b[2]);
            c.push_back(a[0] * b[1] - a[1] * b[0]);
        } else if (a.size() == 2) {
            //ax*by - ay*bx
        }

        return c;
    }

    //Проверка коллинеарности
    int operator ||(const vector < double > &a, const vector < double > &b) {
        double mult = a[0] / b[0];
        const double EPS = 1e-10;
        for (int i = 1; i < a.size(); i++) {
            if (abs((a[i] / b[i]) - mult) < EPS) {
                continue;
            }
            else {
                return 0;
            }
        }
        return 1;
    }

    //Проверка сонаправленности
    bool operator &&(const vector < double > &a, const vector < double > &b) {
        if ((a || b) == 1 and a * b > 0) {
            return true;
        } else {
            return false;
        }
    }

    //Считываем с консоли
    istream& operator>> (std::istream &in, vector < double > &a) {
        int n;
        in >> n;

        if (n == 0) {
            a = {};
        }

        for (int i = 0; i < n; i++) {
            in >> a[i];
        }
        return in;
    }

    // Пишем в консоль
    ostream& operator<< (std::ostream &out, vector < double > &a) {
        for (int i = 0; i < a.size(); i++) {
            out << a[i] << " ";
        }
        out << "\n";
        return out;
    }

    // Меняем порядок
    void reverse(vector < double > &a) {
        int start = 0;
        int end = a.size() - 1;
        while (start < end) {
            swap(a[start], a[end]);
            start++;
            end--;
        }
    }

    // Побитовое поэелментное OR
    vector < int > operator |(const vector < int > &a, const vector < int > &b) {
        vector < int > c;
        for ( int i = 0; i < a.size(); i++) {
            c.push_back(a[i] | b[i]);
        }
        return c;
    }

    // Побитовое поэлементное AND
    vector < int > operator &(const vector < int > &a, const vector < int > &b) {
        vector < int > c;
        for ( int i = 0; i < a.size(); i++) {
            c.push_back(a[i] & b[i]);
        }
        return c;
    }

}  // namespace task
