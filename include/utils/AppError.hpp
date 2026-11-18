#pragma once
#include <stdexcept>
#include <string>

namespace als {
// An error that carries the HTTP status the API layer should answer with.
class AppError : public std::runtime_error {
public:
    AppError(int status, const std::string& message) : std::runtime_error(message), status_(status) {}
    int status() const noexcept { return status_; }

private:
    int status_;
};
}  // namespace als
