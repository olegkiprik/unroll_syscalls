clang $1 $2 $3 -std=c17 -DNDEBUG -fstrict-aliasing -Wall -Wextra -Wpedantic -Wno-unused-function -Werror=vla -Wstrict-aliasing -static -fno-builtin -nostdlib -ffast-math -fno-pic -Os asm_x64.s main.c

