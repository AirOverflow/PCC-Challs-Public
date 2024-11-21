#!/usr/local/bin/python3

from subprocess import run, PIPE
from tempfile import NamedTemporaryFile
from random import choice
from string import ascii_letters
import os

MAX_LEN = 0x100

class CAAS(object):

	def __init__(self, content: str):

		if not content:
			raise Exception("EmptyContentException.")

		self.content: str = content.strip()
		content = content[:-1] if content[-1] == "\n" else ""

	def validate(self) -> bool:
		keywords = (
			"sock", "inet", "bind", "listen", "asm",
			"accept", "connect", "getaddrinfo",
			"gethostbyname", "syscall", "execve",
			"exec", "fork", "scanf", "pcc", "ctf",
			"fopen", "put", "printf", "fread", "sendfile",
			"fwrite", "flag", "whoami", "less", "chmod"
			"ls", "pwd", "grep", "find", "chown", "string",
			"bin", "sh", "cat", "setuid", "getuid",
			"openat", "main", "tmp", "dev", "home", "app")
		characters = ("#", "\\", "^", "[", "]", "|", "{", "}", "?", "~", "(*)()", "==", "!=", ">=", "<=")
		types = ("void", "char", "int", "long", "short")

		if len(self.content) >= MAX_LEN or any(
			keyword in self.content.lower() for keyword in keywords) or any(
			char in self.content for char in characters) or any(
			type in self.content.lower() for type in types):
			return False

		return True

	def compile(self):

		with NamedTemporaryFile(suffix='.c', delete=False) as f:
				f.write(content.encode())
				self._src = f.name

		with NamedTemporaryFile(delete=False) as f:
			self._out = f.name

		output = run([
			'/usr/bin/gcc', "-B/usr/bin", self._src,
			'-w', '-o', self._out, '-O0',
			"-Wl,--entry=_", "-nostartfiles",
			'-fPIE', '-fno-stack-protector',
			'-trigraphs', '-no-pie', '-mpreferred-stack-boundary=8',
			'-Wl,-z,relro,-z,now', '-Wl,-z,noexecstack',
		], stderr=PIPE, stdout=PIPE)

		if output.returncode != 0:
			return False
		
		return True

	def run_task(self):
		if not self.validate():
			os.remove(self._src)
			raise Exception("Validation Failed!")
		
		if not self.compile():
			os.remove(self._src)
			raise Exception("Compilation Failed!")

		os.chdir("/")
		output = run([self._out],stderr=PIPE, stdout=PIPE)
		output = output.stdout.decode('utf-8')

		out_chk = ["pcc", "{", "}"]
		if any(keyword in output.lower() for keyword in out_chk):
			os.remove(self._src)
			os.remove(self._out)
			raise Exception("Validation failed on response!")
		
		print(output)
		os.remove(self._src)
		os.remove(self._out)

if __name__ == "__main__":

	content = ""
	print("Enter src: (<<EOF>> to stop): ", end="")

	try:
		while (_in := input()) != "<<EOF>>" or _in == "":
			content += _in + "\n"
	except KeyboardInterrupt:
		pass

	caas = CAAS(content=content)
	caas.run_task()
