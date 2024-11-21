# Uber

To build this project:

```bash
make
```

This will generate the `Uber.ko` file based on the flag that you add inside the `flag.py`.

To clean all the generated files:

```bash
make clean
```

In case you get errors when building the driver:

```bash
# For Debian-based systems
sudo apt install linux-headers-amd64

# For Arch:
sudo pacman -Sy linux-headers
```

---
