#include "shaders.h"

namespace null::gfx
{
	BOOL CALLBACK CompileShadersCB(HMODULE hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR lParam)
	{
		HRSRC hResource = FindResource(hModule, lpName, lpType);
		HGLOBAL hMemory = LoadResource(hModule, hResource);
		DWORD dwSize = SizeofResource(hModule, hResource);
		LPVOID lpAddress = LockResource(hMemory);

		ID3DBlob* compiledShader = NULL;
		assert(D3DCompile(lpAddress, dwSize, NULL, NULL, NULL, "main", "lpType", 0, 0, &compiledShader, NULL) == S_OK);

		//TODO store compiled shaders

		UnlockResource(hMemory);
		FreeResource(hMemory);
		return TRUE;
	}
}