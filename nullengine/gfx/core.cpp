#include "core.h"

extern HINSTANCE g_hInst;

namespace null::gfx
{
	// Global decl
	IDXGIFactory* dxgiFactory = NULL;
	IDXGIAdapter* dxgiAdapter = NULL;
	IDXGIDevice* dxgiDevice = NULL;
	ID3D11Device* d3ddev = NULL;
	ID3D11DeviceContext* d3dctx = NULL;

	void Initialize()
	{
		constexpr D3D_FEATURE_LEVEL featureLevels[] = {
		   D3D_FEATURE_LEVEL_11_1,
		   D3D_FEATURE_LEVEL_11_0,
		   D3D_FEATURE_LEVEL_10_1,
		   D3D_FEATURE_LEVEL_10_0
		};
		HRESULT hr;
		assert(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory) == S_OK);
		assert(dxgiFactory->EnumAdapters(0, &dxgiAdapter) == S_OK);
		if (D3D11CreateDevice(dxgiAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &d3ddev, NULL, &d3dctx) != S_OK)
			assert(D3D11CreateDevice(dxgiAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, &featureLevels[1], ARRAYSIZE(featureLevels) - 1, D3D11_SDK_VERSION, &d3ddev, NULL, &d3dctx) == S_OK);
		assert(d3ddev->QueryInterface(IID_PPV_ARGS(&dxgiDevice)) == S_OK);
	}

	void Shutdown()
	{
		RELEASE(d3dctx);
		RELEASE(d3ddev);
		RELEASE(dxgiDevice);
		RELEASE(dxgiAdapter);
		RELEASE(dxgiFactory);
	}

#pragma region SWAPCHAIN

	Swapchain::Swapchain(HWND hWnd, uint16_t width, uint16_t height)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		DXGI_SWAP_CHAIN_DESC desc{};
		desc.BufferDesc.Width = width;
		desc.BufferDesc.Height = height;
		desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 0;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = BACKBUFFER_COUNT;
		desc.OutputWindow = hWnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		dxgiFactory->CreateSwapChain(dxgiDevice, &desc, &m_pSwapChain);


		ID3D11Resource* pBackBuf = NULL;
		for (uint8_t i = 0; i < BACKBUFFER_COUNT; i++)
		{
			m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuf));
			d3ddev->CreateRenderTargetView(pBackBuf, NULL, &m_renderTargets[i]);
			pBackBuf->Release();
		}
	}

	void Swapchain::Resize(uint16_t width, uint16_t height)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		for (uint8_t i = 0; i < BACKBUFFER_COUNT; i++)
			RELEASE(m_renderTargets[i]);
		m_pSwapChain->ResizeBuffers(BACKBUFFER_COUNT, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		ID3D11Resource* pBackBuf = NULL;
		for (uint8_t i = 0; i < BACKBUFFER_COUNT; i++)
		{
			m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuf));
			d3ddev->CreateRenderTargetView(pBackBuf, NULL, &m_renderTargets[i]);
			pBackBuf->Release();
		}
	}

	void Swapchain::BeginFrame()
	{
		m_mutex.lock();
	}

	void Swapchain::Present()
	{
		m_mutex.unlock();
		m_pSwapChain->Present(1, 0);
		m_bufferIndex = (m_bufferIndex + 1) % BACKBUFFER_COUNT;
	}

	ID3D11RenderTargetView* Swapchain::GetRTV() const { return m_renderTargets[m_bufferIndex]; }

	void Swapchain::Release()
	{
		RELEASE(m_pSwapChain);
		for (uint8_t i = 0; i < BACKBUFFER_COUNT; i++)
			RELEASE(m_renderTargets[i]);
		m_bufferIndex = 0;
	}
#pragma endregion

}