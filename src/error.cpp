#include "error.hpp"

#include <iostream>
#include <string>


std::ostream& operator<<(std::ostream& out, const Error& err)
{
    if (err.msg.has_value()) {
        out << "Error message: \""
            << err.msg.value()
            << "\"";
    }

    return out;
}