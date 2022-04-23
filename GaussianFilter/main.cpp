#include <iostream>
#include "blur.hpp"

struct Arguments {
    std::filesystem::path input_file = "";
    std::filesystem::path output_file = "";
    uint32_t kernel_size = 0;
};

Arguments parse_arguments(int argc, char **argv) {
    if (argc != 7) {
        throw std::runtime_error("Not enough arguments");
    }

    Arguments result;
    auto argument = std::string(argv[1]);
    bool wait_argument = false;
    for (auto i = 2; i < argc; ++i) {
        if (wait_argument) {
            argument = std::string(argv[i]);
            wait_argument = false;
            continue;
        }

        if (argument == "-i") {
            result.input_file = std::filesystem::path(argv[i]);
            wait_argument = true;
        }

        if (argument == "-o") {
            result.output_file = std::filesystem::path(argv[i]);
            wait_argument = true;
        }

        if (argument == "-r") {
            result.kernel_size = std::stoi(std::string(argv[i]));
            wait_argument = true;
        }

        if (not wait_argument) {
            throw std::runtime_error("Unknown command: " + argument);
        }
    }

    return result;
}

int main(int argc, char ** argv)
{
    Arguments arguments;
    try {
        arguments = parse_arguments(argc, argv);
    }
    catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (not std::filesystem::exists(arguments.input_file)) {
        std::cerr << "File " << arguments.input_file << " not exists" << std::endl;
        return 1;
    }

    if (arguments.kernel_size % 2 == 0) {
        std::cerr << "Kernel size not valid" << std::endl;
        return 1;
    }

    auto algo = GaussianBlur(arguments.kernel_size);

    try {
        algo.run(arguments.input_file, arguments.output_file);
    }
    catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}