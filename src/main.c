#include <libgen.h>
#include "pyrs_http.h"
#include "pyrs_py_module.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: \"%s CONFIG_FILE\"\n", basename(argv[0]));
    return 1;
  }
  
  fprintf(stdout, "Starting pyrs server...\n");

  pyrs_config* config = pyrs_config_create();
  if (!config) {
    fprintf(stderr, "Error creating configuration file instance\n");
    return 1;
  }

  if (!pyrs_config_load_from_file(config, argv[1])) {
    fprintf(stderr, "Error loading configuration file\n");
    pyrs_config_release(config);
    return 1;
  }
  
  if (!pyrs_http_srv_init(config)) {
    fprintf(stderr, "HTTP server initialization error\n");
    pyrs_config_release(config);
    return 1;
  }
  
  pyrs_py_module_global_init();
  
  for (int i = 0; i < config->modules.count; ++i) {
    pyrs_py_module* module = pyrs_py_module_create(config->modules.data + i);
    if (!module) {
      fprintf(stderr, "Error creating module %u\n", i);
      continue;
    }
    
    if (!pyrs_http_srv_add_module(module)) {
      fprintf(stderr, "Error adding module %u to HTTP server\n", i);
      pyrs_py_module_release(module);
      continue;
    }
  }
  
  pyrs_config_release(config);
  
  if (!pyrs_http_srv_run_loop()) {
    fprintf(stderr, "Error running HTTP server loop\n");
    pyrs_http_srv_release();
    pyrs_py_module_global_release();
    return 1;
  }
  
  pyrs_http_srv_release();
  pyrs_py_module_global_release();
  return 0;
}
