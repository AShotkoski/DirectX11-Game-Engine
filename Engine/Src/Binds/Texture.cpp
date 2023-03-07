#include "Texture.h"
#include "Macros.h"
#include "BindableCodex.h"
#include "Util/GeneralUtilities.h"
#include "DirectXTex.h"

namespace Binds
{

    Texture::Texture( Graphics& gfx, std::filesystem::path path, UINT slot )
        : slot_(slot)
    {
        HRESULT hr;

        THROW_FAILED_GFX(CoInitialize( nullptr ));
        // Create texure
        DirectX::ScratchImage Scratch;
		THROW_FAILED_GFX( DirectX::LoadFromWICFile(
			path.wstring().c_str(),
			DirectX::WIC_FLAGS_NONE,
			nullptr,
			Scratch ) );

        // set alpha flag
        bhasAlpha = !Scratch.IsAlphaAllOpaque();

		THROW_FAILED_GFX( DirectX::CreateShaderResourceView(
			pGetDevice( gfx ),
			Scratch.GetImage( 0, 0, 0 ),
			1,
			Scratch.GetMetadata(),
			&pResourceView ) );
        D3D11_TEXTURE2D_DESC td = {};
        td.Format = Scratch.GetMetadata().format;
        td.ArraySize = (UINT)Scratch.GetMetadata().arraySize;
        td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.CPUAccessFlags = 0u;
        td.Height = (UINT)Scratch.GetMetadata().height;
        td.Width = (UINT)Scratch.GetMetadata().width;
        td.MipLevels = 0u;
        td.SampleDesc.Count = 1;
        td.SampleDesc.Quality = 0;
        td.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

        D3D11_SUBRESOURCE_DATA sd = {};
        size_t stmempitch;
        size_t stmempitchslice;
        THROW_FAILED_GFX(DirectX::ComputePitch( td.Format, td.Width, td.Height, stmempitch, stmempitchslice ));
        
        Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

        THROW_FAILED_GFX(pGetDevice( gfx )->CreateTexture2D( &td, nullptr, &pTexture ));
        pGetContext( gfx )->UpdateSubresource( pTexture.Get(), 0u, nullptr, Scratch.GetPixels(), (UINT)stmempitch, (UINT)stmempitchslice );

        D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
        srvd.Format = td.Format;
        srvd.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
        srvd.Texture2D.MipLevels = -1;
        srvd.Texture2D.MostDetailedMip = 0u;

        THROW_FAILED_GFX(
			pGetDevice( gfx )->CreateShaderResourceView( pTexture.Get(), &srvd, &pResourceView ) );

        pGetContext( gfx )->GenerateMips( pResourceView.Get() );
        // clean     
        CoUninitialize();
        Scratch.Release();

    }

    void Texture::Bind( Graphics& gfx )
    {
        pGetContext( gfx )->PSSetShaderResources( slot_, 1u, pResourceView.GetAddressOf() );
    }

    std::string Texture::GenerateUID( std::filesystem::path path, UINT slot )
    {
        using namespace std::string_literals;
        return std::string( typeid( Texture ).name() + "_"s + path.string() + "_"s + std::to_string(slot));
    }

    std::shared_ptr<Texture> Texture::Resolve( Graphics& gfx, std::filesystem::path path, UINT slot )
    {
        return Codex::Resolve<Texture>( gfx, path, slot );
    }

    bool Texture::hasAlpha() const
    {
        return bhasAlpha;
    }

};