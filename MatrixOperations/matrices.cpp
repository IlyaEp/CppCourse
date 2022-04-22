//
// Created by ilya on 27.09.2021.
//

#include <fstream>
#include <algorithm>
#include <numeric>

#include "matrices.hpp"

Matrix::Matrix(size_t number_of_rows, size_t number_of_columns):
        number_of_rows_(number_of_rows), number_of_columns_(number_of_columns) {
    for (size_t row = 0; row < number_of_rows; row++)
    {elements_.emplace_back(number_of_columns);}
}

Matrix::Matrix(std::ifstream &file_with_matrix):
        number_of_rows_(0), number_of_columns_(0) {
    file_with_matrix >> number_of_rows_ >> number_of_columns_;

    for (size_t row = 0; row < number_of_rows_; row++) {
        elements_.emplace_back(number_of_columns_);
        for (size_t column = 0; column < number_of_columns_; column++) {
            double new_elem = 0.0;
            file_with_matrix >> new_elem;
            elements_[row][column] = new_elem;
        }
    }
}

Matrix Matrix::operator+(const Matrix &second_matrix) {
    const auto rows_eq = number_of_rows_ == second_matrix.number_of_rows_;
    const auto cols_eq = number_of_columns_ == second_matrix.number_of_columns_;
    if (not rows_eq or not cols_eq) {
        throw std::runtime_error("The number of rows or columns does not match for addition");
    }

    Matrix result(number_of_rows_, number_of_columns_);
    for (size_t row = 0; row < number_of_rows_; row++) {
        std::transform(elements_[row].begin(), elements_[row].end(),
                       second_matrix.elements_[row].begin(), result.elements_[row].begin(),
                       std::plus<double>());
    }

    return result;
}

std::vector<double> Matrix::get_column(size_t index_column) const noexcept {
    std::vector<double> result(number_of_rows_);
    for (size_t row = 0; row < number_of_rows_; row++)
    {result[row] = elements_[row][index_column];}

    return result;
}

Matrix Matrix::operator*(const Matrix& second_matrix) {
    if (number_of_columns_ != second_matrix.number_of_rows_) {
        throw std::runtime_error("The number of rows or columns does not match for multiplication");
    }

    Matrix result(number_of_rows_, second_matrix.number_of_columns_);
    for (size_t row = 0; row < number_of_rows_; row++) {
        for (size_t column = 0; column < second_matrix.number_of_columns_; column++) {
            std::vector<double> multiplication_row_column(number_of_columns_);
            std::transform(elements_[row].begin(), elements_[row].end(),
                           second_matrix.get_column(column).begin(),
                           multiplication_row_column.begin(),
                           std::multiplies<double>());

            result.elements_[row][column] = std::accumulate(multiplication_row_column.begin(),
                                                            multiplication_row_column.end(), 0.0);
        }
    }

    return result;
}

Matrix Matrix::operator+=(const Matrix &second_matrix) {
    return *this = *this + second_matrix;
}

Matrix Matrix::operator*=(const Matrix &second_matrix) {
    return *this = *this * second_matrix;
}

std::ostream& operator<<(std::ostream &os, const Matrix &matrix) {
    os << matrix.number_of_rows_ << " " << matrix.number_of_columns_ << std::endl;

    for (const auto &row : matrix.elements_) {
        for (const auto &element : row)
        {os << element << " ";}

        os << std::endl;
    }

    return os;
}
