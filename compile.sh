clang -std=c17 -Dasm=__asm__ -DNDEBUG -Dstatic="" -Wall -Wextra -Wpedantic -Wno-language-extension-token -Wno-unused-function -static -fno-builtin -nostdlib -ffast-math -Os asm_x64.s main.c

