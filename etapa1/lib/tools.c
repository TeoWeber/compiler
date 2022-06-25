#include "include/tools.h"

int getLineNumber(void)
{
  return lineNumber;
}

int isRunning(void)
{
  return running;
}

void initMe(void)
{
  running = 1;
  lineNumber = 1;

//   hashInit();
}