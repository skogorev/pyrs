#ifndef PYRSHTTP_H
#define PYRSHTTP_H

#include <stdbool.h>
#include <stdio.h>
#include "pyrs_config.h"
#include "pyrs_py_module.h"

bool pyrs_http_srv_init(pyrs_config* conf);
bool pyrs_http_srv_add_module(pyrs_py_module* module);
bool pyrs_http_srv_run_loop();
void pyrs_http_srv_release();

#endif // PYRSHTTP_H
