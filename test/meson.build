testSrcs = [
	'test.cxx', 'fd.cxx', 'units.cxx', 'utility.cxx', 'enum_utils.cxx',
]

if target_machine.system() == 'linux'
	testSrcs += [
		'memfd.cxx'
	]
endif

tests = executable(
	'tests',
	testSrcs,
	include_directories: include_directories(
		'..', 'catch2/single_include/catch2'
	),
	gnu_symbol_visibility: 'inlineshidden',
	implicit_include_directories: false,
	install: false
)

test(
	'substrate',
	tests,
	workdir: meson.current_build_dir()
)
