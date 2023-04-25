#!/usr/bin/python3

import os

content_type = os.environ['CONTENT_TYPE']
print(f"Content-type: {content_type}\n\n")
name = os.environ['GATEWAY_INTERFACE']
print(f"<html><head><title>Hello, {name}!</title></head><body>")
print("<h1>Hello, World!</h1>")
print("</body></html>")
