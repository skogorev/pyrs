#include "pyrs_py_module.h"

void pyrs_py_module_global_init(const char* project_dir) {
  setenv("PYTHONPATH", project_dir, 0);
  Py_Initialize();
}

void pyrs_py_module_global_release() {
  Py_Finalize();
}

pyrs_py_module* pyrs_py_module_create(pyrs_config_module* conf) {
  pyrs_py_module* m = (pyrs_py_module*)malloc(sizeof(pyrs_py_module));
  if (!m) {
    return m;
  }
  
  m->py_m = PyImport_ImportModule(conf->module);
  if (m->py_m == NULL) {
    fprintf(stderr, "Failed importing python module %s\n", conf->module);
    free(m);
    return false;
  }
  
  m->py_c = PyObject_GetAttrString(m->py_m, conf->class);
  if (m->py_c == NULL) {
    fprintf(stderr, "Failed importing python module class %s\n", conf->class);
    Py_DECREF(m->py_m);
    free(m);
    return false;
  }
  
  m->py_instance = PyInstance_New(m->py_c, NULL, NULL);
  if (m->py_instance == NULL) {
    fprintf(stderr, "Failed creating class instance %s\n", conf->class);
    Py_DECREF(m->py_c);
    Py_DECREF(m->py_m);
    free(m);
    return false;
  }
  
  m->handler_name = strdup(conf->handler);
  m->path = strdup(conf->path);
  return m;
}

pyrs_py_res pyrs_py_module_call(pyrs_py_module* module, char* request) {
  pyrs_py_res res;
  res.data = NULL;
  res.len = 0;
  
  res.py_res = PyObject_CallMethod(module->py_instance, module->handler_name, "s", request ? request : "");
  if (res.py_res) {
    res.data = PyBytes_AsString(res.py_res);
    if (res.data) {
      res.len = strlen(res.data);
    }
  }
  
  return res;
}

void pyrs_py_res_release(pyrs_py_res* res) {
  Py_DECREF(res->py_res);
}

void pyrs_py_module_release(pyrs_py_module* module) {
  free(module->handler_name);
  free(module->path);
  Py_DECREF(module->py_instance);
  Py_DECREF(module->py_c);
  Py_DECREF(module->py_m);
  Py_DECREF(module->py_m);
  free(module);
}
