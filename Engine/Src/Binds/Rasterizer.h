#pragma once
#include "Bindable.h"

namespace Binds
{
    class Rasterizer : public Bindable
    {
    public:
        Rasterizer( Graphics& gfx, bool cullBackface = true );
        virtual void Bind( Graphics& gfx ) override;
        static std::string GenerateUID( bool cullBackface );
        static std::shared_ptr<Rasterizer> Resolve( Graphics& gfx, bool cullBackface = true );
    private:
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;
    };

}
