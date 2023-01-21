#pragma once
#include "Win.h"
#include "BaseException.h"
#include <d3d11.h>

class Graphics
{
	class Exception : public BaseException
	{
	public:
		Exception( int line, const std::string& file, HRESULT hr );
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorString() const noexcept;
		HRESULT GetErrorCode() const noexcept;
		// Static function to translate error code, can be used statically
		static std::string TranslateErrorCode( HRESULT hRes ) noexcept;
	private:
		HRESULT hr;
	};
public:
	Graphics( HWND hWnd );
	~Graphics();
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
public:
	void BeginFrame();
	void EndFrame();
private:
	ID3D11Device*        pDevice              = nullptr;
	IDXGISwapChain*      pSwapChain           = nullptr;
	ID3D11DeviceContext* pContext             = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
};

