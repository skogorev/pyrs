# pyrs
Asynchronous HTTP server (written in C) with Python handlers
Асинхронный HTTP сервер на C с подключаемыми Python-обработчиками

## Building

## Linux and OSX
```
./build_all_and_install.sh
```

## Windows
Coming soon

## Project creation

Creating new pyrs project ([template example](https://github.com/skogorev/pyrs/tree/master/ProjectTemplate))
```
- ProjectDir
  - main.py
  - project.pyrs
```

project.pyrs example:
```
{
	"http_listen_address": "127.0.0.1", // [optional]. Default is "0.0.0.0"
	"http_listen_port": 8000, // [optional]. Default is "8094"
	"thread_count": 2, // [optional]. Default is "2"
	"tcp_nodelay": false, // [optional]. Default is "false"
	"listen_backlog": 1, // [optional]. The maximum length to which the queue of pending may grow. Default is "10"

	"modules": [ // Python handlers

		{
			"path": "/echo", // handlers for http://url:port/echo
			"module": "main", // name of python module
			"class": "Api", // class name
			"handler": "echo_request" // handler method name
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

Project directory should contains 'main' python file. How example:

```
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

###Running:

To run project:
```
$pyrs ProjectDir/pyrs.project
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

Example of request to server from client:
```
$curl -X POST -d "hello" http://127.0.0.1:8094/echo
hello
```

## 3rdparty
[haywire](https://github.com/haywire/haywire)
[json-parser](https://github.com/udp/json-parser)
[vc_vector](https://github.com/skogorev/vc_vector)

## License

[Apache License](LICENSE.md)
