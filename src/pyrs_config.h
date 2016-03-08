#ifndef PYRSCONFIG_H
#define PYRSCONFIG_H

#include <stdbool.h>

#define PY_ALL_CONFIG_OPTIONS \
  XX(http_listen_address, char*,                strdup("0.0.0.0"),      json_string,    assign_string_json) \
  XX(http_listen_port,    unsigned int,         8000,                   json_integer,   assign_uint_json) \
  XX(thread_count,        unsigned int,         2,                      json_integer,   assign_uint_json) \
  XX(parser,              char*,                strdup("http_parser"),  json_string,    assign_string_json) \
  XX(tcp_nodelay,         bool,                 false,                  json_boolean,   assign_bool_json) \
  XX(listen_backlog,      unsigned int,         10,                     json_integer,   assign_uint_json) \
  XX(modules,             pyrs_config_modules,  none_modules(),         json_array,     assign_modules_json)

#define PY_ALL_CONFIG_MODULE_OPTIONS \
  XX(path) \
  XX(module) \
  XX(class) \
  XX(handler)

typedef struct {
#define XX(n) char* n;
  PY_ALL_CONFIG_MODULE_OPTIONS
#undef XX
} pyrs_config_module;

typedef struct {
  pyrs_config_module* data;
  int count;
} pyrs_config_modules;

typedef struct {
#define XX(o, t, d, jt, g) t o;
  PY_ALL_CONFIG_OPTIONS
#undef XX
} pyrs_config;

pyrs_config* pyrs_config_create();
bool pyrs_config_load_from_file(pyrs_config* conf, const char* path);
void pyrs_config_release(pyrs_config* conf);

#endif // PYRSCONFIG_H
