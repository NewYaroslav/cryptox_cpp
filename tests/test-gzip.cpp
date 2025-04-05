#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <gzip/config.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

/// \brief Reads the entire binary file into a std::string.
/// \param file_path Path to the file.
/// \return The file's content as a string.
/// \throws std::runtime_error If the file cannot be opened or read.
std::string read_file_to_string(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer(size, '\0');
    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Failed to read file: " + file_path);
    }

    return buffer;
}

/// \brief Saves a std::string to a text file.
/// \param file_path Path to the file.
/// \param content The string to save to the file.
/// \throws std::runtime_error If the file cannot be opened or written to.
void save_string_to_file(const std::string& file_path, const std::string& content) {
    std::ofstream file(file_path, std::ios::out);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + file_path);
    }

    file << content;
    if (!file) {
        throw std::runtime_error("Failed to write to file: " + file_path);
    }
}

bool test_gzip_file(const std::string &input_file_name, const std::string &ouput_file_name) {
    // Read the content of the compressed file into a string
    std::string gzip_data = read_file_to_string(input_file_name);

    // Check if the file is compressed
    if (!gzip::is_compressed(gzip_data.c_str(), gzip_data.size())) {
        std::cerr << "The file is not compressed." << std::endl;
        return false;
    }

    std::cout << "Decompressing the file..." << std::endl;

    // Decompress the gzip data
    std::string decompressed_data = gzip::decompress(gzip_data);

    // Save the decompressed data to a new file
    save_string_to_file(ouput_file_name, decompressed_data);

    std::cout << "Decompression successful. File saved as " << ouput_file_name << std::endl;
    return true;
}

int main() {
    try {
        test_gzip_file("BTCUSDT2020-03-25.csv.gz", "BTCUSDT2020-03-25.csv.gz");
        test_gzip_file("BTCUSDT-1d-2025-01-15.zip", "BTCUSDT-1d-2025-01-15.gz");

        /*
        // Read the content of the compressed file into a string
        std::string gzip_data = read_file_to_string("BTCUSDT2020-03-25.csv.gz");

        // Check if the file is compressed
        if (!gzip::is_compressed(gzip_data.c_str(), gzip_data.size())) {
            std::cerr << "The file is not compressed." << std::endl;
            return 1;
        }

        std::cout << "Decompressing the file..." << std::endl;

        // Decompress the gzip data
        std::string decompressed_data = gzip::decompress(gzip_data);

        // Save the decompressed data to a new file
        save_string_to_file("BTCUSDT2020-03-25.csv", decompressed_data);

        std::cout << "Decompression successful. File saved as BTCUSDT2020-03-25.csv." << std::endl;
        */

    } catch (const std::exception& e) {
        // Handle any errors during reading, writing, or decompression
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
