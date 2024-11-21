# IntIO

---

Reverse the driver to find a check:

```c
if(len > 4);
```

Should click; what is being checked? Scroll down and you find:

```c
if(stack_888[0] == 'F')
if(stack_888[1] == 'l')
if(stack_888[1] == '@')
if(stack_888[1] == 'G')
```

Now, you will write a simple loader (or copy one from ired.team ;)), and then also; you have to find the symbolic links; which you can easily with `DebugView` when you load the driver as I've included it in the debug logs.

```
AshfaqDriverLink
```

Once you have that, you just send `Fl@G1` (the 1 because the check for length is > 4 and not >= 4), once you do that, you get the flag.


### NOTE:
There is another solution. That is; you have to find the S_BOX which I created myself, you re-build that sbox, and then you simply map the `flag` variable against that s-box to get the flag.