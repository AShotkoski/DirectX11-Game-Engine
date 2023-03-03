#pragma once
#include "Bindable.h"
#include "DynamicCB.h"
#include "Macros.h"

namespace Binds
{
	// No common class for CB because that's a little much on the inheritance

	class PSConstantBufferEx : public Bindable
	{
	public:
		virtual void Bind( Graphics& gfx ) override;
		void Update( Graphics& gfx, const CB::Buffer& buffer );
	protected:
		PSConstantBufferEx( Graphics& gfx, const CB::Buffer& buffer, UINT slot, bool setData );
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
		UINT slot_;
	};

	class CachingPSConstantBufferEx : public PSConstantBufferEx
	{
	public:
		CachingPSConstantBufferEx( Graphics& gfx, const CB::Buffer& buffer, UINT slot, bool setData = true );
		const CB::Buffer& GetBuffer() const;
		void SetBuffer( const CB::Buffer& buffer );
		virtual void Bind( Graphics& gfx ) override;
		virtual void Accept( class TechniqueProbe& probe ) override;
	private:
		CB::Buffer buffer_;
		bool synchronized_;
	};
}