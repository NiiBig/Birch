cpp{{
#include <chrono>

thread_local static std::chrono::time_point<std::chrono::steady_clock> savedTimePoint = std::chrono::steady_clock::now();
}}

/**
 * Reset timer.
 */
function tic() {
  cpp {{
  savedTimePoint = std::chrono::steady_clock::now();
  }}
}

/**
 * Number of seconds since last call to `tic()`.
 */
function toc() -> Real {
  elapsed:Real;
  cpp {{
  std::chrono::duration<double> e = std::chrono::steady_clock::now() - savedTimePoint;
  elapsed = e.count();
  }}
  return elapsed;
}
