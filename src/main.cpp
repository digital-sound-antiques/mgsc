#include <iostream>
#include <fstream>
#include <string>
#include "mgsc.h"

static MGSC mgsc;

void help() {
  std::cerr << "Usage: mgsc MMLFILENAME [MGSFILENAME] [-T]" << std::endl;
}

int main(int argc, char *argv[]) {
  std::fstream tmp, mml, mgs;
  std::string mml_name, mgs_name, mgsc_name;
  int i, outc = 0, option = 0;

  if (argc == 1) {
    help();
    return 1;
  }

  mml_name = std::string(argv[1]);

  // Parse options
  for (i = 2; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'T':
      case 't':
        option |= MGSC::OPT_TRACK_STATUS;
        break;
      default:
        help();
        break;
      }
    } else if (outc == 0)
      outc = i;
    else {
      help();
      return 1;
    }
  }

  if (outc) {
    mgs_name = std::string(argv[outc]);
  } else {
    mgs_name = mml_name.substr(0, mml_name.rfind('.')) + ".MGS";
  }

  // must open in binary mode.
  mml.open(mml_name.c_str(), std::ios::in | std::ios::binary);
  if (mml.fail()) {
    std::cerr << "Can't open " << mml_name << std::endl;
    return 1;
  }

  mgs.open(mgs_name.c_str(),
           std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
  if (mgs.fail()) {
    std::cerr << "Can't create " << mgs_name << std::endl;
    return 1;
  }

  if (!mgsc.Compile(&mml, &mgs, option)) {
    std::cerr << mgsc.GetErrorMessage() << std::endl;
    return 1;
  }

  mgs.close();
  mml.close();

  return 0;
}
