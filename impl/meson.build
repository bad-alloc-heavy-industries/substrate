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

# This is by far not a complete set of conditions, however..
# If we are not in a cross-build environment, or we are not building for ARM Cortex-M or
# we are not building for AVR then we should be able to build the non-native target library.
targetLibraryBuildable = not (meson.is_cross_build() and (
	(target_machine.cpu_family() == 'arm' and target_machine.cpu().startswith('cortex-m'))
	or target_machine.cpu_family() == 'avr'
))

if targetLibraryBuildable
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
else
	libSubstrate = []
endif

# In a cross-build scenario, always build a version of the library for the host too.
if meson.is_cross_build()
	libSubstrateNative = library(
		'substrateNative',
		libSubstrateSrcs,
		c_args: ['-DSUBSTRATE_BUILD_INTERNAL'],
		cpp_args: ['-DSUBSTRATE_BUILD_INTERNAL'],
		include_directories: include_directories('..'),
		dependencies: deps,
		gnu_symbol_visibility: 'inlineshidden',
		implicit_include_directories: false,
		pic: true,
		install: (not meson.is_subproject()),
		native: true,
	)
endif
