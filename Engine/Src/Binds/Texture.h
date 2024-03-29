#pragma once
#include "Bindable.h"
#include <string>
#include <filesystem>

namespace Binds
{

    class Texture : public Bindable
    {
    public:
        Texture( Graphics& gfx, std::filesystem::path path, UINT slot = 0u );
        void Bind( Graphics& gfx ) override;
        static std::string GenerateUID( std::filesystem::path, UINT slot );
        static std::shared_ptr<Texture> Resolve( Graphics& gfx, std::filesystem::path, UINT slot = 0u );
        bool hasAlpha() const;
    private:
        UINT slot_;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pResourceView;
        bool bhasAlpha;
    };

};