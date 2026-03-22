project(
  'ted',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++20'],
)

dependencies = [
  dependency('raylib')
]

sources = [
  'ted.cpp',
]

exe = executable(
  'ted',
  sources,
  install : true,
  dependencies : dependencies,
)

test('basic', exe)
