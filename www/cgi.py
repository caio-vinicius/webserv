#!/usr/bin/python3

import os

name = os.environ['GATEWAY_INTERFACE']
print(f"<html><head><title>Hello, {name}!</title></head><body>")
print("<h1>Hello, World!</h1>")
print("</body></html>")
