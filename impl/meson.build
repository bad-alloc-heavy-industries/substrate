# SPDX-License-Identifier: BSD-3-Clause
libSubstrateSrcs = [
	'socket.cxx', 'console.cxx'
]

deps = []

if target_machine.system() == 'windows'
	deps += cxx.find_library('ws2_32')
else
	libSubstrateSrcs += 'pty.cxx'
endif

if cxx.get_define('__cplusplus').substring(0, -1).version_compare('>=201703')
	std_variant_test = '''
		template <typename U>
		auto foo(U u)
		{
			u.v;
		}

		template <typename T>
		class X
		{
			T v;

		public:
			template <typename U>
			friend auto foo(U);
		};

		int main()
		{
			::foo(X<int>{});
		}
	'''
	if cxx.compiles(std_variant_test, name: 'has a working std::variant implementation (LLVM #31852, #33222)')
		subdir('command_line')
	endif
endif

libSubstrate = library(
	'substrate',
	libSubstrateSrcs,
	c_args: ['-DSUBSTRATE_BUILD_INTERNAL'],
	cpp_args: ['-DSUBSTRATE_BUILD_INTERNAL'],
	include_directories: include_directories('..'),
	dependencies: deps,
	gnu_symbol_visibility: 'inlineshidden',
	implicit_include_directories: false,
	pic: true,
	install: (not meson.is_subproject()),
)
