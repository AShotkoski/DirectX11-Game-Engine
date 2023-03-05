#include "TransformationConstBuffer.h"
#include "BindableCodex.h"
#include "log.h"
#include "Drawable/Drawable.h"
#include "Graphics/TechniqueProbe.h"

namespace Binds
{
	std::unique_ptr<VertexConstantBuffer<TransformationConstBuffer::TransformBuffer>> TransformationConstBuffer::pVertexCBuf;

	TransformationConstBuffer::TransformationConstBuffer( Graphics& gfx )
		: pParent{nullptr}
	{
		pVertexCBuf = std::make_unique<VertexConstantBuffer<TransformBuffer>>( gfx );
	}

	void TransformationConstBuffer::Bind( Graphics& gfx )
	{
		UpdateAndBind( gfx, GetBuffer( gfx ) );
	}

	void TransformationConstBuffer::InitParentRefs( const Drawable& parent )
	{
		pParent = &parent;
	}


	TransformationConstBuffer::TransformBuffer TransformationConstBuffer::GetBuffer( Graphics& gfx ) const
	{
		DCHECK_NOTNULL_F( pParent, "Attempted to get transform matrix without initializing parent" );
		// Update vertex const buffer with projected transformation matrix given by 
		// owner drawable
		const auto parentModel = pParent->GetTransformationMatrix();

		return {
			DirectX::XMMatrixTranspose( parentModel ), // model
			DirectX::XMMatrixTranspose( gfx.GetCamera().GetInvMatrix() ), //Invview
			DirectX::XMMatrixTranspose( parentModel * gfx.GetCamera().GetMatrix() * gfx.GetProjection() ) //proj
		};
	}

	void TransformationConstBuffer::UpdateAndBind( Graphics& gfx, TransformBuffer buffer ) const
	{
		pVertexCBuf->Update( gfx, buffer );
		pVertexCBuf->Bind( gfx );
	}

};