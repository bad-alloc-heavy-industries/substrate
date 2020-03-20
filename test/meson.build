testSrcs = [
	'test.cxx', 'fd.cxx'
]

tests = executable(
	'tests',
	testSrcs,
	include_directories: include_directories('..', 'catch2/single_include/catch2'),
	gnu_symbol_visibility: 'inlineshidden',
	implicit_include_directories: false,
	install: false
)

test(
	'substrate',
	tests
)
