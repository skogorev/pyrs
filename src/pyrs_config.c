#include "pyrs_config.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "json.h"
#include <assert.h>

#define LIMIT_CONFIG_SIZE (3 * 1024 * 1024)

pyrs_config_modules none_modules() {
  pyrs_config_modules modules;
  modules.data = NULL;
  modules.count = 0;
  return modules;
}

// getters

void assign_string_json(char** str, json_value* value) {
  assert(value->type == json_string);
  if (*str) free(*str);
  *str = strdup(value->u.string.ptr);
}

void assign_uint_json(unsigned int* i, json_value* value) {
  assert(value->type == json_integer);
  *i = (unsigned int)value->u.integer;
}

void assign_bool_json(bool* i, json_value* value) {
  assert(value->type == json_boolean);
  *i = value->u.boolean;
}

void assign_modules_json(pyrs_config_modules* modules, json_value* value) {
  assert(value->type == json_array);
  
  if (modules->count > 0) {
    return;
  }
  
  modules->count = value->u.array.length;
  if (modules->count == 0) {
    return;
  }
  
  modules->data = (pyrs_config_module*)calloc(modules->count, sizeof(pyrs_config_module));
  if (!modules->data) {
    return;
  }
  
  for (int i = 0; i < modules->count; ++i) {
    json_value* json_module = value->u.array.values[i];
    if (json_module->type == json_object) {
      for (unsigned int j = 0; j < json_module->u.object.length; ++j) {
        json_object_entry* json_option = json_module->u.object.values + j;
        if (json_option->value->type != json_string) {
          continue;
        }
#define XX(n) else if (strcmp(#n, json_option->name) == 0 && json_option->value->type == json_string) {\
                assign_string_json(&modules->data[i].n, json_option->value); \
              }
        PY_ALL_CONFIG_MODULE_OPTIONS
#undef XX
      }
    }
  }
}

pyrs_config* pyrs_config_create() {
  pyrs_config* conf = (pyrs_config*)malloc(sizeof(pyrs_config));
  if (!conf) {
    return conf;
  }
  
  // Setup default values for all config options
#define XX(o, t, d, jt, g) conf->o = d;
  PY_ALL_CONFIG_OPTIONS
#undef XX
  
  return conf;
}

bool pyrs_config_load_from_json(pyrs_config* conf, const json_value* value)  {
  if (value->type != json_object) {
    return false;
  }
  
  int length = value->u.object.length;
  for (int i = 0; i < length; ++i) {
    json_object_entry* entry = value->u.object.values + i;
    
    if (false) {}
#define XX(o, t, d, jt, g)  else if (strcmp(#o, entry->name) == 0 && entry->value->type == jt) { \
                              g(&conf->o, entry->value); \
                            }

    PY_ALL_CONFIG_OPTIONS
#undef XX
  }
  
  // Checking modules
  
  if (conf->modules.count == 0) {
    fprintf(stderr, "Not found any route module\n");
    return false;
  }
  for (int i = 0; i < conf->modules.count; ++i) {
    pyrs_config_module* module = conf->modules.data + i;
#define XX(n) if (!module->n) return false;
    PY_ALL_CONFIG_MODULE_OPTIONS
#undef XX
  }
  
  return true;
}

bool pyrs_config_load_from_file(pyrs_config* conf, const char* path) {
  struct stat f_info;
  
  if (stat(path, &f_info) != 0) {
    perror("Error retrieving information about a configuration file");
    return false;
  }
  
  if (f_info.st_size > LIMIT_CONFIG_SIZE) {
    fprintf(stderr, "Too large configuration file. Limit: %d\n", LIMIT_CONFIG_SIZE);
    return false;
  }
  
  FILE* f = fopen(path, "rt");
  if (f == NULL) {
    perror("Error opening config file");
    return false;
  }
  
  char* f_content = (char*)malloc(f_info.st_size);
  if (f_content == NULL) {
    fclose(f);
    return false;
  }
  
  if (fread(f_content, f_info.st_size, sizeof f_content[0], f) != 1 ) {
    fprintf(stderr, "Error allocating file content\n");
    fclose(f);
    free(f_content);
    return false;
  }
  
  fclose(f);
  
  json_char* json = (json_char*)f_content;
  json_value* value = json_parse(json, f_info.st_size);
  if (value == NULL) {
    fprintf(stderr, "Error parsing JSON from a configuration file\n");
    free(f_content);
    return false;
  }
  
  bool result = pyrs_config_load_from_json(conf, value);
  if (!result) {
    fprintf(stderr, "Wrong configuration file\n");
  }
  
  free(f_content);
  json_value_free(value);
  
  return result;
}

void pyrs_config_modules_release(pyrs_config_modules* modules) {
  if (modules->count) {
    for (int i = 0; i < modules->count; ++i) {
      pyrs_config_module* module = modules->data + i;
      
#define XX(n) if (module->n) free(module->n);
      PY_ALL_CONFIG_MODULE_OPTIONS
#undef XX
    }
    
    free(modules->data);
  }
}

void pyrs_config_release(pyrs_config* conf) {
  free(conf->http_listen_address);
  free(conf->parser);
  pyrs_config_modules_release(&conf->modules);
  free(conf);
}
