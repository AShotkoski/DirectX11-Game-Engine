#pragma once
#include <DirectXMath.h>
#include <memory>

class Graphics;
class Drawable;

class BaseEntity
{
public:
	void UpdateModel() const;
protected:
	BaseEntity(Graphics& gfx, std::unique_ptr<Drawable> model,
		DirectX::XMFLOAT3 pos, float width = 1.f, float height = 1.f, float rotation = 0.f);
	BaseEntity(BaseEntity&& src) noexcept;
	BaseEntity(const BaseEntity&) = delete;
	BaseEntity& operator=(const BaseEntity&) = delete;
	BaseEntity& operator=(BaseEntity&& src) = delete;

	virtual ~BaseEntity() = default;
protected:
	DirectX::XMFLOAT3 position;
	float rotation;
	float width;
	float height;
	std::unique_ptr<Drawable> pModel;
};