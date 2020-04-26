#ifndef PENUX_DEBUG_H

#define PENUX_DEBUG_H

#include <iostream>

namespace penux
{

#define DBG_ERR (0x1)
#define DBG_WARN (0x2)
#define DBG_INFO (0x4)
#define DBG_TRACE (0x8)
#define DBG_ENTER (0x10)
#define DBG_LEAVE (0x20)

} // end namespace penux

#endif // PENUX_DEBUG_H

namespace penux
{

#undef ERROR
#undef WARNING
#undef INFO
#undef TRACE
#undef ENTER
#undef LEAVE

#ifdef DEBUG

#ifndef DBG_LVL
#define DBG_LVL DBG_ERR
#endif

#if (DBG_LVL & DBG_ERR)
#define ERROR(arg) (std::cout << "ERROR: " << arg << std::endl)
#else
#define ERROR(arg)
#endif // ERROR

#if (DBG_LVL & DBG_WARN)
#define WARNING(arg) (std::cout << "WARNING: " << arg << std::endl)
#else
#define WARNING(arg)
#endif // WARNING

#if (DBG_LVL & DBG_INFO)
#define INFO(arg) (std::cout << "INFO: " << arg << std::endl)
#else
#define INFO(arg)
#endif // INFO

#if (DBG_LVL & DBG_TRACE)
#define TRACE(arg) (std::cout << arg << std::endl)
#else
#define TRACE(arg)
#endif // TRACE

#if (DBG_LVL & DBG_ENTER)
#define ENTER(arg) (std::cout << "ENTER: " << arg << std::endl)
#else
#define ENTER(arg)
#endif // ENTER

#if (DBG_LVL & DBG_LEAVE)
#define LEAVE(arg) (std::cout << "LEAVE: " << arg << std::endl)
#else
#define LEAVE(arg)
#endif // LEAVE

#else // DEBUG not defined

#define ERROR(arg)
#define WARNING(arg)
#define INFO(arg)
#define TRACE(arg)
#define ENTER(arg)
#define LEAVE(arg)

#endif // DEBUG

} // end namespace penux
