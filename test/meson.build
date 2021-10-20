testSrcs = [
	'test.cxx', 'fd.cxx', 'units.cxx', 'utility.cxx', 'enum_utils.cxx',
	'bounded_iterator.cxx', 'fixed_vector.cxx', 'string.cxx', 'managed_ptr.cxx',
	'socket.cxx', 'console.cxx', 'advanced/fd.cxx', 'span.cxx', 'conversions.cxx',
	'bits.cxx', 'prng.cxx', 'hash.cxx', 'index_sequence.cxx', 'indexed_iterator.cxx'
]

if target_machine.system() == 'linux'
	testSrcs += [
		'memfd.cxx',
		'mmap.cxx'
	]
endif

# Work around https://github.com/mesonbuild/meson/issues/1426
deps = []
if target_machine.system() == 'windows'
	deps += cxx.find_library('ws2_32')
endif

catch2 = dependency(
	'catch2',
	required: true,
	version: '>=2.13.3',
	fallback: ['catch2', 'catch2_dep']
)

deps += catch2

tests = executable(
	'tests',
	testSrcs,
	include_directories: include_directories(
		'..'
	),
	dependencies: deps,
	link_with: libSubstrate,
	gnu_symbol_visibility: 'inlineshidden',
	implicit_include_directories: false,
	install: false,
)

if isWindows and coverage
	test(
		'substrate',
		coverageRunner,
		args: coverageArgs + ['cobertura:crunch-none-coverage.xml', '--', tests],
		workdir: meson.current_build_dir()
	)
else
	test(
		'substrate',
		tests,
		workdir: meson.current_build_dir()
	)
endif

valgrind = find_program('valgrind', required: false)
if get_option('valgrind_checks') and valgrind.found()
	common_args = [
		'--track-fds=yes',
		'--demangle=yes',
		'--num-callers=50',
		'--error-limit=no',
		'--keep-debuginfo=yes',
		'--show-error-list=yes',
		'--read-inline-info=yes',
		'--read-var-info=yes',
		'--trace-children=yes',
		'--run-libc-freeres=yes',
		'--run-cxx-freeres=yes',
		'--fair-sched=yes',
	]

	memcheck = run_target('memcheck',
		command: [
			valgrind,
			'--tool=memcheck',
			common_args,
			'--xml=yes',
			'--xml-file=@0@/@1@.%p.err.xml'.format(meson.build_root(), meson.project_name()),
			'--leak-check=full',
			'--show-leak-kinds=all',
			'--leak-resolution=high',
			'--leak-check-heuristics=all',
			'--xtree-leak=yes',
			'--xtree-leak-file=@0@/@1@.%p.kcg'.format(meson.build_root(), meson.project_name()),
			'--undef-value-errors=yes',
			'--track-origins=yes',
			'--expensive-definedness-checks=yes',
			tests
		],
		depends: tests
	)

	callgrind = run_target('callgrind',
		command: [
			valgrind,
			'--tool=callgrind',
			common_args,
			'--dump-line=yes',
			'--collect-jumps=yes',
			'--collect-systime=yes',
			'--collect-bus=yes',
			'--separate-threads=yes',
			'--cache-sim=yes',
			'--branch-sim=yes',
			'--simulate-wb=yes',
			'--cacheuse=yes',
			'--callgrind-out-file=@0@/@1@.%p.cg'.format(meson.build_root(), meson.project_name()),
			tests
		],
		depends: tests
	)

	massif = run_target('massif',
		command: [
			valgrind,
			'--tool=massif',
			common_args,
			'--heap=yes',
			'--stacks=yes',
			'--depth=50',
			'--massif-out-file=@0@/@1@.%p.mf'.format(meson.build_root(), meson.project_name()),
			tests
		],
		depends: tests
	)
endif
