#include "Cube.h"
#include "Binds/BindableBaseIncludes.h"
#include "CubePrimitive.h"
#include "Util/Colors.h"
#include "Graphics/Vertex.h"
#include "Binds/BindableCodex.h"
#include "Binds/ConstantBufferEx.h"
#include "Graphics/TechniqueProbe.h"
#include "ImGui.h"

Cube::Cube(
	Graphics& gfx,
	DirectX::XMFLOAT3 size,
	DirectX::XMFLOAT3 position,
	float pitch, float yaw, float roll )
	: size( size )
	, pos( position )
	, pitch( pitch )
	, yaw( yaw )
	, roll( roll )
{
	Vert::VertexLayout vLayout;
	vLayout.Append( Vert::VertexLayout::Position_3D );
	vLayout.Append( Vert::VertexLayout::Normal );
	Vert::VertexBuffer vertBuf( std::move( vLayout ) );
	IndexedTriangleList itl = GeometricPrim::Cube::GetIndependentFaces( vertBuf );
	itl.SetNormalsIndependentFlat();
	// Set common idx buf, vert buf, and topology on parent drawable
	pVertexBuffer = Binds::VertexBuffer::Resolve( gfx, itl.vb, "Cube" );
	pIndexBuffer = Binds::IndexBuffer::Resolve( gfx, itl.indices, "Cube" );
	pTopology = Binds::Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Create Technique for phong
	Technique solidPhong("Solid Color Phong");
	{
	Step only( 0 );
	// Add binds to step needed to phong
	only.AddBind( Binds::PixelShader::Resolve( gfx, L"phong__PS.cso" ) );
	auto pVS = Binds::VertexShader::Resolve( gfx, L"phong__VS.cso" );
	auto vsbytecode = pVS->pGetBytecode();
	only.AddBind( std::move( pVS ) );
	only.AddBind( Binds::InputLayout::Resolve( gfx, vertBuf.GetLayout(), *vsbytecode ) );
	only.AddBind( std::make_shared<Binds::TransformationConstBuffer>( gfx ) );
	// Setup phong material properties
	CB::Layout cblay;
	cblay.add( CB::Float, "specularIntensity" );
	cblay.add( CB::Float, "specularPower" );
	cblay.add( CB::Float3, "color" );
	CB::Buffer cbbuf( std::move( cblay ) );
	cbbuf["specularIntensity"] = 0.4f;
	cbbuf["specularPower"] = 80.f;
	cbbuf["color"] = DirectX::XMFLOAT3{ 1.f,0.5,0.05f };
	only.AddBind( std::make_shared<Binds::CachingPSConstantBufferEx>( gfx, cbbuf, 1u ) );
	solidPhong.AddStep( std::move( only ) );
	}
	AddTechnique( std::move( solidPhong ) );

	// Mask
	Technique Outline("Outline");
	{
		{
			Step mask( 1u );
			auto pVS = Binds::VertexShader::Resolve( gfx, L"VSTransform.cso" );
			auto vsbytecode = pVS->pGetBytecode();
			mask.AddBind( std::move( pVS ) );
			mask.AddBind( Binds::InputLayout::Resolve( gfx, vertBuf.GetLayout(), *vsbytecode ) );
			mask.AddBind( std::make_shared<Binds::TransformationConstBuffer>( gfx ) );
			Outline.AddStep( std::move( mask ) );
		}
		{
			Step draw( 2u );
			auto pVS = Binds::VertexShader::Resolve( gfx, L"phong__VS.cso" );
			auto vsbytecode = pVS->pGetBytecode();
			draw.AddBind( std::move( pVS ) );
			draw.AddBind( Binds::InputLayout::Resolve( gfx, vertBuf.GetLayout(), *vsbytecode ) );
			draw.AddBind( Binds::PixelShader::Resolve( gfx, L"PSSolid.cso" ) );
			class ScalingTransformCB : public Binds::TransformationConstBuffer
			{
			public:
				ScalingTransformCB( Graphics& gfx, float scale )
					: TransformationConstBuffer(gfx )
					, scale_(scale)
				{}
				virtual void Bind( Graphics& gfx ) override
				{
					auto scaling = DirectX::XMMatrixScaling( scale_, scale_, scale_ );
					auto trans = GetBuffer( gfx );
					trans.model = trans.model * scaling;
					trans.modelViewProj = trans.modelViewProj * scaling;
					UpdateAndBind( gfx, trans );
				}
			private:
				float scale_;
			};
			draw.AddBind( std::make_shared<ScalingTransformCB>(gfx, 1.05f) );
			struct CBb
			{
				float r = 1.f;
				float g = 0.f;
				float b = 0.f;
				float pad = 0;
			} cbb;
			draw.AddBind( Binds::PixelConstantBuffer<CBb>::Resolve( gfx, cbb, "solidred", 1u ) );
			Outline.AddStep( std::move( draw ) );
		}
		AddTechnique( std::move( Outline ) );

	}
}

DirectX::XMMATRIX Cube::GetTransformationMatrix() const noexcept
{
	using namespace DirectX;
	auto scale = XMMatrixScaling( size.x, size.y, size.z );
		return scale
			* XMMatrixRotationRollPitchYaw(pitch,yaw,roll)
			* XMMatrixTranslation(pos.x,pos.y,pos.z);
}

void Cube::SpawnControlWindow( Graphics& gfx, std::string name )
{
	if ( ImGui::Begin( name.c_str() ) )
	{
		class Probing : public TechniqueProbe
		{
		public:
			virtual void OnSetTechnique() override
			{
				ImGui::Text( pTechnique->GetName().c_str() );
				bool techActive = pTechnique->Active();
				using namespace std::string_literals;
				if ( ImGui::Checkbox( ("Active##"s + std::to_string(techIdx)).c_str(), &techActive) )
				{
					pTechnique->SetActiveState( techActive );
				}
			}
			virtual bool OnVisitBuffer( CB::Buffer& cb ) const override
			{
				namespace dx = DirectX;
				bool changed = false;
				auto l_changed = [&changed]( bool b )
				{
					if ( b )
					{
						changed = changed || true;
					}
				};
				if ( auto e = cb["color"]; e.Exists() )
				{
					l_changed( ImGui::ColorEdit3( "color", &e ) );
				}
				if ( auto e = cb["specularIntensity"]; e.Exists() )
				{
					l_changed( ImGui::DragFloat( "Specular Intensity", &e, 0.01f ) );
				}
				if ( auto e = cb["specularPower"]; e.Exists() )
				{
					l_changed( ImGui::DragFloat( "Specular Power", &e, 0.25f ) );
				}
				return changed;
			}
		} probe;

		Accept( probe );
	}
	ImGui::End();
}
