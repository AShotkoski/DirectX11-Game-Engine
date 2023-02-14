#pragma once
#include "Bindable.h"
#include <string>

namespace Binds
{

    class Texture : public Bindable
    {
    public:
        Texture( Graphics& gfx, const std::wstring& path, UINT slot = 0u );
        void Bind( Graphics& gfx ) override;
        static std::string GenerateUID(  const std::wstring& path, UINT slot = 0u );
        static std::shared_ptr<Bindable> Resolve( Graphics& gfx, const std::wstring& path, UINT slot = 0u );
    private:
        UINT slot_;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pResourceView;
    };

};