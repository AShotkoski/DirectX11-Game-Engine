#include "Bindable.h"

ID3D11Device* Bindable::pGetDevice( Graphics& gfx ) noexcept
{
    return gfx.pDevice.Get();
}

ID3D11DeviceContext* Bindable::pGetContext( Graphics& gfx ) noexcept
{
    return gfx.pContext.Get();
}
