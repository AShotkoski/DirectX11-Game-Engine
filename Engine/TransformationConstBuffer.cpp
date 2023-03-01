#include "TransformationConstBuffer.h"
#include "BindableCodex.h"

namespace Binds
{
	std::unique_ptr<VertexConstantBuffer<TransformationConstBuffer::TransformBuffer>> TransformationConstBuffer::pVertexCBuf;

	TransformationConstBuffer::TransformationConstBuffer( Graphics& gfx, const Drawable& parent )
		:
		parent( parent )
	{
		pVertexCBuf = std::make_unique<VertexConstantBuffer<TransformBuffer>>( gfx );
	}

	void TransformationConstBuffer::Bind( Graphics& gfx )
	{
		UpdateAndBind( gfx, GetBuffer( gfx ) );
	}

	std::string TransformationConstBuffer::GenerateUID( const Drawable& parent )
	{
		using namespace std::string_literals;
		return std::string( typeid( TransformationConstBuffer ).name() + "_"s + std::to_string((size_t)&parent));
	}

	std::shared_ptr<TransformationConstBuffer> TransformationConstBuffer::Resolve( Graphics& gfx, const Drawable& parent )
	{
		return Codex::Resolve<TransformationConstBuffer>(gfx,parent);
	}

	TransformationConstBuffer::TransformBuffer TransformationConstBuffer::GetBuffer( Graphics& gfx ) const
	{
		// Update vertex const buffer with projected transformation matrix given by 
		// owner drawable
		const auto parentModel = parent.GetTransformationMatrix();

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