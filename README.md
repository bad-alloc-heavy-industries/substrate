# substrate
[![GitHub Actions](https://github.com/bad-alloc-heavy-industries/substrate/workflows/GitHub%20Actions/badge.svg)](https://github.com/bad-alloc-heavy-industries/substrate/actions)
[![codecov](https://codecov.io/gh/bad-alloc-heavy-industries/substrate/branch/master/graph/badge.svg)](https://codecov.io/gh/bad-alloc-heavy-industries/substrate)
[![Coverity Status](https://scan.coverity.com/projects/20662/badge.svg)](https://scan.coverity.com/projects/substrate)
[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-v2.0%20adopted-ff69b4.svg)](code_of_conduct.md)

## Supported platforms, language versions, and compilers
<table>
    <thead>
        <tr>
            <th rowspan="3">Platform</th>
            <th rowspan="3">C++ Standard</th>
            <th colspan="12">Compiler</th>
        </tr>
        <tr>
            <th colspan="5" style="border-right: 1px solid !important;">G++</th>
            <th colspan="7" style="border-right: 1px solid !important;">Clang</th>
        </tr>
        <tr>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
            <th>14</th>
            <th>15</th>
        </tr>
    </thead>
    <tbody>
      <tr>
        <td rowspan="4">Linux</td>
        <td>C++ 11</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
      </tr>
      <tr>
        <td>C++ 14</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
      </tr>
      <tr>
        <td>C++ 17</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
      </tr>
      <tr>
        <td>C++ 20</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:heavy_check_mark:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:heavy_check_mark:</td>
      </tr>
      <tr>
        <td rowspan="3">NetBSD</td>
        <td>C++ 11</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:heavy_check_mark:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:x:</td>
      </tr>
      <tr>
        <td>C++ 14</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:heavy_check_mark:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:x:</td>
      </tr>
      <tr>
        <td>C++ 17</td>
        <td>:x:</td>
        <td>:grey_question:</td>
        <td>:heavy_check_mark:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:grey_question:</td>
        <td>:x:</td>
      </tr>
    </tbody>
</table>

<table>
    <thead>
        <tr>
            <th rowspan="3">Platform</th>
            <th rowspan="3">C++ Standard</th>
            <th colspan="16">Compiler</th>
        </tr>
        <tr>
            <th colspan="5" style="border-right: 1px solid !important;">G++</th>
            <th colspan="3">Apple Clang</th>
        </tr>
        <tr>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>10.15</th>
            <th>11.6</th>
        </tr>
    </thead>
    <tbody>
      <tr>
       <td rowspan="4">macOS</td>
        <td>C++ 11</td>
        <td>:heavy_check_mark: (1)</td>
        <td>:heavy_check_mark: (1)</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
      </tr>
      <tr>
        <td>C++ 14</td>
        <td>:heavy_check_mark: (1)</td>
        <td>:heavy_check_mark: (1)</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
      </tr>
      <tr>
        <td>C++ 17</td>
        <td>:heavy_check_mark: (1) (2)</td>
        <td>:heavy_check_mark: (1) (2)</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
      </tr>
      <tr>
        <td>C++ 20</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:heavy_check_mark:</td>
        <td>:x:</td>
        <td>:x:</td>
      </tr>
    </tbody>
</table>

(1): Programs built on Big Sur may crash on exception handle: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=98805
(2): Using `fd` constructors based on `std::filesystem::path` or the `command_line` module requires C++17, a minimum target OS of macOS 10.15, iOS/tvOS 13, or watchOS 6, and (if using GCC) GCC 9 or higher. The build system will detect this automatically.

<table>
    <thead>
        <tr>
            <th rowspan="3">Platform</th>
            <th rowspan="3">C++ Standard</th>
            <th colspan="6">Compiler</th>
        </tr>
        <tr>
            <th colspan="2" style="border-right: 1px solid !important;">MSYS G++</th>
            <th colspan="1" style="border-right: 1px solid !important;">MSYS Clang</th>
            <th colspan="3">MSVC</th>
        </tr>
        <tr>
            <th>mingw64</th>
            <th>ucrt64</th>
            <th>clang64</th>
            <th>16.11</th>
            <th>17.0</th>
        </tr>
    </thead>
    <tbody>
      <tr>
       <td rowspan="4">Windows</td>
        <td>C++ 11</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td colspan="2" style="text-align:center;">N/A</td>
      </tr>
      <tr>
        <td>C++ 14</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
      </tr>
      <tr>
        <td>C++ 17</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
      </tr>
      <tr>
        <td>C++ 20</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:x:</td>
        <td>:heavy_check_mark:</td>
      </tr>
    </tbody>
</table>

## Getting Started

There are two ways to use this library: installation and embedding.

### Installing substrate

To install substrate, it really is as simple as:

```bash
meson setup build
meson install -C build
```

Though, if you want to install to a specific location such as /usr, then you might want to specify `--prefix=/usr` to meson during the `meson setup` step.

### Embedding substrate

To embed substrate, add the project as a submodule to your project, then set up your build system
to view the 'substrate' folder in this repo as part of your compiler's include path.

Using meson this is acomplished by declaring a subproject dependency and grab the dependency with:

```meson
substrate = subproject(
  'substrate',
).get_variable(
  'substrate_dep'
)
```

To be included in your root meson.build. You would then specify `substrate` as a dependency of any library or
executable using the library.

### Variables defined by the Meson dependency object

The Meson dependency object defines the following variables you can access with `substrate.get_variable()`:

* `compile_args`: Compiler arguments required to build against the library if building custom compiler invocations
* `link_args`: Linker arguments required to build against the library if build custom linker invocations
* `command_line_enabled`: Set to 'true' if the command line parser is enabled in a build, 'false' otherwise

There are two dependency objects available from the Meson subproject. These are designed for cross-compilation, with
the 'substrate_dep' providing a dependency for the target system, and `substrateNative_dep` providing a dependency
for the build host system.
