project(
	'substrate',
	'cpp',
	default_options: [
		'cpp_std=c++11',
		'warning_level=3'
	],
	version: '0.0.1',
	license: 'BSD-3-Clause'
)

cxx = meson.get_compiler('cpp')
pkgconfig = import('pkgconfig')

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

subdir('substrate')
subdir('impl')

if get_option('build_tests')
	subdir('test')
endif

if get_option('gen_docs')
	message('Building documentation')
	subdir('docs')
endif

if not meson.is_subproject()
	pkgconfig.generate(
		subdirs: '.',
		name: 'substrate',
		filebase: 'substrate',
		libraries: [libSubstrate],
		version: meson.project_version(),
		description: 'A collection of headers containing useful tools and gadgets for building C++ programs'
	)
endif

substrate_dep = declare_dependency(
	dependencies: deps,
	include_directories: include_directories('.'),
	link_with: libSubstrate,
	version: meson.project_version()
)

runClangTidy = find_program('runClangTidy.py')
run_target(
	'clang-tidy',
	command: [runClangTidy, '-s', meson.current_source_dir(), '-p', meson.current_build_dir()]
)
