#include "datetime.hpp"

#include <ctime>
#include <iomanip>

namespace core
{
  datetime::datetime() = default;

  datetime::~datetime() = default;

  auto datetime::Now(const std::string &format) -> const std::string
  {
    // Get current time
    std::time_t currentTime = std::time(nullptr);

    // Convert current time to struct tm
    std::tm timeInfo;
    localtime_r(&currentTime, &timeInfo); // For POSIX

    // Format time according to the given format
    std::stringstream ss;
    ss << std::put_time(&timeInfo, format.c_str());
    return ss.str();
  }
} // namespace core
