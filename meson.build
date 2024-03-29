# SPDX-License-Identifier: BSD-3-Clause
project(
	'substrate',
	'cpp',
	default_options: [
		'cpp_std=c++14',
		'warning_level=3',
		'buildtype=release',
		'default_library=shared',
		'b_ndebug=if-release',
	],
	version: '1.0.0',
	license: 'BSD-3-Clause',
	subproject_dir: 'deps'
)

cxx = meson.get_compiler('cpp')
isWindows = target_machine.system() == 'windows'
pkgconfig = import('pkgconfig')
coverage = get_option('b_coverage')

extended_warnings = [
	'-Wdouble-promotion',
	'-Wformat=2',
	'-Wformat-overflow=2',
	'-Wformat-signedness',
	'-Wformat-truncation',
	'-Wnull-dereference',
	'-Wmissing-attributes',
	'-Wmissing-braces',
	'-Wsequence-point',
	'-Wreturn-type',
	'-Wunused',
	'-Wunused-local-typedefs',
	'-Wunused-const-variable=2',
	'-Wmaybe-uninitialized',
	'-Wunknown-pragmas',
	'-Wstrict-aliasing',
	'-Wstrict-overflow=3',
	'-Wstring-compare',
	'-Wstringop-overflow',
	'-Warith-conversion',
	'-Wvla-parameter',
	'-Wduplicated-branches',
	'-Wshadow=local',
	'-Wunsafe-loop-optimizations',
	'-Wbad-function-cast',
	'-Wcast-qual',
	'-Wcast-align=strict',
	'-Wcast-function-type',
	'-Wconversion',
	'-Wdangling-else',
	'-Wsign-conversion',
	'-Wfloat-conversion',
	'-Wpacked',
#	'-Wpadded',
	'-Wredundant-decls',
#	'-Winline',
	'-Wvla',
	'-Wstack-protector',
	'-Wunsuffixed-float-constant',
	'-Wimplicit-fallthrough',
	'-Werror=switch',
	'-we4061' # enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
]

add_project_arguments(
	cxx.get_supported_arguments(extended_warnings),
	language: 'cpp'
)


if cxx.get_id() == 'gcc' and cxx.version() < '7.0.0'
	add_project_arguments(
		cxx.get_supported_arguments(['-Wno-attributes']),
		language: 'cpp'
	)
elif cxx.get_id() == 'clang'
	add_project_arguments(
		cxx.get_supported_arguments(['-Wno-c++17-extensions']),
		language: 'cpp'
	)
endif

# Add default defines
if isWindows
	add_project_arguments(
		'-DWIN32',
		'-D_WIN32',
		'-D_WINDOWS',
		'-D_CRT_NONSTDC_NO_DEPRECATE',
		'-D_CRT_SECURE_NO_WARNINGS',
		language: ['c', 'cpp']
	)

	add_project_arguments(cxx.get_supported_arguments('-guard:cf'), language: ['cpp'])
	add_project_link_arguments(cxx.get_supported_link_arguments('-guard:cf', '-Wl,-guard:cf'), language: ['cpp'])

	if target_machine.cpu_family() == 'x86_64' or target_machine.cpu_family() == 'aarch64'
		add_project_arguments('-D_WIN64', language: ['c', 'cpp'])
	endif

	# MinGW defines the SDK minimum target to Server 2003
	# Console APIs require Windows 10 17763 (Redstone 5)
	if cxx.get_id() == 'gcc' or cxx.get_id() == 'clang'
		add_project_arguments('-DWINVER=NTDDI_WIN10_RS5', '-D_WIN32_WINNT=NTDDI_WIN10_RS5', '-DNTDDI_VERSION=NTDDI_WIN10_RS5', language: ['c', 'cpp'])
	endif

	# if MSVC, b_coverage will always return false because
	# Meson doesn't officially support coverage
	if cxx.get_id() == 'msvc' and get_option('buildtype') == 'debug'
		coverageRunner = find_program('OpenCppCoverage', required: false)

		message('Enabling coverage under MSVC')
		coverageArgs = [
			'--sources', '@0@\\impl\\*'.format(meson.current_source_dir()),
			'--sources', '@0@\\substrate\\*'.format(meson.current_source_dir()),
			'--sources', '@0@\\*'.format(meson.current_build_dir()),
			'--modules', meson.current_build_dir(),
			'--export_type'
		]
		coverage = true
	endif
endif

cxxVersion = cxx.get_define('__cplusplus').substring(0, -1)

if not get_option('build_library')
	targetLibraryBuildable = false
	libSubstrate = []
	libSubstrateArgs = []
endif

subdir('substrate')
subdir('impl')

if get_option('gen_docs')
	message('Building documentation')
	subdir('docs')
endif

if not meson.is_subproject() and get_option('build_library')
	pkgconfig.generate(
		subdirs: '.',
		name: 'substrate',
		filebase: 'substrate',
		libraries: [libSubstrate, deps],
		version: meson.project_version(),
		extra_cflags: libSubstrateArgs,
		variables: [
			'command_line_enabled=@0@'.format(substrateCommandLineEnabled),
		],
		description: 'A collection of headers containing useful tools and gadgets for building C++ programs'
	)
endif

variablesCompileArgs = [
	'-I@0@'.format(meson.current_source_dir()),
] + libSubstrateArgs

variables = {
	'compile_args': ' '.join(variablesCompileArgs),
	'command_line_enabled': substrateCommandLineEnabled.to_string(),
}

nativeDeps = deps
if targetLibraryBuildable
	variables += {
		'link_args': libSubstrate.full_path(),
	}
else
	deps = []
endif

substrate_dep = declare_dependency(
	dependencies: deps,
	include_directories: include_directories('.'),
	compile_args: variablesCompileArgs,
	link_with: libSubstrate,
	variables: variables,
	version: meson.project_version()
)

if get_option('build_tests') and targetLibraryBuildable
	if cxxVersion.version_compare('>=201402')
		subdir('test')
	else
		warning('Catch2 requires C++14 to build the tests, skipping')
	endif
endif

# When we're in a cross-build environment, also declare the native version of the library
if meson.is_cross_build()
	substrateNativeArgs = ' '.join(['-I@0@'.format(meson.current_source_dir())] + libSubstrateArgs)
	substrateNative_dep = declare_dependency(
		dependencies: depsNative,
		include_directories: include_directories('.'),
		compile_args: substrateNativeArgs,
		link_with: libSubstrateNative,
		variables: {
			'compile_args': substrateNativeArgs,
			'link_args': libSubstrateNative.full_path(),
			'command_line_enabled': substrateNativeCommandLineEnabled.to_string(),
		},
		version: meson.project_version()
	)
endif

runClangTidy = find_program('runClangTidy.py')
run_target(
	'clang-tidy',
	command: [runClangTidy, '-s', meson.current_source_dir(), '-p', meson.current_build_dir()]
)
