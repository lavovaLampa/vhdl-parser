#pragma once

#include <filesystem>
#include <vector>

namespace pre_parse {

namespace fs = std::filesystem;

enum class library_unit_kind {
    entity,
    configuration,
    package,
    architecture,
    package_body
};

struct LibraryUnit {
    fs::path file;
    std::optional<std::string> name;
    library_unit_kind kind;
    size_t offset;
    size_t len;
};

std::vector<LibraryUnit> pre_parse_file(fs::path file);

} // namespace pre_parse