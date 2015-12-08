#ifndef _MGSCWIN_H_
#define _MGSCWIN_H_
#include <stdlib.h>
#include <iostream>
#include "z80.h"
#include "ram64k.h"
#include "bdos.h"

class MGSC {
protected:
  Z80_CPU cpu;
  RAM64K ram;
  EMU_BDOS bdos;
  char errmsg[512];

public:
  enum { OPT_TRACK_STATUS = 1 };
  MGSC();
  ~MGSC();
  bool LoadCompiler(std::istream *mgsc);
  bool Compile(std::iostream *mml, std::iostream *mgs, int option);
  void SetConsole(std::istream *in, std::ostream *out);
  const char *GetErrorMessage();
};

#endif