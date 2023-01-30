#pragma once

#include <dxgi.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "../common.h"

#include <mutex>

namespace null::gfx
{
	void Initialize();
	void Shutdown();

	class Swapchain
	{
	public:
		Swapchain() = default;
		DISABLE_COPY(Swapchain);
		DISABLE_MOVE(Swapchain);

		explicit Swapchain(HWND hWnd, uint16_t width, uint16_t height);
		void Resize(uint16_t width, uint16_t height);
		void BeginFrame();
		void Present();
		ID3D11RenderTargetView* GetRTV() const;
		void Release();
	private:
		std::mutex m_mutex;
		IDXGISwapChain* m_pSwapChain = NULL;
		ID3D11RenderTargetView* m_renderTargets[BACKBUFFER_COUNT]{};
		uint8_t m_bufferIndex = 0;
	};
}

