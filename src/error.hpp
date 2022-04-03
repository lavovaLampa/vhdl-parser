#ifndef ERROR
#define ERROR

#include <optional>

struct Error {
    std::optional<std::string> msg { std::nullopt };

    Error(std::optional<std::string> msg = std::nullopt)
        : msg(msg)
    {
    }

    inline virtual std::ostream& operator<<(std::ostream& out)
    {
        if (this->msg.has_value()) {
            out << "Error message: \""
                << this->msg.value()
                << "\"";
        }

        return out;
    }
};

#endif /* ERROR */
