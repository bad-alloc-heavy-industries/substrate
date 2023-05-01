# substrate
[![GitHub Actions](https://github.com/bad-alloc-heavy-industries/substrate/workflows/GitHub%20Actions/badge.svg)](https://github.com/bad-alloc-heavy-industries/substrate/actions)
[![codecov](https://codecov.io/gh/bad-alloc-heavy-industries/substrate/branch/master/graph/badge.svg)](https://codecov.io/gh/bad-alloc-heavy-industries/substrate)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/bad-alloc-heavy-industries/substrate.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/bad-alloc-heavy-industries/substrate/alerts/)
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
```
meson build
ninja -C build install
```

Though, if you want to install to a specific location such as /usr, then you might want to specify `--prefix=/usr` to meson.

### Embedding substrate

To embed substrate, add the project as a submodule to your project, then set up your build system
to view the 'substrate' folder in this repo as part of your compiler's include path.

Using meson this is acomplished by selecting this directory using `include_directories()`
If substrate is in your repo under deps/substrate, that looks like:
`substrate = include_directories('deps/substrate')`

To be included in your root meson.build. You would then specify `substrate` as part of any library or executable include_directories specification.
