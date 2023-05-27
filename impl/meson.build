# SPDX-License-Identifier: BSD-3-Clause
libSubstrateSrcs = [
	'socket.cxx', 'console.cxx', 'affinity.cxx', 'thread.cxx',
]

deps = []

if target_machine.system() == 'windows'
	deps += cxx.find_library('ws2_32')
else
	libSubstrateSrcs += 'pty.cxx'
endif

deps += dependency('threads')

if cxx.get_define('__cplusplus').substring(0, -1).version_compare('>=201703')
	friendTypenameTest = '''
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
	friendTypenameTemplate = cxx.compiles(
		friendTypenameTest,
		name: 'accepts friend declarations for auto functions (LLVM #31852, #33222)'
	)

	stdVariantGCCTest = '''
		#include <tuple>
		#include <variant>
		int main() {
			using variant_t = std::variant<short, int, long>;
			constexpr auto variant_v = variant_t{std::in_place_index_t<0>{}, short{}};
			constexpr auto tuple = std::make_tuple(variant_v);
			constexpr std::tuple tuple_v{variant_v};
		}
	'''

	stdVariantGCC = cxx.compiles(
		stdVariantGCCTest,
		name: 'has a working std::variant implementation (GCC #80165)'
	)

	stdFilesystemPathTest = '''
		#include <filesystem>
		#include <iostream>

		using namespace std::literals::string_view_literals;

		int main() {
			std::filesystem::path p{"/bin/bash"sv};
			std::cout << p << std::endl;
			return 0;
		}
	'''

	# GCC < 9.1 splits the filesystem module into a separate library
	libstdcppFS = cxx.find_library('stdc++fs', required: false)

	stdFilesystemPath = cxx.links(
		stdFilesystemPathTest,
		name: 'has a working std::filesystem::path implementation (GCC >= 8 in macOS pre 10.15)',
		dependencies: libstdcppFS,
	)

	initializerListTest = '''
		#include <array>
		#include <cstdint>

		using size_t = std::size_t;

		namespace internal
		{
		template<std::size_t... seq> using indexSequence_t = std::index_sequence<seq...>;
		template<std::size_t N> using makeIndexSequence = std::make_index_sequence<N>;

		template<typename T, size_t N, size_t... index> constexpr std::array<T, N>
			makeArray(T (&&elems)[N], indexSequence_t<index...>)
		{
			return {{elems[index]...}};
		}
		} // namespace internal

		template<typename T, size_t N> constexpr std::array<T, N>
			make_array(T (&&elems)[N]) // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
		{
			return internal::makeArray(std::move(elems), internal::makeIndexSequence<N>{});
		}

		int main()
		{
			auto test = make_array<const char *>({
				"program",
				"choiceC",
				nullptr,
			});
		}
	'''

	initializerList = cxx.compiles(
		initializerListTest,
		name: 'accepts nullptr as part of CTAD with pointer type values (GCC #85977)'
	)

	if friendTypenameTemplate and stdVariantGCC and stdFilesystemPath and initializerList
		deps += [libstdcppFS]

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

if isWindows
	rcCfg = configuration_data()
	if cxx.get_id() == 'msvc' or cxx.get_id() == 'clang-cl'
		rcCfg.set_quoted('VER_ORIGINALFILENAME_STR', 'substrate.dll')
	else
		rcCfg.set_quoted('VER_ORIGINALFILENAME_STR', 'libsubstrate.dll')
	endif
	rcCfg.set_quoted('VER_FILEDESCRIPTION_STR', 'A collection of headers containing useful tools and gadgets for building C++ programs')
	rcCfg.set_quoted('VER_PRODUCTNAME_STR', meson.project_name())
	projectVersion = '@0@,0'.format(meson.project_version().replace('.', ','))
	rcCfg.set('VER_FILEVERSION', projectVersion)
	rcCfg.set('VER_PRODUCTVERSION', projectVersion)
	rcCfg.set_quoted('VER_FILEVERSION_STR', meson.project_version())
	rcCfg.set_quoted('VER_PRODUCTVERSION_STR', meson.project_version())
	rcCfg.set('license', ' OR '.join(meson.project_license()))

	windows = import('windows')
	substrateRc = configure_file(
		input: 'substrate.rc.in',
		output: 'substrate.rc',
		configuration: rcCfg
	)
	libSubstrateSrcs += windows.compile_resources(substrateRc)
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
	hostCXX = meson.get_compiler('cpp', native: true)

	if hostCXX.get_define('__cplusplus').substring(0, -1).version_compare('>=201703')
		# Re-check if we should or can enable the command line handling stuff by checking
		# everything again against the host compiler
		friendTypenameTemplate = hostCXX.compiles(
			friendTypenameTest,
			name: 'accepts friend declarations for auto functions (LLVM #31852, #33222)'
		)

		stdVariantGCC = hostCXX.compiles(
			stdVariantGCCTest,
			name: 'has a working std::variant implementation (GCC #80165)'
		)

		# GCC < 9.1 splits the filesystem module into a separate library
		libstdcppFS = hostCXX.find_library('stdc++fs', required: false)

		stdFilesystemPath = hostCXX.links(
			stdFilesystemPathTest,
			name: 'has a working std::filesystem::path implementation (GCC >= 8 in macOS pre 10.15)',
			dependencies: libstdcppFS,
		)

		initializerList = hostCXX.compiles(
			initializerListTest,
			name: 'accepts nullptr as part of CTAD with pointer type values (GCC #85977)'
		)

		if friendTypenameTemplate and stdVariantGCC and stdFilesystemPath and initializerList
			deps += [libstdcppFS]

			subdir('command_line')
		endif
	endif

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
