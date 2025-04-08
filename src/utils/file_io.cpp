#include <fstream>
#include <sstream>
#include "utils/file_io.hpp"

namespace DerkScheme::Utils::FileIO {
    std::string read_file_sync(const std::string& path, ReadMode mode)
    {
        if (mode != ReadMode::textual_mode) {
            return ""; // binary files not supported for now...
        }

        std::ifstream reader {path};
        std::ostringstream sout;
        std::string temp;

        while (not std::getline(reader, temp).eof()) {
            sout << temp << '\n';
            temp.clear();
        }

        return sout.str();
    }
}
