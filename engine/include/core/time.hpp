#ifndef ENGINE_CORE_TIME_HPP
#define ENGINE_CORE_TIME_HPP

#include <chrono>

namespace engine::core {

class Time final {
public:
    Time();

    void update();

    float get_delta_time() const;
    float get_elapsed_time() const;

private:
    using clock = std::chrono::steady_clock;

    clock::time_point start_time;
    clock::time_point last_frame_time;
    float delta_time_value;
    float elapsed_time_value;
};

}  // namespace engine::core

#endif  // ENGINE_CORE_TIME_HPP
