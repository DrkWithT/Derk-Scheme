/**
 * @file derkscheme_main.cpp
 * @author DrkWithT
 * @brief Contains driver code for my Scheme interpreter.
 * @version 0.1.0
 * @date 2025-04-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <print>
#include <string_view>
#include "utils/file_io.hpp"

constexpr auto min_argc = 2;

constexpr auto derkscheme_version_major = 0;
constexpr auto derkscheme_version_minor = 1;
constexpr auto derkscheme_version_patch = 0;

int main(int argc, char* argv[]) {
    using namespace DerkScheme;

    if (argc < min_argc) {
        std::print(std::cerr, "usage: {} <action | file>\n<action> = [-help | -vers]\n", argv[0]);
        return 1;
    }

    std::string_view arg_1 {argv[1]};

    if (arg_1 == "-help") {
        std::print(
            "usage: derkscheme <action | file>\n<action> = [-help | -vers]\n"
        );
        return 0;
    } else if (arg_1 == "-vers") {
        std::print("DerkScheme v{}.{}.{}:\nInitial version, WIP.\n",
            derkscheme_version_major,
            derkscheme_version_minor,
            derkscheme_version_patch
        );
        return 0;
    }

    auto source = Utils::FileIO::read_file_sync(arg_1.data(), Utils::FileIO::ReadMode::textual_mode);

    std::print("Source:\n\n{}\n", source);
}
