doxygen = find_program('doxygen', required: false)
if doxygen.found()
	dot = find_program('dot', required: false)
	doxydat = configuration_data()
	doxydat.set('VERSION', meson.project_version())
	# If we have dot, we will generate class diagrams
	if dot.found()
		doxydat.set('USE_DOT', 'YES')
	else
		doxydat.set('USE_DOT', 'NO')
	endif

	doxydat.set('INPUT_FILES', '../substrate/' + ' ../substrate/'.join(headers + advanced_headers))

	doxyfile = configure_file(
		input: 'Doxyfile.in',
		output: 'Doxyfile',
		configuration: doxydat,
		install: false
	)

	custom_target(
		'doc',
		input: doxyfile,
		output: [ 'html'],
		command: [ doxygen, doxyfile ],
		install: false,
		build_by_default: true
	)
else
	warning('Can not find Doxygen, not generating documentation as requested')
endif
