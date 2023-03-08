#include "GraphicsResource.h"

ID3D11Device* GraphicsResource::pGetDevice( Graphics& gfx ) noexcept
{
    return gfx.pDevice.Get();
}

ID3D11DeviceContext* GraphicsResource::pGetContext( Graphics& gfx ) noexcept
{
    return gfx.pContext.Get();
}
