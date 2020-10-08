
#include <cmath>
#include "matrix.h"

using task::Matrix;

Matrix::Matrix(): rows_(1), cols_(1) {
    alloc_memory();
    matrix[0][0] = 1;
}

Matrix::Matrix(size_t rows, size_t cols):
    rows_(rows), cols_(cols)
{
    alloc_memory();
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            if (i == j)
                this->matrix[i][j] = 1.;
            else
                this->matrix[i][j] = 0.;
        }
    }
}

Matrix::Matrix(const Matrix& copy):
        rows_(copy.rows_), cols_(copy.cols_)
{
    alloc_memory();
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] = copy[i][j];
        }
    }
}

Matrix & Matrix::operator=(const Matrix &a) {
    if (this == &a) {
        return *this;
    }

    if (rows_ != a.rows_ || cols_ != a.cols_) {
        for (int i = 0; i < rows_; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;

        rows_ = a.rows_;
        cols_ = a.cols_;
        alloc_memory();
    }

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] = a.matrix[i][j];
        }
    }
    return *this;
}

double & Matrix::get(size_t row, size_t col) {
    if (row + 1 > rows_ or col + 1 > cols_) {
        throw OutOfBoundsException();
    } else {
        return matrix[row][col];
    }
}

const double & Matrix::get(size_t row, size_t col) const {
    if (row + 1 > rows_ or col + 1 > cols_) {
        throw OutOfBoundsException();
    }
    return matrix[row][col];
};

void Matrix::set(size_t row, size_t col, const double &value) {
    if (row + 1 > rows_ or col + 1 > cols_) {
        throw OutOfBoundsException();
    }
    matrix[row][col] = value;
}

double * Matrix::operator[](size_t row) {
    return matrix[row];
}

const double * Matrix::operator[](size_t row) const {
    return matrix[row];
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
    if (new_rows <= 0 || new_cols <= 0) {
        throw OutOfBoundsException();
    }

    Matrix new_matrix(new_rows, new_cols);

    for (int i = 0; i < new_rows; i++) {
        for (int j = 0; j < new_cols; j++) {
            if (i >= rows_ or j >= cols_) {
                new_matrix.matrix[i][j] = 0.;
            } else {
                new_matrix.matrix[i][j] = matrix[i][j];
            }
        }
    }

    cols_ = new_cols;
    rows_ = new_rows;
    matrix = new_matrix.matrix;
}

std::vector<double> Matrix::getRow(size_t row) {
    std::vector <double> new_vec;

    for (int i = 0; i < cols_; i++) {
        new_vec.push_back(matrix[row][i]);
    }
    return new_vec;
}

std::vector<double> Matrix::getColumn(size_t column) {
    std::vector <double> new_vec;

    for (int i = 0; i < rows_; i++) {
        new_vec.push_back(matrix[i][column]);
    }
    return new_vec;
}

Matrix Matrix::operator*(const Matrix &a) const {
    if (this->cols_ != a.rows_) {
        throw SizeMismatchException();
    }
    Matrix ret = Matrix(this->rows_, a.cols_);
    for (int i = 0; i < ret.rows_; i++) {
        for (int j = 0; j < ret.cols_; j++) {
            double sum = 0;
            for (int k = 0; k < this->cols_; k++)
                sum += matrix[i][k] * a[k][j];
            ret[i][j] = sum;
        }
    }
    return ret;
}

Matrix Matrix::operator*(const double &a) const {
    Matrix current = *this;
    current *= a;
    return current;
}

Matrix & Matrix::operator*=(const double &a) {
    for (int i = 0; i < this->rows_; i++) {
        for (int j = 0; j < this->cols_; j++) {
            this->matrix[i][j] *= a;
        }
    }
    return *this;
}

Matrix & Matrix::operator*=(const Matrix &a) {
    Matrix result = *this * a;
    *this = result;
    return *this;
}

Matrix Matrix::operator+(const Matrix &a) const {
    if ((this->rows_ != a.rows_) || (this->cols_ != a.cols_)) {
        throw SizeMismatchException();
    } else {
        Matrix current = *this;
        current += a;
        return current;
    }
}

Matrix & Matrix::operator+=(const Matrix &a) {
    if ((this->rows_ != a.rows_) || (this->cols_ != a.cols_)) {
        throw SizeMismatchException();
    } else {
        for (int i = 0; i < this->rows_; i++) {
            for (int j = 0; j < this->cols_; j++) {
                this->matrix[i][j] += a.matrix[i][j];
            }
        }
    }
    return *this;
}

