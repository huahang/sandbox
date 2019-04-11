cc_library(
  name = 'json',
  srcs = [
    'arraylist.c',
    'debug.c',
    'json_c_version.c',
    'json_object.c',
    'json_object_iterator.c',
    'json_tokener.c',
    'json_util.c',
    'libjson.c',
    'linkhash.c',
    'printbuf.c',
    'random_seed.c',
  ],
  extra_cppflags = [
    '-Wwrite-strings',
    '-Wno-unused-parameter',
    '-Wno-float-equal',
    '-Wno-implicit-function-declaration',
    '-std=gnu99',
  ],
  defs = [
    '_GNU_SOURCE',
    'HAVE_CONFIG_H',
    '_REENTRANT',
    'VALGRIND',
  ]
)
