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

if cxx.get_define('__cplusplus').substring(0, -1).version_compare('>=201703')
	std_variant_test = '''
		#include <variant>
		#include <vector>
		int main(int argc, char* argv[])
		{
			using tuple_t = std::variant<int, double>;
			std::vector<tuple_t> list;
			list.emplace_back(argc - 1);

			return std::get<int>(v.begin());
		}
	'''
	if cxx.compiles(std_variant_test, name: 'has a working std::variant implementation (GCC #90397, LLVM #41863)')
		subdir('command_line')
	endif
endif

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
