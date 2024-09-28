#ifndef REGALLOC_H
#define REGALLOC_H

#define REG_AMOUNT 9

typedef struct {
  const char* r64;
  const char* r32;
  const char* r16;
  const char* r8;
  char in_use;
  // int value;
} reg;

static reg regs[REG_AMOUNT] =
{
  (reg){"rax", "eax", "ax", "al", 0},
  (reg){"rdi", "edi", "di", "dil", 0}, // 1 param
  (reg){"rsi", "esi", "si", "sil", 0}, // 2 param
  (reg){"rdx", "edx", "dx", "dl", 0}, // 3 param
  (reg){"rcx", "ecx", "cx", "cl", 0}, // 4 param
  (reg){"r8", "r8d", "r8w", "r8b", 0},  // 5 param
  (reg){"r9", "r9d", "r9w", "r9b", 0},  // 6 param
  (reg){"r10", "r10d", "r10w", "r10b", 0},
  (reg){"r11", "r11d", "r11w", "r11b", 0}
};

const char* selReg(const int regI, const int size);

void useReg(int regI);
void freeReg(int regI);

#endif
