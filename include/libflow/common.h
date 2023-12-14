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

#endif // !LIBFLOW_INCLUDE_LIBFLOW_COMMON_H_
