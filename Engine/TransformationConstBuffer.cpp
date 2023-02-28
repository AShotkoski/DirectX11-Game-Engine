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
		// Update vertex const buffer with projected transformation matrix given by 
		// owner drawable
		const auto parentModel = parent.GetTransformationMatrix();

		const TransformBuffer tb = {
			DirectX::XMMatrixTranspose( parentModel ), // model
			DirectX::XMMatrixTranspose( gfx.GetCamera().GetInvMatrix() ), //Invview
			DirectX::XMMatrixTranspose( parentModel * gfx.GetCamera().GetMatrix() * gfx.GetProjection() ) //proj
		};

		pVertexCBuf->Update( gfx, tb );
		pVertexCBuf->Bind( gfx );
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

};