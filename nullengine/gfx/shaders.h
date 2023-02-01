#pragma once
#include "gfx.h"

#include <windows.h>

namespace null::gfx
{
	BOOL CALLBACK CompileShadersCB(HMODULE hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR lParam);
}
