//
// Created by ilya on 12.11.2021.
//

#ifndef GAUSSIAN_BLUR_HPP
#define GAUSSIAN_BLUR_HPP

#include <vector>
#include <filesystem>

#pragma pack(push, 1)
struct BMPHeader
{
    std::uint16_t magic = 0x4D42;
    std::uint32_t file_size = 0;
    std::uint16_t reserved = 0;
    std::uint16_t reserved_other = 0;
    std::uint32_t data_offset = 0;
};

struct BMPInfoHeader
{
    std::uint32_t size = 0;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint16_t planes = 1;
    std::uint16_t bit_count = 24;
    std::uint32_t compression = 0;
    std::uint32_t size_image = 0;
    std::int32_t x = 2835;
    std::int32_t y = 2835;
    std::uint32_t clr_used = 0;
    std::uint32_t clr_important = 0;
};

struct BMPColorHeader {
    uint32_t red_mask{ 0x00ff0000 };
    uint32_t green_mask{ 0x0000ff00 };
    uint32_t blue_mask{ 0x000000ff };
    uint32_t alpha_mask{ 0xff000000 };
    uint32_t color_space_type{ 0x73524742 };
    uint32_t unused[16]{ 0 };
};
#pragma pack(pop)


class GaussianBlur
{
    uint32_t radius_;
    std::vector<double> kernel_;
    BMPInfoHeader info_;
    BMPHeader header_;
    BMPColorHeader color_header_;

    std::vector<uint8_t> read_bmp_to_matrix(const std::filesystem::path& path_to_bmp);
    void write_matrix_to_bmp(const std::vector<uint8_t> &matrix_with_img, const std::filesystem::path& path_to_result);
    std::vector<uint8_t> blur(const std::vector<uint8_t> &matrix_with_img, bool is_horizontal = true) const;

public:
    explicit GaussianBlur(uint32_t new_radius);
    void run(const std::filesystem::path& path_to_bmp, const std::filesystem::path& path_to_result);
};

#endif //GAUSSIAN_BLUR_HPP
