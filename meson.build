project(
	'substrate',
	'cpp',
	default_options: [
		'cpp_std=c++11',
		'warning_level=3'
	],
	version: '0.0.1'
)

cxx = meson.get_compiler('cpp')
pkgconfig = import('pkgconfig')

subdir('substrate')
subdir('test')

pkgconfig.generate(
	subdirs: '.',
	name: 'substrate',
	filebase: 'substrate',
	version: meson.project_version(),
	description: 'A collection of headers containing useful tools and gadgets for building C++ programs'
)
