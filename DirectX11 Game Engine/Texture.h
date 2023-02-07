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
    private:
        Microsoft::WRL::ComPtr<ID3D11Resource> pResource;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pResourceView;
    };

};