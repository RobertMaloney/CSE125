#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include "..\GameClient.h"
#include "..\graphics\GraphicsEngine.h"
#include <deque>

class InputHandler
{
public:

   static void handleKey(int key, int action, int mods);
};

#endif