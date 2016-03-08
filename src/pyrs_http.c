#include "pyrs_http.h"
#include <string.h>
#include <haywire.h>
#include "vc_vector.h"

vc_vector* g_modules = NULL;

void response_complete(void* user_data) {
  // Releasing route handler
}

void handle(http_request* request, hw_http_response* response, void* user_data) {
  hw_string status_code;
  hw_string content_type_name;
  hw_string content_type_value;
  hw_string body;
  hw_string keep_alive_name;
  hw_string keep_alive_value;
  hw_string route_matched_name;
  hw_string route_matched_value;
  pyrs_py_module* module = (pyrs_py_module*)user_data;
  
  SETSTRING(status_code, HTTP_STATUS_200);
  hw_set_response_status_code(response, &status_code);
  SETSTRING(content_type_name, "Content-Type");
  SETSTRING(content_type_value, "text/html");
  hw_set_response_header(response, &content_type_name, &content_type_value);
  
  if (request->keep_alive) {
    SETSTRING(keep_alive_name, "Connection");
    SETSTRING(keep_alive_value, "Keep-Alive");
    hw_set_response_header(response, &keep_alive_name, &keep_alive_value);
  } else {
    hw_set_http_version(response, 1, 0);
  }
  
  SETSTRING(route_matched_name, "Route-Matched");
  route_matched_value.value = module->path;
  route_matched_value.length = strlen(module->path);
  hw_set_response_header(response, &route_matched_name, &route_matched_value);
  
  pyrs_py_res res = pyrs_py_module_call(module, request->body->value);
  body.value = res.data;
  body.length = res.len;
  hw_set_body(response, &body);
  hw_http_response_send(response, module, response_complete);
  pyrs_py_res_release(&res);
}

bool pyrs_http_srv_init(pyrs_config* conf) {
  configuration hw_config;
  hw_config.http_listen_address = conf->http_listen_address;
  hw_config.http_listen_port = conf->http_listen_port;
  hw_config.thread_count = conf->thread_count;
  hw_config.parser = conf->parser;
  hw_config.tcp_nodelay = conf->tcp_nodelay;
  hw_config.listen_backlog = conf->listen_backlog;
  if (hw_init_with_config(&hw_config) != 0) {
    return false;
  }
  
  if (!g_modules) {
    g_modules = vc_vector_create(0, sizeof(pyrs_py_module*), (vc_vector_deleter*)&pyrs_py_module_release);
    if (!g_modules) {
      return false;
    }
  }
  
  return true;
}

bool pyrs_http_srv_add_module(pyrs_py_module* module) {
  if (!vc_vector_push_back(g_modules, module)) {
    return false;
  }
  
  hw_http_add_route(module->path, handle, module);
  return true;
}

bool pyrs_http_srv_run_loop() {
  return hw_http_open() == 0;
}

void pyrs_http_srv_release() {
  if (g_modules != NULL) {
    vc_vector_release(g_modules);
  }
}
