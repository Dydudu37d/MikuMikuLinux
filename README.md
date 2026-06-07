# MikuMikuLinux
## MikuMikuLinux：
MikuMikuDance的ARM和Linux移植版

## 硬件加速：
REP&ERMS
SSE
AVX
NEON
NT

### Clang
```
# ARM
clang -mcpu=native -lSDL2 -O3 -Wall Main.c -o MikuMikuLinux
```
```
# x86
clang -march=native -lSDL2 -O3 -Wall Main.c -o MikuMikuLinux
```
