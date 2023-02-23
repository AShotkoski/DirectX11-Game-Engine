#include "ImGuiLog.h"
#include <sstream>

ImGuiLog::ImGuiLog()
	: verbosity_curr_idx(0)
{
	AutoScroll = true;
	Clear();
	// Integrate loguru callback
	loguru::add_callback( "imguilog callback", loguru_bouncer, this, loguru::Verbosity_MAX );
	// Add verbosites for selection
	verbosities.push_back( loguru::Verbosity_MAX );
	verbosities.push_back( loguru::Verbosity_INFO  );
	verbosities.push_back( loguru::Verbosity_WARNING  );
	verbosities.push_back( loguru::Verbosity_ERROR  );

	// Clunky way to ensure we start at the correct verbosity level
	change_loguru_verbosity( verbosities[verbosity_curr_idx] );
}

ImGuiLog::~ImGuiLog()
{
	loguru::remove_callback( "imguilog callback" );
}

void ImGuiLog::Clear()
{
	Buf.clear();
	LineOffsets.clear();
	LineOffsets.push_back( 0 );
}

void ImGuiLog::Draw( const char* title, bool* p_open )
{
	if ( !ImGui::Begin( title, p_open ) )
	{
		ImGui::End();
		return;
	}

	// Options menu
	if ( ImGui::BeginPopup( "Options" ) )
	{
		ImGui::Checkbox( "Auto-scroll", &AutoScroll );
		auto toname = [](loguru::Verbosity verb)->const char* 
		{
			if ( verb == loguru::Verbosity_MAX )
				return "MAX";
			else
				return loguru::get_verbosity_name( verb );
		};
		if ( ImGui::BeginCombo( "Verbosity", toname(verbosities[verbosity_curr_idx])) )
		{
			for ( int n = 0; n < verbosities.size(); n++ )
			{
				const bool is_selected = ( verbosity_curr_idx == n );
				if ( ImGui::Selectable( toname( verbosities[n] ), is_selected ) )
				{
					verbosity_curr_idx = n;
					change_loguru_verbosity( verbosities[verbosity_curr_idx] );
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if ( is_selected )
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::EndPopup();
	}

	// Main window
	if ( ImGui::Button( "Options" ) )
		ImGui::OpenPopup( "Options" );
	ImGui::SameLine();
	bool clear = ImGui::Button( "Clear" );
	ImGui::SameLine();
	Filter.Draw( "Filter", -100.0f );

	ImGui::Separator();

	if ( ImGui::BeginChild(
		"scrolling",
		ImVec2( 0, 0 ),
		false,
		ImGuiWindowFlags_HorizontalScrollbar ) )
	{
		if ( clear )
			Clear();

		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
		const char* buf = Buf.begin();
		const char* buf_end = Buf.end();
		if ( Filter.IsActive() )
		{
			// In this example we don't use the clipper when Filter is enabled.
			// This is because we don't have random access to the result of our filter.
			// A real application processing logs with ten of thousands of entries may want to
			// store the result of search/filter.. especially if the filtering function is not
			// trivial (e.g. reg-exp).
			for ( int line_no = 0; line_no < LineOffsets.Size; line_no++ )
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = ( line_no + 1 < LineOffsets.Size ) ?
					( buf + LineOffsets[line_no + 1] - 1 ) :
					buf_end;
				if ( Filter.PassFilter( line_start, line_end ) )
					ImGui::TextUnformatted( line_start, line_end );
			}
		}
		else
		{
			// The simplest and easy way to display the entire buffer:
			//   ImGui::TextUnformatted(buf_begin, buf_end);
			// And it'll just work. TextUnformatted() has specialization for large blob of text
			// and will fast-forward to skip non-visible lines. Here we instead demonstrate
			// using the clipper to only process lines that are within the visible area. If you
			// have tens of thousands of items and their processing cost is non-negligible,
			// coarse clipping them on your side is recommended. Using ImGuiListClipper requires
			// - A) random access into your data
			// - B) items all being the  same height,
			// both of which we can handle since we have an array pointing to the beginning of
			// each line of text. When using the filter (in the block of code above) we don't
			// have random access into the data to display anymore, which is why we don't use
			// the clipper. Storing or skimming through the search result would make it possible
			// (and would be recommended if you want to search through tens of thousands of
			// entries).
			ImGuiListClipper clipper;
			clipper.Begin( LineOffsets.Size );
			while ( clipper.Step() )
			{
				for ( int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd;
					  line_no++ )
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = ( line_no + 1 < LineOffsets.Size ) ?
						( buf + LineOffsets[line_no + 1] - 1 ) :
						buf_end;
					ImGui::TextUnformatted( line_start, line_end );
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		// Keep up at the bottom of the scroll region if we were already at the bottom at the
		// beginning of the frame. Using a scrollbar or mouse-wheel will take away from the
		// bottom edge.
		if ( AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() )
			ImGui::SetScrollHereY( 1.0f );
	}
	ImGui::EndChild();
	ImGui::End();
}

void ImGuiLog::change_loguru_verbosity( loguru::Verbosity verbosity )
{
	loguru::remove_callback( "imguilog callback" );
	loguru::add_callback( "imguilog callback", loguru_bouncer, this, verbosity );
}

void ImGuiLog::loguru_bouncer( void* user_data, const loguru::Message& message )
{
	std::ostringstream oss;
	
	oss << loguru::get_verbosity_name(message.verbosity) << " --- " << message.message << '\n';
	reinterpret_cast<ImGuiLog*>( user_data )->AddLog( oss.str().c_str() );
}

void ImGuiLog::AddLog( const char* fmt, ... )
{
	int     old_size = Buf.size();
	va_list args;
	va_start( args, fmt );
	Buf.appendfv( fmt, args );
	va_end( args );
	for ( int new_size = Buf.size(); old_size < new_size; old_size++ )
		if ( Buf[old_size] == '\n' )
			LineOffsets.push_back( old_size + 1 );
}