#pragma once
#include "Bindable.h"
#include <string>

namespace Binds
{

    class Texture : public Bindable
    {
    public:
        Texture( Graphics& gfx, const std::wstring& path );
        void Bind( Graphics& gfx ) override;
        static std::string GenerateUID(  const std::wstring& path );
        static std::shared_ptr<Bindable> Resolve( Graphics& gfx, const std::wstring& path );
    private:
        Microsoft::WRL::ComPtr<ID3D11Resource> pResource;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pResourceView;
    };

};