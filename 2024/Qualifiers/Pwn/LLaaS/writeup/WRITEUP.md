# llaas

---

Linked List as a Service (LLaaS) is a basic binary exploitation challenge that focuses on the following techniques:

- Arbitrary Address Read (AAR)
- Out of Bound Write (OOB-Write)
- Vtable Hijacking (sort of)

In this program, we have our own vtable. Since it's an `unordered_map`, it is stored on the heap. Along with that, whatever option we select; based on that, a specific function is called. The argument passed to these functions is of type `LinkedList<__int64_t*>*` which is also stored on the heap.

So, the end goal is to overwrite the `list` variable with `/bin/sh` and overwrite the vtable entry with `system`.

For that, we firstly need to leak values.

Looking at the `print_at_idx` method, we can see:

```cpp
std::cout << *(this->list[idx]) << std::endl;
```

Whatever the data we have, it will be dereferenced, and then printed. So, what we can do, is look for an address that contains an address to either libc, the binary or the heap (or even the stack).
