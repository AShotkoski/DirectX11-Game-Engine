#include "Mouse.h"

std::optional<DirectX::XMFLOAT2> Mouse::GetRawMouseMovement() const
{
	if ( rawDeltaQueue.empty() )
		return std::nullopt;
	const auto delta = rawDeltaQueue.front();
	rawDeltaQueue.pop();
	return delta;
}

void Mouse::RawInput( RAWMOUSE mouse )
{
	rawDeltaQueue.push( { (float)mouse.lLastX, (float)-mouse.lLastY } );
	TrimRawDeltaQueue();
}

void Mouse::TrimRawDeltaQueue()
{
	while ( rawDeltaQueue.size() > rawDeltaQueueMaxsize )
	{
		rawDeltaQueue.pop();
	}
}
