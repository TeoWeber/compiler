// tools.c - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#include "tools.h"

int getLineNumber(void) {
  return lineNumber;
}

int isRunning(void) {
  return running;
}

void initMe(void) {
  running = 1;
  lineNumber = 1;

  hashInit();
}