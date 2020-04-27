# substrate
[![GitHub Actions](https://github.com/bad-alloc-heavy-industries/substrate/workflows/GitHub%20Actions/badge.svg)](https://github.com/bad-alloc-heavy-industries/substrate/actions) [![codecov](https://codecov.io/gh/bad-alloc-heavy-industries/substrate/branch/master/graph/badge.svg)](https://codecov.io/gh/bad-alloc-heavy-industries/substrate) [![Total alerts](https://img.shields.io/lgtm/alerts/g/bad-alloc-heavy-industries/substrate.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/bad-alloc-heavy-industries/substrate/alerts/) [![Coverity Status](https://scan.coverity.com/projects/20662/badge.svg)](https://scan.coverity.com/projects/substrate)

## Supported platforms, language versions, and compilers
<table>
    <thead>
        <tr>
            <th rowspan="3">Platform</th>
            <th rowspan="3">C++ Standard</th>
            <th colspan="11">Compiler</th>
        </tr>
        <tr>
            <th colspan="5">G++</th>
            <th colspan="5">Clang</th>
            <th colspan="1">MSVC</th>
        </tr>
        <tr>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>14.11</th>
        </tr>
    </thead>
    <tbody>
      <tr>
        <td rowspan="3">Linux</td>
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
        <td>:x:</td>
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
        <td>:x:</td>
      </tr>
      <tr>
        <td>C++ 17</td>
        <td>:x:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:heavy_check_mark:</td>
        <td>:x:</td>
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
        <td>:x:</td>
      </tr>
      <tr>
       <td rowspan="3">Windows</td>
        <td>C++ 11</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:grey_question:</td>
      </tr>
      <tr>
        <td>C++ 14</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:grey_question:</td>
      </tr>
      <tr>
        <td>C++ 17</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:x:</td>
        <td>:grey_question:</td>
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
