#pragma once
#include "Bindable.h"
#include "Macros.h"
#include "Win.h"
#include "DDSLoader/DDSTextureLoader.h"
#include <string>

class Texture : public Bindable
{
public:
    Texture( Graphics& gfx, const std::wstring& path )
    {
        HRESULT hr;  
        // Create texure
        THROW_FAILED_GFX( DirectX::CreateDDSTextureFromFile( pGetDevice( gfx ), path.c_str(),
                                                             &pResource,
                                                             &pResourceView ) );
    }
    void Bind( Graphics& gfx ) override
    {
        pGetContext( gfx )->PSSetShaderResources( 0u, 1u, pResourceView.GetAddressOf() );
    }
private:
    Microsoft::WRL::ComPtr<ID3D11Resource> pResource;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pResourceView;
};

