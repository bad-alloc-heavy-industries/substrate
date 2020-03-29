libSubstrateSrcs = [
	'socket.cxx'
]

libSubstrate = library(
	'substrate',
	libSubstrateSrcs,
	include_directories: include_directories('..'),
	gnu_symbol_visibility: 'inlineshidden',
	implicit_include_directories: false,
	pic: true,
	install: true
)
