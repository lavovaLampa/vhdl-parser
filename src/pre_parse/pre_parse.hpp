#pragma once

#include <cstddef>
#include <filesystem>
#include <vector>
#include <tl/optional.hpp>


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
    tl::optional<std::string> name;
    size_t offset;
    size_t len;
    library_unit_kind kind;
};

std::vector<LibraryUnit> pre_parse_file(fs::path file);

} // namespace pre_parse