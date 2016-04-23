import BaseHTTPServer

class request_handler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-Type", "text/html")
        self.end_headers()
        self.wfile.write("let me guess who this is...<br />")
        client = self.client_address[0]
        self.wfile.write("Your ip: "+client)
        if client[len(client)-1] == '2':
            self.wfile.write("<br /><h3>You are the server admin: Don</h3>")

if __name__ == '__main__':
    httpd = BaseHTTPServer.HTTPServer(('', 8080), request_handler)
    try:
        print "Activating Server..."
        httpd.serve_forever()
        print "Server Activated"
    except KeyboardInterrupt:
        httpd.server_close()
        print "Server Shut Down"
