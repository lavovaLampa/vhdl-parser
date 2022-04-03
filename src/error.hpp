#ifndef ERROR
#define ERROR

#include <iostream>
#include <optional>
#include <string>

struct Error {
    std::optional<std::string> msg { std::nullopt };

    Error(std::optional<std::string> msg = std::nullopt)
        : msg(msg)
    {
    }

    friend std::ostream& operator<<(std::ostream& out, const Error& err);
};

#endif /* ERROR */
