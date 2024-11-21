import http.server
import json

class handle_request(http.server.BaseHTTPRequestHandler):
    def _set_headers(self):
        self.send_response(200)
        self.send_header("Content-type", "application/json")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()

    def do_GET(self):
        response = {
            "__proto__": {  
            "report": True
          }
        }
        self._set_headers()
        self.wfile.write(json.dumps(response).encode())

server = http.server.HTTPServer(('', 9000), handle_request)
server.serve_forever()
