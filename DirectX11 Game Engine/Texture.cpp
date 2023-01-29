#include "Texture.h"
#include "DDSLoader/DDSTextureLoader.h"
#include "Macros.h"

Texture::Texture( Graphics& gfx, const std::wstring& path )
{
    HRESULT hr;
    // Create texure
    THROW_FAILED_GFX( DirectX::CreateDDSTextureFromFile( pGetDevice( gfx ), path.c_str(),
                                                         &pResource,
                                                         &pResourceView ) );
}

void Texture::Bind( Graphics& gfx )
{
    pGetContext( gfx )->PSSetShaderResources( 0u, 1u, pResourceView.GetAddressOf() );
}
