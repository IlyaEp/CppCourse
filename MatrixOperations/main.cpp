#include <iostream>
#include <fstream>

#include "matrices.hpp"

int main(int argc, char ** argv)
{
    if (argc < 2 or argc % 2 != 0) {
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (not file) {
        std::cerr << "File " + std::string(argv[1]) + " not exists" << std::endl;
        return 1;
    }

    Matrix base_matrix(file);

    bool wait_for_command = true;
    std::string command;
    for (auto i = 2; i < argc; i++) {
        if (wait_for_command) {
            command = std::string(argv[i]);

            if (command != "--add" && command != "--mult") {
                std::cerr << "Unknown command: " + command << std::endl;
                return 1;
            }

            wait_for_command = false;
            continue;
        }

        std::ifstream file_with_new_matrix(argv[i]);
        if (not file_with_new_matrix) {
            std::cerr << "File " + std::string(argv[i]) + " not exists" << std::endl;
            return 1;
        }

        Matrix matrix(file_with_new_matrix);

        try {
            if (command == "--add")
            {base_matrix += matrix;}

            if (command == "--mult")
            {base_matrix *= matrix;}

        }
        catch (const std::runtime_error &e) {
            std::cerr << std::string(argv[i]) + ": " << e.what() << std::endl;
            return 1;
        }

        wait_for_command = true;
    }

    std::cout << base_matrix;

    return 0;
}
