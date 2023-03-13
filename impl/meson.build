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
	friend_typename_test = '''
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

	friend_typename_template = cxx.compiles(friend_typename_test, name: 'accepts friend declarations for auto functions (LLVM #31852, #33222)')

	std_variant_gcc_test = '''
		#include <tuple>
		#include <variant>
		int main() {
			using variant_t = std::variant<short, int, long>;
			constexpr auto variant_v = variant_t{std::in_place_index_t<0>{}, short{}};
			constexpr auto tuple = std::make_tuple(variant_v);
			constexpr std::tuple tuple_v{variant_v};
		}
	'''

	std_variant_gcc = cxx.compiles(std_variant_gcc_test, name: 'has a working std::variant implementation (GCC #80165)')

	if friend_typename_template and std_variant_gcc
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

library_type = get_option('default_library')

# xclaesse says that library_type == 'both' is not yet supported on Windows
# Reference: https://gitlab.gnome.org/GNOME/glib/-/blob/main/meson.build#L279
# See: https://matrix.to/#/!GNNuZCPFOXyoAJPmtC:matrix.org/$Igksu0vUvmDxcEKZUpQ2KZCduFiPA22Q2Z7veH5ZFUE
if library_type == 'both' and isWindows
  error('On Windows default_library must be "shared" or "static" but not "both"')
endif

libSubstrateArgs = []

if library_type == 'static' and isWindows
	libSubstrateArgs = ['-DSUBSTRATE_BUILD_STATIC']
endif

if targetLibraryBuildable
	libSubstrate = library(
		'substrate',
		libSubstrateSrcs,
		c_args: ['-DSUBSTRATE_BUILD_INTERNAL'] + libSubstrateArgs,
		cpp_args: ['-DSUBSTRATE_BUILD_INTERNAL'] + libSubstrateArgs,
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
		c_args: ['-DSUBSTRATE_BUILD_INTERNAL'] + libSubstrateArgs,
		cpp_args: ['-DSUBSTRATE_BUILD_INTERNAL'] + libSubstrateArgs,
		include_directories: include_directories('..'),
		dependencies: deps,
		gnu_symbol_visibility: 'inlineshidden',
		implicit_include_directories: false,
		pic: true,
		install: (not meson.is_subproject()),
		native: true,
	)
endif
