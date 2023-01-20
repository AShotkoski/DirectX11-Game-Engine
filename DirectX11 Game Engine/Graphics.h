#pragma once
#include "Win.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics( HWND hWnd );
	~Graphics();
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
public:
	void BeginFrame();
	void EndFrame();
private:
	ID3D11Device*        pDevice    = nullptr;
	IDXGISwapChain*      pSwapChain = nullptr;
	ID3D11DeviceContext* pContext   = nullptr;
};

