#include "discrete_key_control.h"
#include "udero/UderoLogger.h"
#ifdef WIN32
#include <conio.h>
#endif


#ifdef WIN32
#define KBHIT _kbhit
#define GETCH _getch

void INIT_SCR() {}
void EXIT_SCR() {}
void CLEAR_SCR() { system("cls");  }
#else
#include "Console.h"
#define KBHIT ssr::myKbhit
#define GETCH ssr::myGetch


void INIT_SCR() { ssr::init_scr(); }
void EXIT_SCR() { ssr::exit_scr(); }
void CLEAR_SCR() { ssr::clear_scr(); }
#endif
///int keyPressed = 0;

bool initKeyInput(ParamLoader* loader) {
	INIT_SCR();
  return true;
}
void finiKeyInput() {
	EXIT_SCR();
}

KeyContext checkKeyInput(const KeyContext& ctx) {
	CLEAR_SCR();
  KeyContext newCtx = ctx;
  if (KBHIT()) {
    int k = GETCH();
    UVERBOSE("getch = %c(0x%02X)", (char)k, k);
    newCtx.keyReleasedCount = 0;
    newCtx.old_key = ctx.new_key;
    newCtx.new_key = k;

  } else {
    if (ctx.new_key != 0) {
      newCtx.keyReleasedCount = ctx.keyReleasedCount + 1;
      newCtx.old_key = ctx.new_key;
      newCtx.new_key = ctx.new_key;
      if (newCtx.keyReleasedCount >= keyReleasedCountThreshold) {
	newCtx.new_key = 0;
	newCtx.old_key = ctx.new_key;
      }
    } else {
      newCtx.new_key = 0;
      newCtx.old_key = 0;
      newCtx.keyReleasedCount = 0;
    }
  }
  return newCtx;
}


