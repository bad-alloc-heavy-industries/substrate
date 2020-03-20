project(
	'substrate',
	'cpp',
	version: '0.0.1'
)

pkgconfig = import('pkgconfig')

headers = [
	'fd'
]

install_headers(
	headers,
	subdir: 'substrate'
)

pkgconfig.generate(
	subdirs: '.',
	name: 'substrate',
	filebase: 'substrate',
	version: meson.project_version(),
	description: 'A collection of headers containing useful tools and gadgets for building C++ programs'
)
