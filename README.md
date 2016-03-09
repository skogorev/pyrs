# pyrs
Asynchronous HTTP server (written in C) with Python handlers.

### Project examples

- [Echo server](https://github.com/skogorev/pyrs/tree/master/examples/echo)
- [Yandex En-Fr translator server](https://github.com/skogorev/pyrs/tree/master/examples/en_fr_translator)

## Building

### Linux and OSX
```
./build_all_and_install.sh
```

### Windows
Coming soon

#### Requirements
* cmake
* python2.7

## Project creation
[template example](https://github.com/skogorev/pyrs/tree/master/project_template)

The structure of the pyrs project:
```
- ProjectDir
  - main.py
  - project.pyrs
```

`project.pyrs` example:
```
{
  "http_listen_address": "127.0.0.1", /* [optional]. Default is "0.0.0.0" */
  "http_listen_port": 8000, /* [optional]. Default is "8094" */
  "thread_count": 2, /* [optional]. Default is "2" */
  "tcp_nodelay": false, /* [optional]. Default is "false" */
  "listen_backlog": 1, /* [optional]. The maximum length to which the queue of pending may grow. Default is "10" */

  "modules": [ /* Python handlers */

    {
      "path": "/echo", /* handlers for http://url:port/echo */
      "module": "main", /* name of python module */
      "class": "Api", /* class name */
      "handler": "echo_request" /* handler method name */
    },

    {
      "path": "/*",
      "module": "main",
      "class": "Api",
      "handler": "default_request"
    }

    ...

  ]
}

```

The project directory should contain the main Python file. As an example:
```python
class Api:
    def __init__(self):
      print "Api module is initialized" # The server creates an instance of class for each module

    def echo_request(self, request): # Called when a user requests http://url:port/echo
      print "'echo' module called"
      return request

    def hello_request(self, request): # call when user gets http://url:port/hello
      print "'hello' module called"
      return "world"

    def default_request(self, request):  # otherwise
      print "'default' module called"
      return ""
```

## Running:

To run the pyrs project:
```
$ pyrs project.pyrs 
Starting pyrs server...
Api module is initialized
Added route /echo
Api module is initialized
Added route /hello
Api module is initialized
Added route /*
Address: 0.0.0.0
Port: 8094
Listening...
```

Example of a request to the server from the client:
```
$ curl -X POST -d "hello" http://127.0.0.1:8094/echo
hello
```

## Benchmarks
OSX. 3 GHz Intel Core i7. 8 GB 1600 MHz DDR3

[haywire benchmark test](https://github.com/haywire/haywire/tree/a82784b9c2cf2a4b71157697315a28535c873abc/benchmark) ***without using python handlers***
```
$ wrk --script ./pipelined_get.lua --latency -d 30s -t 8 -c 32 http://127.0.0.1:8094 -- 64
Running 30s test @ http://127.0.0.1:8094
  8 threads and 32 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     9.21ms    6.96ms  72.08ms   67.88%
    Req/Sec    24.26k    28.92k  121.92k    88.18%
  Latency Distribution
     50%    8.95ms
     75%   14.15ms
     90%   20.72ms
     99%    0.00us
  5783416 requests in 30.08s, 0.88GB read
  Non-2xx or 3xx responses: 5783416
Requests/sec: 192282.04
Transfer/sec:     29.89MB
```
test ***pyrs server*** with [echo python handler](https://github.com/skogorev/pyrs/tree/master/examples/echo)
```
$ wrk --script ./pipelined_get.lua --latency -d 30s -t 8 -c 32 http://127.0.0.1:8094 -- 64
Running 30s test @ http://127.0.0.1:8094
  8 threads and 32 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     7.79ms    6.21ms  56.99ms   72.48%
    Req/Sec    23.37k    15.39k   99.84k    76.47%
  Latency Distribution
     50%    6.97ms
     75%   12.14ms
     90%   18.72ms
     99%    0.00us
  5586251 requests in 30.09s, 868.38MB read
  Non-2xx or 3xx responses: 5586251
Requests/sec: 185625.24
Transfer/sec:     28.86MB
```

## 3rdparty
* [haywire](https://github.com/haywire/haywire)
* [json-parser](https://github.com/udp/json-parser)
* [vc_vector](https://github.com/skogorev/vc_vector)

## License

[Apache License](LICENSE.md)
