headers = [
	'fd',
	'memfd',
	'units',
	'utility',
	'enum_utils',
	'fixed_vector',
	'bounded_iterator',
	'string',
	'managed_ptr',
	'socket',
	'console',
	'pty',
	'pipe',
	'mmap',
	'span'

]

advanced_headers = [
	'advanced/io',
	'advanced/fd'
]

internal_headers = [
	'internal/defs',
	'internal/cxx11_compat',
	'internal/fd_compat',
	'internal/tls_wrapper',
	'internal/tls_dummy'
]

if get_option('socket_t_tls')
	tls_library = get_option('tls_library')
	if tls_library == 'libressl'
		internal_headers += 'internal/libressl_wrapper'
	endif
endif

install_headers(
	headers,
	subdir: 'substrate'
)

install_headers(
	advanced_headers,
	subdir: 'substrate/advanced'
)

install_headers(
	internal_headers,
	subdir: 'substrate/internal'
)
