#include <vector>
#include <deque>
#include <cstdint>
#include <memory>
#include <gsl/pointers>
#include <filesystem>
#include <optional>


class DesignUnit;
class PrimaryUnit;
class Entity;
class Configuration;
class Package;
class SecondaryUnit;
class Architecture;
class PackageBody;
class DesignLibrary;
class DesignFile;

class DesignUnit {
    Identifier id;

public:
    virtual ~DesignUnit() = default;
};

class PrimaryUnit : public DesignUnit {
};

class Entity : public PrimaryUnit {
    std::vector<std::reference_wrapper<Architecture>> architectures;
};

class Configuration : public PrimaryUnit {
};

class Package : public PrimaryUnit {
    std::optional<PackageBody&> package_body;
};

class SecondaryUnit : public DesignUnit {
};

class Architecture : public SecondaryUnit {
};

class PackageBody : public SecondaryUnit {
    Package& package;
};

class DesignLibrary {
    Identifier library_name;
    std::vector<gsl::not_null<std::unique_ptr<DesignFile>>> design_files;
};

class DesignFile {
    std::filesystem::path file_path;
    DesignLibrary& design_library;
    std::vector<gsl::not_null<std::unique_ptr<DesignUnit>>> design_units;
};