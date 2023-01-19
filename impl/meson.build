# SPDX-License-Identifier: BSD-3-Clause
libSubstrateSrcs = [
	'socket.cxx', 'console.cxx'
]

deps = []

if target_machine.system() == 'windows'
	deps += cxx.find_library('ws2_32')
else
	libSubstrateSrcs += 'pty.cxx'
endif

subdir('command_line')

libSubstrate = library(
	'substrate',
	libSubstrateSrcs,
	c_args: ['-DSUBSTRATE_BUILD_INTERNAL'],
	cpp_args: ['-DSUBSTRATE_BUILD_INTERNAL'],
	include_directories: include_directories('..'),
	dependencies: deps,
	gnu_symbol_visibility: 'inlineshidden',
	implicit_include_directories: false,
	pic: true,
	install: (not meson.is_subproject()),
)
