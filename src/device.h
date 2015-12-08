#ifndef _DEVICE_H_
#define _DEVICE_H_
#include "xtypes.h"

class IDevice {
public:
  virtual void Reset() = 0;
  virtual bool Write(UINT32 adr, UINT32 val, UINT32 id = 0) = 0;
  virtual bool Read(UINT32 adr, UINT32 &val, UINT32 id = 0) = 0;
  virtual void SetOption(int id, int val){};
};

#endif
