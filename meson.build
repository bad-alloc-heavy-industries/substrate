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
subdir('test')

if get_option('gen_docs')
	message('Building documentation')
	subdir('docs')
endif

pkgconfig.generate(
	subdirs: '.',
	name: 'substrate',
	filebase: 'substrate',
	libraries: [libSubstrate],
	version: meson.project_version(),
	description: 'A collection of headers containing useful tools and gadgets for building C++ programs'
)

runClangTidy = find_program('runClangTidy.py')
run_target(
	'clang-tidy',
	command: [runClangTidy, '-s', meson.current_source_dir(), '-p', meson.current_build_dir()]
)
