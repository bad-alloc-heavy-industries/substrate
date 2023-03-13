# SPDX-License-Identifier: BSD-3-Clause
project(
	'substrate',
	'cpp',
	default_options: [
		'cpp_std=c++11',
		'warning_level=3'
	],
	version: '0.0.1',
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
	'-Winline',
	'-Wvla',
	'-Wstack-protector',
	'-Wunsuffixed-float-constant',
	'-Wimplicit-fallthrough',
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

	if target_machine.cpu_family() == 'x86_64' or target_machine.cpu_family() == 'aarch64'
		add_project_arguments('-D_WIN64', language: ['c', 'cpp'])
	endif

	# MinGW defines the SDK minimum target to Server 2003
	# Console APIs require Windows 10 17763 (Redstone 5)
	if cxx.get_id() == 'gcc' or cxx.get_id() == 'clang'
		add_project_arguments('-DWINVER=NTDDI_WIN10_RS5', '-D_WIN32_WINNT=NTDDI_WIN10_RS5', '-DNTDDI_VERSION=NTDDI_WIN10_RS5', language: ['c', 'cpp'])
	endif

	if coverage
		coverageRunner = find_program('OpenCppCoverage', required: false)
		coverageArgs = [
			'--sources', '@0@\impl*\*'.format(meson.current_source_dir()),
			'--sources', '@0@\substrate*\*'.format(meson.current_source_dir()),
			'--sources', '@0@\test*\*'.format(meson.current_source_dir()),
			'--sources', '@0@\*'.format(meson.current_build_dir()),
			'--modules', meson.current_build_dir(),
			'--export_type'
		]
		coverage = coverageRunner.found() and debug
	endif
endif

subdir('substrate')
if get_option('build_library')
	subdir('impl')
else
	targetLibraryBuildable = false
	deps = []
	libSubstrate = []
	libSubstrateArgs = []
endif

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
		description: 'A collection of headers containing useful tools and gadgets for building C++ programs'
	)
endif

variables_compile_args = [
	'-I@0@'.format(meson.current_source_dir()),
] + libSubstrateArgs

variables_link_args = []

if targetLibraryBuildable
	variables_link_args += libSubstrate.full_path()
endif

substrate_dep = declare_dependency(
	dependencies: deps,
	include_directories: include_directories('.'),
	link_with: libSubstrate,
	variables: {
		'compile_args': ' '.join(variables_compile_args),
		'link_args': ' '.join(variables_link_args)
	},
	version: meson.project_version()
)

if get_option('build_tests') and targetLibraryBuildable
	subdir('test')
endif

# When we're in a cross-build environment, also declade the native version of the library
if meson.is_cross_build()
	substrate_native_dep = declare_dependency(
		dependencies: deps,
		include_directories: include_directories('.'),
		link_with: libSubstrateNative,
		variables: {
			'compile_args': ' '.join(['-I@0@'.format(meson.current_source_dir())] + libSubstrateArgs),
			'link_args': libSubstrateNative.full_path()
		},
		version: meson.project_version()
	)
endif

runClangTidy = find_program('runClangTidy.py')
run_target(
	'clang-tidy',
	command: [runClangTidy, '-s', meson.current_source_dir(), '-p', meson.current_build_dir()]
)
