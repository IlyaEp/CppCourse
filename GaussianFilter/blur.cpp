//
// Created by ilya on 12.11.2021.
//

#include <iostream>
#include <cmath>
#include <fstream>

#include "blur.hpp"

GaussianBlur::GaussianBlur(uint32_t new_radius): radius_(new_radius), kernel_(std::vector<double>(radius_)) {
    const auto sigma = 0.3 * ((radius_ - 1) * 0.5 - 1) + 0.8;
    auto sum = 0.0;
    for (uint32_t i = 0; i < radius_; ++i) {
        kernel_[i] = std::exp(-0.5 * std::pow((i - (radius_ - 1) / 2.0), 2.0)/(std::pow(sigma, 2.0)));
        sum += kernel_[i];
    }

    for (uint32_t i = 0; i < radius_; ++i)
    {kernel_[i] /= sum;}
}

void GaussianBlur::run(const std::filesystem::path& path_to_bmp, const std::filesystem::path& path_to_result) {
    auto img = read_bmp_to_matrix(path_to_bmp);
    img = blur(blur(img), false);
    write_matrix_to_bmp(img, path_to_result);
}

std::vector<uint8_t> GaussianBlur::read_bmp_to_matrix(const std::filesystem::path& path_to_bmp) {
    std::ifstream file(path_to_bmp, std::ios::binary);

    file.read(reinterpret_cast<char*>(&header_), sizeof(header_));
    file.read(reinterpret_cast<char*>(&info_), sizeof(info_));

    if (info_.width < radius_ or info_.height < radius_) {
        throw std::runtime_error("Kernel size not valid for this image");
    }

    if (info_.bit_count == 32) {
        file.read(reinterpret_cast<char*>(&color_header_), sizeof(color_header_));
    }

    file.seekg(header_.data_offset, std::ifstream::beg);
    uint width_real = static_cast<uint>(ceil(info_.bit_count * info_.width / 32.0) * 4);
    std::vector<std::uint8_t> data(width_real * info_.height);
    file.read(reinterpret_cast<char*>(data.data()), static_cast<long>(data.size()));

    file.close();

    return data;
}

void GaussianBlur::write_matrix_to_bmp(const std::vector<uint8_t> &matrix_with_img,
                                       const std::filesystem::path& path_to_result) {
    std::ofstream file(path_to_result, std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<const char*>(&header_), sizeof(header_));
    file.write(reinterpret_cast<const char*>(&info_), sizeof(info_));
    if (info_.bit_count == 32) {
        file.write(reinterpret_cast<char*>(&color_header_), sizeof(color_header_));
    }
    file.seekp(header_.data_offset);
    file.write(reinterpret_cast<const char*>(matrix_with_img.data()), static_cast<long>(matrix_with_img.size()));

    file.close();
}

std::vector<uint8_t> GaussianBlur::blur(const std::vector<uint8_t> &matrix_with_img, bool is_horizontal) const {
    std::vector<uint8_t> result(matrix_with_img.size(), 0);
    const auto channels = info_.bit_count / 8u;

    if (info_.bit_count == 32) {
        for (uint32_t i = 0; i < info_.height; ++i) {
            for (uint32_t j = 0; j < info_.width; ++j) {
                result[channels * (i * info_.width + j) + 3] = 255;
            }
        }
    }

    const auto half_of_radius = static_cast<int32_t>(radius_ / 2);
    const auto start_index = is_horizontal? half_of_radius : radius_;

    for (uint32_t i = start_index; i < info_.height - start_index; ++i) {
        for (uint32_t j = start_index; j < info_.width - start_index; ++j) {
            double blue = 0;
            double green = 0;
            double red = 0;
            for (int32_t k = -half_of_radius; k < half_of_radius + 1; ++k) {
                if (is_horizontal) {
                    blue += matrix_with_img[channels * (i * info_.width + j + k)] * kernel_[half_of_radius + k];
                    green += matrix_with_img[channels * (i * info_.width + j + k) + 1] * kernel_[half_of_radius + k];
                    red += matrix_with_img[channels * (i * info_.width + j + k) + 2] * kernel_[half_of_radius + k];
                }
                else {
                    blue += matrix_with_img[channels * ((i + k) * info_.width + j)] * kernel_[half_of_radius + k];
                    green += matrix_with_img[channels * ((i + k) * info_.width + j) + 1] * kernel_[half_of_radius + k];
                    red += matrix_with_img[channels * ((i + k) * info_.width + j) + 2] * kernel_[half_of_radius + k];
                }

            }

            result[channels * (i * info_.width + j)] = static_cast<uint8_t>(blue);
            result[channels * (i * info_.width + j) + 1] = static_cast<uint8_t>(green);
            result[channels * (i * info_.width + j) + 2] = static_cast<uint8_t>(red);
        }
    }

    return result;
}