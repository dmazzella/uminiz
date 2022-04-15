uminiz
=====

Micropython wrapper for [miniz](https://github.com/richgel999/miniz), miniz: Single C source file zlib-replacement library, originally from code.google.com/p/miniz.

Compiling the cmodule into MicroPython
=====

To build such a module, compile MicroPython with an extra make flag named ```USER_C_MODULES``` set to the directory containing all modules you want included (not to the module itself).


#### unix port

```bash
➜  ~ git clone https://github.com/micropython/micropython.git
➜  ~ cd micropython
➜  micropython (master) ✗ git clone https://github.com/dmazzella/uminiz.git ports/unix/cmodules/uminiz
➜  micropython (uminiz) ✗ cd ports/unix/cmodules/uminiz
➜  micropython (uminiz) ✗ git submodule update --init
➜  micropython (uminiz) ✗ cd ../../../../
➜  micropython (master) ✗ make -j8 -C mpy-cross && make -j2 -C ports/unix/ VARIANT="dev" USER_C_MODULES="$(pwd)/ports/unix/cmodules"
```


#### stm32 port

```bash
~ git clone https://github.com/micropython/micropython.git micropython
➜  ~ cd micropython
➜  micropython (master) ✗ git submodule update --init
➜  micropython (master) ✗ git clone https://github.com/dmazzella/uminiz.git ports/stm32/boards/PYBD_SF6/cmodules/uminiz
➜  micropython (uminiz) ✗ cd ports/stm32/boards/PYBD_SF6/cmodules/uminiz
➜  micropython (uminiz) ✗ git submodule update --init
➜  micropython (uminiz) ✗ cd ../../../../../../
➜  micropython (master) ✗ make -j2 -C mpy-cross && make -j2 -C ports/stm32/ BOARD="PYBD_SF6" USER_C_MODULES="$(pwd)/ports/stm32/boards/PYBD_SF6/cmodules"
```

Usage
=====

```python
MicroPython v1.17-76-gc42c1c871-dirty on 2021-10-13; PYBD-SF6W with STM32F767IIK
Type "help()" for more information.
>>> import _miniz
>>> d = "test " * 100
>>> d
'test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test '
>>> c = _miniz.compress(d)
>>> c
b'x\x01\xed\xc4A\x11\x00\x00\x00\x01\xc1*\xcaI\xe0\xfa\x8f |\x16\x07q\x96(\xc11\xbb\x81'
>>> _miniz.decompress(c)
b'test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test test '
>>> 
```

API
=====
 - ```_miniz.compress(data, level=6)```
 - ```_miniz.decompress(data, maxout=2048)```
