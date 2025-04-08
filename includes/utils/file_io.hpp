#pragma once

/**
 * @file file_io.hpp
 * Contains file handling utility functions.
 * @author DrkWithT
 */

// #include <fstream>
// #include <sstream>
#include <string>

namespace DerkScheme::Utils::FileIO {
    enum class ReadMode {
      textual_mode,
      binary_mode
    };

    [[nodiscard]] std::string read_file_sync(const std::string& path, ReadMode mode);
}
