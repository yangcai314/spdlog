//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#include <string>
#include <initializer_list>
#include <chrono>
#include <memory>
#include <atomic>
#include <exception>
#if defined(_WIN32) && defined(SPDLOG_WCHAR_FILENAMES)
#include <codecvt>
#include <locale>
#endif

#include <spdlog/details/null_mutex.h>

//visual studio upto 2013 does not support noexcept nor constexpr
#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define SPDLOG_NOEXCEPT throw()
#define SPDLOG_CONSTEXPR
#else
#define SPDLOG_NOEXCEPT noexcept
#define SPDLOG_CONSTEXPR constexpr
#endif

#if defined(__GNUC__)  || defined(__clang__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("DEPRECATED")
#define DEPRECATED
#endif

namespace spdlog
{

class formatter;

namespace sinks
{
class sink;
}

using log_clock = std::chrono::system_clock;
using sink_ptr = std::shared_ptr < sinks::sink >;
using sinks_init_list = std::initializer_list < sink_ptr >;
using formatter_ptr = std::shared_ptr<spdlog::formatter>;
#if defined(SPDLOG_NO_ATOMIC_LEVELS)
using level_t = details::null_atomic_int;
#else
using level_t = std::atomic_int;
#endif


#define SPDLOG_LEVEL_TRACE 0
#define SPDLOG_LEVEL_DEBUG 1
#define SPDLOG_LEVEL_INFO 2
#define SPDLOG_LEVEL_WARN 3
#define SPDLOG_LEVEL_ERR 4
#define SPDLOG_LEVEL_CRIT 5
#define SPDLOG_LEVEL_OFF 6

//Log level enum
namespace level
{
typedef enum
{
    trace = SPDLOG_LEVEL_TRACE,
    debug = SPDLOG_LEVEL_DEBUG,
    info = SPDLOG_LEVEL_INFO,
    warn = SPDLOG_LEVEL_WARN,
    err = SPDLOG_LEVEL_ERR,
    critical = SPDLOG_LEVEL_CRIT,
    off = SPDLOG_LEVEL_OFF
} level_enum;

static const char* level_names[] { "trace", "debug", "info",  "warning", "error", "critical", "off"};

static const char* short_level_names[] { "T", "D", "I", "W", "E", "C", "O"};

inline const char* to_str(spdlog::level::level_enum l)
{
    return level_names[l];
}

inline const char* to_short_str(spdlog::level::level_enum l)
{
    return short_level_names[l];
}
} //level


//
// Async overflow policy - block by default.
//
enum class async_overflow_policy
{
    block_retry, // Block / yield / sleep until message can be enqueued
    discard_log_msg // Discard the message it enqueue fails
};


//
// Log exception
//
class spdlog_ex : public std::exception
{
public:
    spdlog_ex(const std::string& msg) :_msg(msg) {}
    const char* what() const SPDLOG_NOEXCEPT override
    {
        return _msg.c_str();
    }
private:
    std::string _msg;

};

//
// wchar support for windows file names (SPDLOG_WCHAR_FILENAMES must be defined)
//
#if defined(_WIN32) && defined(SPDLOG_WCHAR_FILENAMES)
using filename_t = std::wstring;
#else
using filename_t = std::string;
#endif

#define SDLOG_STR_HELPER(x) #x
#define SPDLOG_STR(x) SDLOG_STR_HELPER(x)

} //spdlog
