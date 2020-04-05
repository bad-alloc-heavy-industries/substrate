#include <cstdlib>
#include <fcntl.h>
#include <array>

#include <substrate/pty>

using substrate::pty_t;

pty_t::pty_t() noexcept : ptyMaster{[]() noexcept -> fd_t
	{
		fd_t result = posix_openpt(O_RDWR | O_NOCTTY);
		if (result.valid() && (grantpt(result) || unlockpt(result)))
			return {};
		return result;
	}()},
	ptySlave{[this]() noexcept -> fd_t
	{
		if (!ptyMaster.valid())
			return {};
#ifdef TIOCGPTPEER
		const auto pty = ioctl(ptyMaster, TIOCGPTPEER, O_RDWR | O_NOCTTY);
		if (pty >= 0)
			return {pty};
#else
		try
		{
			std::array<char, 512> ptsName;
			if (!ptsname_r(ptyMaster, ptsName.data(), ptsName.size()))
				return {ptsName.data(), O_RDWR | O_NOCTTY};
		}
		catch (std::exception &)
			{ return {}; }
#endif
		ptyMaster = {};
		return {};
	}()} { }
