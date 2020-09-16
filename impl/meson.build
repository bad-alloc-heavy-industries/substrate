libSubstrateSrcs = [
	'socket.cxx', 'console.cxx', 'pty.cxx'
]

deps = []

if target_machine.system() == 'windows'
	deps += cxx.find_library('Ws2_32')
endif

libSubstrate = library(
	'substrate',
	libSubstrateSrcs,
	include_directories: include_directories('..'),
	dependencies: deps,
	gnu_symbol_visibility: 'inlineshidden',
	implicit_include_directories: false,
	pic: true,
	install: (not meson.is_subproject())
)
