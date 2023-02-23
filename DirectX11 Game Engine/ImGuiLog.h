#pragma once
#include "ImGui/imgui.h"
#include "Loguru/loguru.hpp"
#include <vector>

class ImGuiLog
{
public:
	ImGuiLog();
	~ImGuiLog();
	void Clear();
	void AddLog( const char* fmt, ... ) IM_FMTARGS( 2 );
	void Draw( const char* title, bool* p_open = nullptr );
private:
	void change_loguru_verbosity( loguru::Verbosity verbosity );
	static void loguru_bouncer( void* user_data, const loguru::Message& message );
private:
	ImGuiTextBuffer Buf;
	ImGuiTextFilter Filter;
	ImVector<int>   LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	bool            AutoScroll;  // Keep scrolling if already at the bottom.
	std::vector<loguru::Verbosity> verbosities;
	int verbosity_curr_idx;
};
