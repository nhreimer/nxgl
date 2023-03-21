#ifndef INC_2A66D4AC7AAB421AAB551E280E121CAB
#define INC_2A66D4AC7AAB421AAB551E280E121CAB

#include <chrono>

using namespace std::chrono;

namespace nxgl
{
class Clock
{
public:

  Clock() = default;

  float reset()
  {
    auto result = getMilliseconds();
    m_lastMeasurement = high_resolution_clock::now();
    return result;
  }

  [[nodiscard]] inline float getMilliseconds() const
  {
    return duration< float, std::milli >(
      high_resolution_clock::now() -
      m_lastMeasurement ).count();
  }

private:

  high_resolution_clock::time_point m_lastMeasurement;

};
}

#endif //INC_2A66D4AC7AAB421AAB551E280E121CAB
