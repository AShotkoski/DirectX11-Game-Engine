#pragma once
#include "Win.h"
#include "BaseException.h"
#include <d3d11.h>
#include <wrl.h>

class Graphics
{
private:
	friend class Bindable;
public:
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
	~Graphics() = default;
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
public:

	void BeginFrame();

	void DrawTest(float angle);

	void Draw( UINT vertexCount, UINT start );

	void EndFrame();
private:
	Microsoft::WRL::ComPtr<ID3D11Device>           pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>         pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>    pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
};

