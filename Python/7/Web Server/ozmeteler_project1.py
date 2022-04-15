# -*- coding: utf-8 -*-
"""
Created on Sun Mar 17 13:10:00 2019

@author: Mehmet Batu Ozmeteler

This pile of code runs with Python 2.7 and implements a multi-threaded HTTP Server.
"""

#!/usr/bin/env python3
import sys, socket
from SocketServer import ThreadingMixIn
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer

class ThreadingSimpleServer(ThreadingMixIn, HTTPServer):
    pass


class myHandler(BaseHTTPRequestHandler):
	
	#Handler for the GET requests
    def do_GET(self):
        if self.path == "/":
            self.path = "/index.html"
        elif self.path.endswith(".jpg"):
            self.path = self.path;
        else:
            self.path = self.path + "/index.html"
    
        try:
            sendReply = False
            if self.path.endswith(".html"):
                mimetype = 'text/html'
                sendReply = True
            if self.path.endswith(".jpg"):
                mimetype = 'image/jpg'
                sendReply = True
            if self.path.endswith(".gif"):
				mimetype = 'image/gif'
				sendReply = True
                
            if sendReply == True:
                f = open(documentRoot + self.path)
                self.send_response(200)
                self.send_header('Content-type', mimetype)
                self.end_headers()
                self.log_message()
                self.wfile.write(f.read())
                f.close()  
            return
        except IOError:
            print "File not found, shutting down the web server"
            server.socket.close()
    
    def log_request(self, format, *args):
        pass
    
    def log_message(self):
        date_time = self.log_date_time_string()
        address_string = self.client_address
        print (date_time, self.command, "/" + self.path, address_string) 

try:
    portNumber = int(input("Port Number: "))
    documentRoot = str(raw_input("Document Root: "))
    print("\n")
    hostName = socket.gethostname()
    
    server = ThreadingSimpleServer(("", portNumber), myHandler)
    print "Serving HTTP traffic from", documentRoot, "on", hostName, "using port", portNumber
    while True:
        sys.stdout.flush()
        server.handle_request()
        
except KeyboardInterrupt:
    print "Interrupt received, shutting down the web server"
    server.socket.close()
    