Matrix Matrix::operator-(const Matrix &a) const {
    Matrix current = *this;
    current -= a;
    return current;
}

Matrix & Matrix::operator-=(const Matrix &a) {
    if ((this->rows_ != a.rows_) || (this->cols_ != a.cols_)) {
        throw SizeMismatchException();
    } else {
        for (int i = 0; i < this->rows_; i++) {
            for (int j = 0; j < this->cols_; j++) {
                this->matrix[i][j] -= a.matrix[i][j];
            }
        }
    }
    return *this;
}

Matrix Matrix::operator+() const {
    return *this;
}

Matrix Matrix::operator-() const {
    for (int i = 0; i < this->rows_; i++) {
        for (int j = 0; j < this->cols_; j++) {
            this->matrix[i][j] = -this->matrix[i][j];
        }
    }
    return *this;
}

// Функция расчета определителя адаптированная под нашу задачу по ссылке:
// https://www.geeksforgeeks.org/determinant-of-a-matrix/
double Matrix::det_calc(double **mat, int n) const{
    double num1, num2, det = 1, total = 1;
    double temp[n + 1];
    int index;

    for(int i = 0; i < n; i++)
    {
        index = i;
        while(mat[index][i] == 0 && index < n) {
            index++;
        }
        if(index == n) {
            continue;
        }
        if(index != i) {
            for(int j = 0; j < n; j++){
                std::swap(mat[index][j],mat[i][j]);
            }
            det = det * pow(-1, index - i);
        }
        for(int j = 0; j < n; j++) {
            temp[j] = mat[i][j];
        }
        for(int j = i+1; j < n; j++) {
            num1 = temp[i];
            num2 = mat[j][i];

            for(int k = 0; k < n; k++) {
                mat[j][k] = (num1 * mat[j][k]) - (num2 * temp[k]);
            }
            total = total * num1;
        }

    }

    for(int i = 0; i < n; i++) {
        det = det * mat[i][i];
    }
    return (det/total);
}

double Matrix::det() const {
    if (this->rows_ != this->cols_) {
        throw SizeMismatchException();
    } else {
        return this->det_calc(this->matrix, this->rows_);
    }
}

double Matrix::trace() const {
    if (this->rows_ != this->cols_) {
        throw SizeMismatchException();
    } else {
        int bound = std::min(this->rows_, this->cols_);
        double sum = 0;
        for (int i = 0; i < bound; ++i)
            sum += matrix[i][i];
        return sum;
    }
}

void Matrix::transpose() {
    Matrix ret(cols_, rows_);
    for (int i = 0; i < this->rows_; i++) {
        for (int j = 0; j < this->cols_; j++) {
            ret.matrix[j][i] = -this->matrix[i][j];
        }
    }
    *this = ret;
}

Matrix Matrix::transposed() const {
    Matrix ret(cols_, rows_);
    for (int i = 0; i < this->rows_; i++) {
        for (int j = 0; j < this->cols_; j++) {
            ret.matrix[j][i] = -this->matrix[i][j];
        }
    }
    return ret;
}

bool Matrix::operator==(const Matrix &a) const {
    if (this->rows_ != a.rows_ || this->cols_ != a.cols_) {
        return false;
    } else {
        for (int i = 0; i < rows_ ; i++) {
            for (int j = 0; j < cols_; j++) {
                if (std::abs(this->matrix[i][j] - a.matrix[i][j]) > EPS) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &a) const {
    if (this->rows_ != a.rows_ || this->cols_ != a.cols_) {
        return true;
    } else {
        for (int i = 0; i < rows_ ; i++) {
            for (int j = 0; j < cols_; j++) {
                if (std::abs(this->matrix[i][j] - a.matrix[i][j]) > EPS) {
                    return true;
                }
            }
        }
    }
    return false;
}

int Matrix::get_rows() const {
    return this->rows_;
}

int Matrix::get_cols() const {
    return this->cols_;
}


Matrix task::operator*(const double& a, const Matrix& b) {
    return b * a;
}

std::ostream& task::operator<<(std::ostream& output, const Matrix& matrix) {
    for (int i = 0; i < matrix.get_rows(); i++) {
        for (int j = 0; j < matrix.get_cols(); j++) {
            output << matrix[i][j] << " ";
        }
    }
    output << '\n';
    return output;
}

std::istream& task::operator>>(std::istream& input, Matrix& matrix) {
    int rows, cols;
    input >> rows;
    input >> cols;
    Matrix ret(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            input >> ret[i][j];
        }
    }
    matrix = ret;
    return input;
}
