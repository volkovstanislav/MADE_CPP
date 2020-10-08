#pragma once

#include <vector>
#include <iostream>


namespace task {

    const double EPS = 1e-6;


    class OutOfBoundsException : public std::exception {};
    class SizeMismatchException : public std::exception {};


    class Matrix {

    public:

        Matrix();
        Matrix(size_t rows, size_t cols);
        Matrix(const Matrix& copy);

        Matrix& operator=(const Matrix& a);

        double& get(size_t row, size_t col);
        const double& get(size_t row, size_t col) const;
        void set(size_t row, size_t col, const double& value);
        void resize(size_t new_rows, size_t new_cols);

        double * operator[](size_t row);
        const double * operator[](size_t row) const;


        Matrix& operator+=(const Matrix& a);
        Matrix& operator-=(const Matrix& a);
        Matrix& operator*=(const Matrix& a);
        Matrix& operator*=(const double& number);

        Matrix operator+(const Matrix& a) const;
        Matrix operator-(const Matrix& a) const;
        Matrix operator*(const Matrix& a) const;
        Matrix operator*(const double& a) const;

        Matrix operator-() const;
        Matrix operator+() const;

        double det() const;
        void transpose();
        Matrix transposed() const;
        double trace() const;

        std::vector<double> getRow(size_t row);
        std::vector<double> getColumn(size_t column);

        bool operator==(const Matrix& a) const;
        bool operator!=(const Matrix& a) const;

        int get_rows() const;
        int get_cols() const;

        double det_calc(double **mat, int n) const;

    private:
        double **matrix;
        int rows_, cols_;

        void alloc_memory() {
            matrix = new double *[rows_];
            for (int i = 0; i < rows_; i++)
                matrix[i]  = new double[cols_];
        }


    };

    Matrix operator*(const double& a, const Matrix& b);

    std::ostream& operator<<(std::ostream& output, const Matrix& matrix);

    std::istream& operator>>(std::istream& input, Matrix& matrix);

}  // namespace task




