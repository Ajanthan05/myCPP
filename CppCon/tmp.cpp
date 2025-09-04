#include <iostream>
#include <array>
#include <vector>
#include <cstddef>
#include <type_traits>

constexpr std::size_t dynamic_size = static_cast<std::size_t>(-1);

// --------------------
// Primary template: general case = dynamic sizes
// --------------------
template<typename T, std::size_t Rows = dynamic_size, std::size_t Cols = dynamic_size>
class Matrix {
    std::size_t rows_, cols_;
    std::vector<T> data_;

public:
    Matrix(std::size_t r, std::size_t c) : rows_(r), cols_(c), data_(r * c) {}

    T& at(std::size_t row, std::size_t col) {
        return data_[row * cols_ + col];
    }

    const T& at(std::size_t row, std::size_t col) const {
        return data_[row * cols_ + col];
    }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }
};

// --------------------
// Partial specialization: fixed sizes
// --------------------
template<typename T, std::size_t Rows, std::size_t Cols>
class Matrix<T, Rows, Cols> {
    // Enable only if Rows and Cols are not dynamic_size
    static_assert(Rows != dynamic_size && Cols != dynamic_size, "Fixed-size Matrix requires non-dynamic dimensions");
    std::array<T, Rows * Cols> data_{};

public:
    Matrix() = default; // no need to pass dimensions

    T& at(std::size_t row, std::size_t col) {
        return data_[row * Cols + col];
    }

    const T& at(std::size_t row, std::size_t col) const {
        return data_[row * Cols + col];
    }

    constexpr std::size_t rows() const { return Rows; }
    constexpr std::size_t cols() const { return Cols; }
};

int main() {
    Matrix<int, 3, 3> fixed_matrix;
    fixed_matrix.at(1, 1) = 5;
    std::cout << "Fixed[1][1] = " << fixed_matrix.at(1, 1) << "\n";

    Matrix<int> dynamic_matrix(4, 4); // falls back to primary template
    dynamic_matrix.at(2, 2) = 9;
    std::cout << "Dynamic[2][2] = " << dynamic_matrix.at(2, 2) << "\n";

    return 0;
}