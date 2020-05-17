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
