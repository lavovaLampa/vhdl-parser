#include "error.hpp"

#include <iostream>

std::ostream& Error::operator<<(std::ostream& out)
{
    if (this->msg.has_value()) {
        out << "Error message: \""
            << this->msg.value()
            << "\"";
    }

    return out;
}