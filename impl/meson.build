libSubstrateSrcs = [
	'socket.cxx', 'console.cxx', 'pty.cxx'
]

libSubstrate_deps = []
defs = []
incs = [
	include_directories('..')
]

if target_machine.system() == 'windows'
	libSubstrate_deps += cxx.find_library('Ws2_32')
endif

if get_option('socket_t_tls')
	defs += '-DSUBSTRATE_SOCKET_TLS'
	tls_library = get_option('tls_library')
	if tls_library == 'libressl'
		libSubstrate_deps += cxx.find_library('libtls', dirs: [ get_option('libressl_prefix') ], required: true)
		libSubstrate_deps += cxx.find_library('libssl', dirs: [ get_option('libressl_prefix') ], required: true)
		libSubstrate_deps += cxx.find_library('libcrypto', dirs: [ get_option('libressl_prefix') ], required: true)

		defs += '-DSUBSTRATE_SOCKET_TLS_LIBRESSL'
	endif
endif

libSubstrate = library(
	'substrate',
	libSubstrateSrcs,
	include_directories: incs,
	dependencies: libSubstrate_deps,
	gnu_symbol_visibility: 'inlineshidden',
	implicit_include_directories: false,
	cpp_args: [
		defs
	],
	pic: true,
	install: true
)
