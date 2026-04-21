#include "../include/core/time.hpp"

namespace engine::core {

Time::Time()
    : start_time(clock::now()),
      last_frame_time(start_time),
      delta_time_value(0.0f),
      elapsed_time_value(0.0f) {
}

void Time::update() {
    const clock::time_point current_time = clock::now();

    delta_time_value = std::chrono::duration<float>(current_time - last_frame_time).count();

    elapsed_time_value = std::chrono::duration<float>(current_time - start_time).count();

    last_frame_time = current_time;
}

float Time::get_delta_time() const {
    return delta_time_value;
}

float Time::get_elapsed_time() const {
    return elapsed_time_value;
}

}  // namespace engine::core
