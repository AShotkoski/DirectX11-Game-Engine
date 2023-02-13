#include "Texture.h"
#include "DDSLoader/DDSTextureLoader.h"
#include "Macros.h"
#include "BindableCodex.h"
#include "GeneralUtilities.h"
#include <algorithm>

namespace Binds
{

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