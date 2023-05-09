#!/usr/bin/python3

import os
import sys

print("Content-Type: text/html\r\n\r\n")

data = sys.stdin.readline()
print(data)
name = os.environ['GATEWAY_INTERFACE']
print(f"<html><head><title>Hello, {name}!</title></head><body>")
print("<h1>Hello, World!</h1>")
print("</body></html>")
