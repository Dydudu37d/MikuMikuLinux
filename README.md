# MikuMikuLinux
# MikuMikuLinux：
面向硬件编程和Linux包括x86和ARM的MikuMikuDance改版

## 我们使用的硬件加速包括：
REP前缀（太奶级别）
SSE加速（爷爷级别）
AVX256（爸爸级别）
AVX512（兼容毁灭）
NEON （古希腊掌管ARM CPU加速的神）
NT（这位更是堪比UDP网络协议的传输方法，Non-Temporal）

### Clang
```
# ARM
clang -mcpu=native -lSDL2 -O3 -Wall Main.c -o MikuMikuLinux
```
```
# x86
clang -march=native -lSDL2 -O3 -Wall Main.c -o MikuMikuLinux
```
