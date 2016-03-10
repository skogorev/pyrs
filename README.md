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

[haywire benchmark test](https://github.com/haywire/haywire/tree/a82784b9c2cf2a4b71157697315a28535c873abc/benchmark) **without using python handlers**
```
$ wrk --script ./pipelined_get.lua --latency -d 30s -t 8 -c 32 http://127.0.0.1:8094/test -- 64
Running 30s test @ http://127.0.0.1:8094/test
  8 threads and 32 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     9.68ms    4.96ms  57.02ms   40.08%
    Req/Sec    12.28k     1.01k   15.52k    77.25%
  Latency Distribution
     50%   16.69ms
     75%    0.00us
     90%    0.00us
     99%    0.00us
  2939137 requests in 30.09s, 484.92MB read
Requests/sec:  97672.17
Transfer/sec:     16.11MB

```
test ***pyrs server*** with [echo python handler](https://github.com/skogorev/pyrs/tree/master/examples/echo)
```
$ wrk --script ./pipelined_get.lua --latency -d 30s -t 8 -c 32 http://127.0.0.1:8094/test -- 64
Running 30s test @ http://127.0.0.1:8094/test
  8 threads and 32 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    18.17ms    9.88ms  76.32ms   55.87%
    Req/Sec     7.28k     1.21k   16.23k    75.95%
  Latency Distribution
     50%   19.23ms
     75%   28.24ms
     90%   34.23ms
     99%    0.00us
  1740258 requests in 30.08s, 287.12MB read
Requests/sec:  57848.00
Transfer/sec:      9.54MB
```

## 3rdparty
* [haywire](https://github.com/haywire/haywire)
* [json-parser](https://github.com/udp/json-parser)
* [vc_vector](https://github.com/skogorev/vc_vector)

## License

[Apache License](LICENSE.txt)
