libSubstrateSrcs = [
	'socket.cxx'
]

libSubstrateLibs = []

if target_machine.system() == 'windows'
	libSubstrateLibs += cxx.find_library('Ws2_32')
endif

libSubstrate = library(
	'substrate',
	libSubstrateSrcs,
	include_directories: include_directories('..'),
	dependencies: libSubstrateLibs,
	gnu_symbol_visibility: 'inlineshidden',
	implicit_include_directories: false,
	pic: true,
	install: true
)
