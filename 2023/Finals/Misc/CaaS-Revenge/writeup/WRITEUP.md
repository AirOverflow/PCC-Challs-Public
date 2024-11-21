# CaaS.

---

How one should've approached this challenge:

- You have 60 bytes, but you have a persistent filesystem, you can write, another header file, byte-by-byte and then simply include that header file, call the function you've written in the header file and then execute /bin/bash

- Also, if you got *Invalid keyword* in output and connection got broken, you could simply comment out part-1 and run part-2 and you will still have your shell.

---