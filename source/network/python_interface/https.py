'''
Socketserver Https Threading Server with SSL security,favicon and Homepage.
Everything here is implemented as long as concerns Servers.
Need to employ Database to it!
'''

from http.server import BaseHTTPRequestHandler
from socketserver import ThreadingTCPServer
import concurrent.futures
import socket
import ssl
import cgi
import cgitb
cgitb.enable()

PORT = 443
IP = socket.gethostbyname(socket.gethostname())
servername = socket.gethostname()
print('https://{}:{}'.format(IP, PORT))

class CustomHandler(BaseHTTPRequestHandler):

    tasklist = ['Task 1', 'Task 2', 'Task 3']
    
    def do_GET(self):
        
        if self.path.endswith('/'):
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Encoding', 'UTF-8')
            self.end_headers()
            
            homepage = open('c:\\users\\antonios\\desktop\\index.html', 'rb').read()
            self.wfile.write(bytes(homepage))
            
        if self.path.endswith('/exampleicon.ico'):
            self.send_response(200)
            self.send_header('Content-Type:', 'image/x-icon')
            self.send_header('Content-Length', 77494)
            self.send_header('Accept', 'bytes')
            self.end_headers()
            
            exampleicon = open('c:\\users\\antonios\\desktop\\exampleicon.ico', 'rb').read()
            self.wfile.write(bytes(exampleicon))
        
        if self.path.endswith('/favicon.ico'):
            self.send_response(200)
            self.send_header('Content-Type:', 'image/x-icon')
            self.send_header('Content-Length', 17542)
            self.send_header('Accept', 'bytes')
            self.end_headers()

            favicon = open('c:\\users\\antonios\\desktop\\favicon.ico', 'rb').read()
            self.wfile.write(bytes(favicon))
        
        if self.path.endswith('/tasklist'):
            self.send_response(200)
            self.send_header('Content-Type:', 'text/html')
            self.end_headers()
            
            output = ''
            output += '<html>\r\n'
            output += '<head>\r\n'
            output += '<meta charset="UTF-8">'
            output += '</head>\r\n\r\n'
            output += '<body>\r\n'
            output += '<h1>Task List</h1>'
            output += '<h3><a href="/tasklist/new">Add New Task</a></h3>'
            for task in self.tasklist:
                output += task
                output += '<a/ href="/tasklist/{}/remove">X</a>'.format(task)
                output += '</br>'
            output += '<hr/>'
            output += '<h3><a href="/">Cancel</a></h3>'
            output += '</body>\r\n'
            output += '</html>'
            self.wfile.write(output.encode())
            
        if self.path.endswith('/new'):
            self.send_response(200)
            self.send_header('Content-Type:', 'text/html')
            self.end_headers() 
            
            output = ''
            output += '<html>\r\n'
            output += '<head>\r\n'
            output += '<meta charset="UTF-8">'
            output += '</head>\r\n\r\n'
            output += '<body>\r\n'
            output += '<h1>Add New Task</h1>'
            output += '<form method="POST" enctype="multipart/form-data" action="/tasklist/new">'
            output += '<input name="Task" type="text" placeholder="Add New Task">'
            output += '<input type="submit" value="Add">'
            output += '</form>\r\n'
            output += '</body>\r\n'
            output += '</html>'
            self.wfile.write(output.encode())
            
        if self.path.endswith('/remove'):
            listIDpath = self.path.split('/')[2]
            self.send_response(200)
            self.send_header('Content-Type:', 'text/html')
            self.end_headers()
            
            output = ''
            output += '<html>\r\n'
            output += '<head>\r\n'
            output += '<meta charset="UTF-8">'
            output += '</head>\r\n\r\n'
            output += '<body>\r\n'
            output += '<h1>Remove Task: {}</h1>'.format(listIDpath.replace('%20', ' '))
            output += '<form method="POST" enctype="multipart/form-data" action="/tasklist/{}/remove">'.format(listIDpath)
            output += '<input type="Submit" value="Remove"></form>\r\n'
            output += '<a href="/tasklist">Cancel</a>'
            output += '</body>\r\n'
            output += '</html>'
            self.wfile.write(output.encode())            
    
    def do_POST(self):
        if self.path.endswith('/new'):
            ctype, pdict = cgi.parse_header(self.headers.get('Content-Type'))
            pdict['boundary'] = bytes(pdict['boundary'], 'utf-8')
            content_len = int(self.headers.get('Content-length'))
            pdict['CONTENT-LENGTH'] = content_len
            if ctype == 'multipart/form-data':
                fields = cgi.parse_multipart(self.rfile, pdict)
                new_task = fields.get('Task')
                self.tasklist.append(new_task[0])
            
            self.send_response(301)
            self.send_header('Content-Type:', 'text/html')
            self.send_header('Location', '/tasklist')
            self.end_headers()
            
        if self.path.endswith('/remove'):
            listIDpath = self.path.split('/')[2]
            ctype, pdict = cgi.parse_header(self.headers.get('Content-Type'))
            if ctype == 'multipart/form-data':
                list_item = listIDpath.replace('%20', ' ')
                self.tasklist.remove(list_item)
                
                self.send_response(301)
                self.send_header('Content-Type:', 'text/html')
                self.send_header('Location', '/tasklist')
                self.end_headers()

def main():
    SERVER = ThreadingTCPServer((IP, PORT), CustomHandler)
    SERVER.socket = ssl.wrap_socket(SERVER.socket, certfile='c:\\users\\antonios\\desktop\\ascert.pem',
                       keyfile='c:\\users\\Antonios\\Desktop\\askey.pem', server_side=True)
    print('Server Running on {}:{}'.format(IP, PORT))
    while True:
        with concurrent.futures.ThreadPoolExecutor() as executor:
            executor.submit(SERVER.serve_forever)

if __name__ == '__main__':
    main()

