#include "Texture.h"
#include "Macros.h"
#include "BindableCodex.h"
#include "GeneralUtilities.h"
#include "DirectXTex.h"

namespace Binds
{

    Texture::Texture( Graphics& gfx, const std::wstring& path )
    {
        HRESULT hr;

        THROW_FAILED_GFX(CoInitializeEx( nullptr, COINIT_MULTITHREADED ));
        // Create texure
        auto pScratch = std::make_unique<DirectX::ScratchImage>();
		THROW_FAILED_GFX( DirectX::LoadFromWICFile(
			path.c_str(),
			DirectX::WIC_FLAGS_NONE,
			nullptr,
			*pScratch ) );
        D3D11_TEXTURE2D_DESC td = {};
        td.Format = pScratch->GetMetadata().format;
        td.ArraySize = (UINT)pScratch->GetMetadata().arraySize;
        td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.CPUAccessFlags = 0u;
        td.Height = (UINT)pScratch->GetMetadata().height;
        td.Width = (UINT)pScratch->GetMetadata().width;
        td.MipLevels = (UINT)pScratch->GetMetadata().mipLevels;
        td.SampleDesc.Count = 1;
        td.SampleDesc.Quality = 0;
        td.MiscFlags = 0u;

        D3D11_SUBRESOURCE_DATA sd = {};
        size_t stmempitch;
        size_t stmempitchslice;
        THROW_FAILED_GFX(DirectX::ComputePitch( td.Format, td.Width, td.Height, stmempitch, stmempitchslice ));
        sd.SysMemPitch = (UINT)stmempitch;
        sd.SysMemSlicePitch = (UINT)stmempitchslice;
        sd.pSysMem = pScratch->GetPixels();
        
        Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

        THROW_FAILED_GFX(pGetDevice( gfx )->CreateTexture2D( &td, &sd, &pTexture ));

        D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
        srvd.Format = td.Format;
        srvd.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
        srvd.Texture2D.MipLevels = td.MipLevels;
        srvd.Texture2D.MostDetailedMip = 0u;

        THROW_FAILED_GFX(
			pGetDevice( gfx )->CreateShaderResourceView( pTexture.Get(), &srvd, &pResourceView ) );
        // clean
        CoUninitialize();
        pScratch->Release();
    }

    void Texture::Bind( Graphics& gfx )
    {
        pGetContext( gfx )->PSSetShaderResources( 0u, 1u, pResourceView.GetAddressOf() );
    }

    std::string Texture::GenerateUID( const std::wstring& path )
    {
        using namespace std::string_literals;
        std::string pathA = Util::WStringToString(path);
        return std::string( typeid( Texture ).name() + "_"s + pathA );
    }

    std::shared_ptr<Bindable> Texture::Resolve( Graphics& gfx, const std::wstring& path )
    {
        return Codex::Resolve<Texture>( gfx, path );
    }

};