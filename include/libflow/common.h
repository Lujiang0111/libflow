#ifndef LIBFLOW_INCLUDE_LIBFLOW_COMMON_H_
#define LIBFLOW_INCLUDE_LIBFLOW_COMMON_H_

#include <cstddef>
#include <cstdint>
#include <memory>

#if defined(_WIN32)
#if defined(LIBFLOW_API_EXPORT)
#define LIBFLOW_API __declspec(dllexport)
#else
#define LIBFLOW_API __declspec(dllimport)
#endif
#else
#define LIBFLOW_API
#endif

namespace lflow
{

constexpr int64_t kUsInNs = 1000LL;
constexpr int64_t kMsInNs = 1000LL * kUsInNs;
constexpr int64_t kSecInNs = 1000LL * kMsInNs;

constexpr int64_t kSecInMs = 1000LL;
constexpr int64_t kSecInUs = 1000LL * kSecInMs;

constexpr int64_t kMsInTs = 90LL;
constexpr int64_t kSecInTs = 1000LL * kMsInTs;

}

#endif // !LIBFLOW_INCLUDE_LIBFLOW_COMMON_H_
