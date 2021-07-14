#include "stdafx.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include <SDL.h>
#include "Minigin.h"

int main(int, char* []) //
{
	Minigin::GetInstance().Run();
	return 0;
}