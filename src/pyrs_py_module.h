#ifndef PYRSPYMODULE_H
#define PYRSPYMODULE_H

#include <stdbool.h>
#include <stdio.h>
#include <Python/Python.h>
#include "pyrs_config.h"

typedef struct {
  PyObject* py_res;
  char* data;
  size_t len;
} pyrs_py_res;

typedef struct {
  char* path;
  char* handler_name;
  
  PyObject* py_m;
  PyObject* py_c;
  PyObject* py_instance;
} pyrs_py_module;

void pyrs_py_module_global_init();
void pyrs_py_module_global_release();

pyrs_py_module* pyrs_py_module_create(pyrs_config_module* conf);
pyrs_py_res pyrs_py_module_call(pyrs_py_module* module, char* request); // Safe multi-th
void pyrs_py_res_release(pyrs_py_res* res);
void pyrs_py_module_release(pyrs_py_module* module);

#endif // PYRSPYMODULE_H
