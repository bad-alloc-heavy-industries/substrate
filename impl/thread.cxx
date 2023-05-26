#ifdef _WIN32 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#if !defined(_WIN32) || defined(__WINPTHREADS_VERSION)
// MSYS2 + winpthreads need both APIs to translate between native_handle_type
// (pthread) and HANDLE (native).
#include <unistd.h>
#include <pthread.h>
#endif

#include "substrate/thread"

namespace substrate
{
namespace thread
{
std::thread::native_handle_type currentThread()
{
#ifdef _POSIX_THREADS
	return pthread_self();
#elif defined(_WIN32)
	return GetCurrentThread();
#else
	throw std::runtime_error("not implemented");
#endif
}
} // namespace thread
} // namespace substrate
