//
// Created by ilya on 27.09.2021.
//

#ifndef MATRICES_OPERATIONS_HPP
#define MATRICES_OPERATIONS_HPP

#include <vector>
#include <iostream>
#include <string>

class Matrix {
    std::vector<std::vector<double>> elements_;
    size_t number_of_rows_;
    size_t number_of_columns_;

    bool error_in_execution_ = false;

    std::vector<double> get_column(size_t index_column) const noexcept;

public:
    explicit Matrix(size_t number_of_rows = 0, size_t number_of_columns = 0);

    explicit Matrix(std::ifstream &file_with_matrix);

    Matrix operator+(const Matrix &second_matrix);
    Matrix operator*(const Matrix &second_matrix);
    Matrix operator+=(const Matrix &second_matrix);
    Matrix operator*=(const Matrix &second_matrix);

    friend std::ostream& operator<<(std::ostream &os, const Matrix &matrix);
};

#endif //MATRICES_OPERATIONS_HPP
