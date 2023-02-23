#pragma once
#include "ImGui/imgui.h"
#include "Loguru/loguru.hpp"

class ImGuiLog
{
public:
	ImGuiLog();
	~ImGuiLog();
	void Clear();
	void AddLog( const char* fmt, ... ) IM_FMTARGS( 2 );
	void Draw( const char* title, bool* p_open = nullptr );
private:
	static void loguru_bouncer( void* user_data, const loguru::Message& message );
private:
	ImGuiTextBuffer Buf;
	ImGuiTextFilter Filter;
	ImVector<int>   LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	bool            AutoScroll;  // Keep scrolling if already at the bottom.
};
