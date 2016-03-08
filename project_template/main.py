class Api:
    def __init__(self):
      print "Api module is initialized"

    def echo_request(self, request):
      print "'echo' module called"
      return request

    def hello_request(self, request):
      print "'hello' module called"
      return "world"

    def default_request(self, request):
      print "'default' module called"
      return ""
