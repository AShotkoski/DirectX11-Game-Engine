#include "Material.h"
#include "Technique.h"
#include "Binds/BindableBaseIncludes.h"
#include "Binds/Blender.h"
#include "Util/Colors.h"
#include "Util/GeneralUtilities.h"

Material::Material( Graphics& gfx, const aiMaterial& aiMat, std::filesystem::path modelPath )
{
	// Dynamically compose dynamic CB from aiMaterial, as well as dynamically compose a vertex layout
	// (but don't fill the buffer yet)
	
	using elem = Vert::VertexLayout::ElementType;
	std::string rootPath = modelPath.parent_path().string() + std::string("\\");

	// Step 1: Phong technique creation
	{
		Technique Phong{ "Phong" };
		{
			Step only( 0 );
			bool hasDiffuseMap = false;
			bool hasAlpha = false;
			bool hasNormalMap = false;
			aiString texFilename;
			std::string shaderName = "phong_";

			// Common
			vertlayout_.Append( elem::Position_3D );
			vertlayout_.Append( elem::Normal );
			CB::Layout cbLayout;

			// Check for texture
			if ( aiMat.GetTexture( aiTextureType_DIFFUSE, 0, &texFilename ) == aiReturn_SUCCESS )
			{
				hasDiffuseMap = true;
				const auto texPath = rootPath + texFilename.C_Str();
				shaderName += "diff";
				vertlayout_.Append( elem::TexCoordUV );
				auto pTex = Binds::Texture::Resolve( gfx, texPath );
				hasAlpha = pTex->hasAlpha();
				only.AddBind( std::move( pTex ) );
			}
			else // if we don't have a texture, we need material color
			{
				cbLayout.add( CB::Float3, "materialColor" );
			}
			
			// Check for Normal Map. TODO: add support for different normal map semantics
			if ( aiMat.GetTexture( aiTextureType_HEIGHT, 0, &texFilename ) == aiReturn_SUCCESS )
			{
				hasNormalMap = true;
				const auto texPath = rootPath + texFilename.C_Str();
				shaderName += "norm";
				vertlayout_.Try_append( elem::TexCoordUV );
				vertlayout_.Append( elem::Tangent );
				vertlayout_.Append( elem::Bitangent );
				only.AddBind( Binds::Texture::Resolve( gfx, texPath, 1u ) );
				// Todo add cb param for normal amount
			}
			
			// Specular
			{
				//TODO spec map
				cbLayout.add( CB::Float, "specularIntensity" );
				cbLayout.add( CB::Float, "specularPower" );
			}


			// Create CB
			CB::Buffer cbBuf( std::move( cbLayout ) );

			// Specular assignments
			{
				float flBuf = 0.f;
				aiColor3D colBuf;
				if ( aiMat.Get( AI_MATKEY_SHININESS, flBuf ) == aiReturn_SUCCESS )
				{
					cbBuf["specularPower"] = flBuf;
				}
				if ( aiMat.Get( AI_MATKEY_COLOR_SPECULAR, colBuf ) == aiReturn_SUCCESS )
				{
					// Average color of specular color is our intensity, TODO actual support for multi col channels
					Color specCol = *reinterpret_cast<Color*>( &colBuf );
					flBuf = ( specCol.el[0] + specCol.el[1] + specCol.el[2] ) / 3.f;
					cbBuf["specularIntensity"] = flBuf;
				}
			}

			if ( hasDiffuseMap || hasNormalMap )
			{
				only.AddBind( Binds::Sampler::Resolve( gfx ) );
			}

			// Common
			only.AddBind( Binds::Blender::Resolve( gfx ) );// never blend yet
			only.AddBind( std::make_shared<Binds::TransformationConstBuffer>( gfx ) );
			// Add the material CB to the step
			only.AddBind( std::make_shared<Binds::CachingPSConstantBufferEx>( gfx, cbBuf, 1u ) );
			// Add shaders
			only.AddBind( Binds::PixelShader::Resolve( gfx, Util::StringToWString( shaderName + std::string( "_PS.cso" ) ) ) );
			auto pVS = Binds::VertexShader::Resolve( gfx, Util::StringToWString( shaderName + std::string( "_VS.cso" ) ) );
			auto vsByteCode = pVS->pGetBytecode();
			only.AddBind( Binds::InputLayout::Resolve( gfx, vertlayout_, *vsByteCode ) );
			only.AddBind( std::move( pVS ) );
			tag_ += shaderName;

			// add step to tech
			Phong.AddStep( std::move( only ) );
			techniques_.push_back( std::move( Phong ) );
		}// step only
	} // tech phong
}

const std::vector<Technique>& Material::GetTechniques() const
{
	return techniques_;
}

const Vert::VertexLayout& Material::GetVertexLayout() const
{
	return vertlayout_;
}

const std::string& Material::GetTag() const
{
	return tag_;
}
