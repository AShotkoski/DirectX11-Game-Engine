#pragma once
#include "Win.h"
#include <DirectXMath.h>
#include <queue>
#include <optional>

class Mouse
{
	friend class Window;
public:
	Mouse() = default;
	std::optional<DirectX::XMFLOAT2> GetRawMouseMovement() const;
private:
	// Called by window to supply raw mouse data
	void RawInput( RAWMOUSE mouse );

	void TrimRawDeltaQueue();
private:
	static constexpr size_t rawDeltaQueueMaxsize = 16u;
	mutable std::queue<DirectX::XMFLOAT2> rawDeltaQueue;
};