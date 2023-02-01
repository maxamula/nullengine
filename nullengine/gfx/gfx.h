#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

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

	class DepthStencilBuffer
	{
	public:
		DepthStencilBuffer() = default;
		DISABLE_COPY(DepthStencilBuffer);
		DISABLE_MOVE(DepthStencilBuffer);
		explicit DepthStencilBuffer(unsigned short width, unsigned short height);

		void Release();

		void Resize(unsigned short width, unsigned short height);

	private:
		void _CreateInterfaces();

		ID3D11Texture2D* m_depthStencilBuffer = NULL;
		ID3D11DepthStencilView* m_depthStencilView = NULL;
		ID3D11DepthStencilState* m_depthStencilState = NULL;
		unsigned short m_width = 0;
		unsigned short m_height = 0;
	};

	class Viewport
	{
	public:
		Viewport() = default;
		DISABLE_COPY(Viewport);
		DISABLE_MOVE(Viewport);

		explicit Viewport(unsigned short width, unsigned short height);
		void Set();
		void Resize(unsigned short width, unsigned short height);

	private:
		D3D11_VIEWPORT m_viewport{};
	};
}

