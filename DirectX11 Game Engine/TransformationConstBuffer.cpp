#include "TransformationConstBuffer.h"
#include "BindableCodex.h"

namespace Binds
{

	TransformationConstBuffer::TransformationConstBuffer( Graphics& gfx, const Drawable& parent )
		:
		VertexCBuf( gfx ),
		parent( parent )
	{
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

		VertexCBuf.Update( gfx, tb );
		VertexCBuf.Bind( gfx );
	}

	std::string TransformationConstBuffer::GenerateUID( const Drawable& parent )
	{
		using namespace std::string_literals;
		return std::string( typeid( TransformationConstBuffer ).name() + "_"s + std::to_string((long) &parent));
	}

	std::shared_ptr<Bindable> TransformationConstBuffer::Resolve( Graphics& gfx, const Drawable& parent )
	{
		return Codex::Resolve<TransformationConstBuffer>(gfx,parent);
	}

};