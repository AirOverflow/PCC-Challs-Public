#!/usr/bin/env python3
# Author: @TheFlash2k

'''
Description:
	This Compiler as a Service is a production grade application that will simply parse a C-code and then execute it.

Changelog(s):
	v0.1 - Validated existence of sockets
	v0.2 - Validated existence of system function and halted program if it exists
	v0.3 - Validated existence of word "PCC" and "FLAG" in the program output.

	# For finals:
	v0.4 - Added scanf keyword in validate keywords to prevent user input
	v0.5 - Remove Output and Limited to Local testing by setting everything to DEBUG
	v0.6 - Decreased allowed user input length to 60 bytes.
'''

DEBUG = False # Maybe I forgot to turn off debug on remote too?
MAX_CONTENT_LEN = 60

import subprocess
from tempfile import NamedTemporaryFile
from random import choice
from string import ascii_letters, digits
import os

def debug(msg: str) -> None:
	if DEBUG: print(f"[DEBUG] {msg}")

def get_random_string(length: int = 10) -> str:
	return ''.join(choice(ascii_letters + digits) for _ in range(length))

def validate(content: str, output=False) -> str:
	''' Check if invalid keywords exist. '''
	keywords = [
		"sock", "inet", "bind", "listen", "accept", "connect", "getaddrinfo", "gethostbyname", "system", "execve", "exec", "fork", "scanf", "pcc", "ctf", "freopen", "puts", "printf", "read", "write", "flag", "whoami", "cat", "ls", "pwd", "grep", "find", "chmod", "chown", "string", "stdlib", "bin", "sh"
	]

	kws = content.lower()
	for keyword in keywords:
		if keyword in kws:
			msg = f"Invalid keyword [{kws}] found"
			if output: msg += " in output of command"
			raise Exception(msg)
		
	return content

def compile(content: str) -> str:

	name = compiled_bin = ""
	try:
		if len(content) > MAX_CONTENT_LEN:
			raise Exception("Content too long.")
		
		content = validate(content)

		with NamedTemporaryFile(suffix='.c', delete=False) as f:
			f.write(content.encode())
		name = f.name

		compiled_bin = get_random_string(10)

		debug(f"STORED AT: {name}")
		os.environ['PATH']="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" # Manually adding the path to fix cc1 issue.
		output = subprocess.run(['gcc', f.name, '-w', '-o', compiled_bin], stderr=subprocess.PIPE, stdout=subprocess.PIPE)
		if output.returncode != 0:
			debug(f"stdout: {output.stdout.decode()}")
			debug(f"stderr: {output.stderr.decode()}")
			raise Exception("[ERROR] Compilation failed.")

		del os.environ['PATH']
		''' Replaced subprocess.run with Popen '''
		process = subprocess.Popen(
			f"./{compiled_bin}",
			stderr=subprocess.PIPE,
			stdout=subprocess.PIPE,
			shell=True,
			encoding='utf-8',
    		errors='replace')
		while True:
			output = process.stdout.readline()
			if output == '' and process.poll != None:
				break
			output = validate(output, output=True)
			print(output)

	except Exception as E:
		print(E)
		pass
	finally:
		subprocess.run(["rm", "-f", name, compiled_bin])

if __name__ == "__main__":

	data = ""
	print("Enter the C code to compile: ")
	while True:
		try:
			curr_d = input()
			if curr_d == "" or curr_d.lower() == "end-of-code":
				break
			data += curr_d + "\n"

		except Exception as E:
			debug(E)
			break

	compile(data)
