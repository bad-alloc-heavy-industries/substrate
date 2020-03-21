# substrate

[![GitHub Actions](https://github.com/bad-alloc-heavy-industries/substrate/workflows/GitHub%20Actions/badge.svg)](https://github.com/bad-alloc-heavy-industries/substrate/actions)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/bad-alloc-heavy-industries/substrate.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/bad-alloc-heavy-industries/substrate/alerts/)

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
