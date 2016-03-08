import json
import urllib2

class Translator:
    key = "trnsl.1.1.20160308T164451Z.a23210e830483171.38004e90453fd753b4b2ab587b699cac0ee7dae3"
    api_url = "https://translate.yandex.net/api/v1.5/tr.json/translate"

    def translate_request(self, request):
      try:
        url = self.api_url + "?key=" + self.key + "&text=" + request + "&lang=en-fr"
        tr = urllib2.urlopen(url).read()
        js = json.loads(tr)
        tr = js["text"][0]
        return tr
      except:
        return "Failed"

    def default_request(self, request):
      return "Not supported"
