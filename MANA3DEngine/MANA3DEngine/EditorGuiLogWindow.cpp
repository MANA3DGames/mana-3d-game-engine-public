#include "EditorGuiLogWindow.h"
#include "MString.h"
using namespace MANA3D;


EditorLog::EditorLog()
{
    AutoScroll = true;
    ScrollToBottom = false;
    Clear();

    logLineCount = 0;
}

void EditorLog::Clear()
{
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);

    logLineCount = 0;
}

void EditorLog::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{
    logLineCount++;

    int old_size = Buf.size();
    va_list args;
    va_start(args, fmt);
    Buf.appendfv(fmt, args);
    va_end(args);
    for (int new_size = Buf.size(); old_size < new_size; old_size++)
        if (Buf[old_size] == '\n')
            LineOffsets.push_back(old_size + 1);
    if (AutoScroll)
        ScrollToBottom = true;
}

void EditorLog::Draw()
{
    ImGui::Spacing();
	ImGui::Text( "Logs Count: %i", logLineCount );
	ImGui::Separator();

    ImGui::BeginChild( "MANA3DEditorConsole" );

    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (clear)
        Clear();
    if (copy)
        ImGui::LogToClipboard();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    const char* buf = Buf.begin();
    const char* buf_end = Buf.end();
    if (Filter.IsActive())
    {
        // In this example we don't use the clipper when Filter is enabled.
        // This is because we don't have a random access on the result on our filter.
        // A real application processing logs with ten of thousands of entries may want to store the result of search/filter.
        // especially if the filtering function is not trivial (e.g. reg-exp).
        for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
        {
            const char* line_start = buf + LineOffsets[line_no];
            const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
            if (Filter.PassFilter(line_start, line_end))
                ImGui::TextUnformatted(line_start, line_end);
        }
    }
    else
    {
        // The simplest and easy way to display the entire buffer:
        //   ImGui::TextUnformatted(buf_begin, buf_end);
        // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
        // Here we instead demonstrate using the clipper to only process lines that are within the visible area.
        // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
        // Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
        // both of which we can handle since we an array pointing to the beginning of each line of text.
        // When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
        // Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
        ImGuiListClipper clipper;
        clipper.Begin( LineOffsets.Size );
        while ( clipper.Step() )
        {
            for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;

                ImVec4 color;
                if ( strstr( line_start, "[error]" ) ) 
                    color = ImVec4( 1.0f, 0.4f, 0.4f, 1.0f );
                else if ( strstr( line_start, "[warn]" ) ) 
                    color = ImVec4( 1.0f, 1.0f, 0.5f, 1.0f );
                else  
                    color = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );

                ImGui::PushStyleColor( ImGuiCol_Text, color ); 

                ImGui::TextUnformatted( line_start, line_end );

                ImGui::PopStyleColor();
            }
        }
        clipper.End();
    }
    ImGui::PopStyleVar();

    if ( ScrollToBottom )
        ImGui::SetScrollHereY( 1.0f );
    ScrollToBottom = false;
    ImGui::EndChild();

    ImGui::EndChild();
}








EditorGuiLogWindow::EditorGuiLogWindow()
{
}

EditorGuiLogWindow::~EditorGuiLogWindow() {}


void EditorGuiLogWindow::Display()
{
	editorLog.Draw();
}



EditorLog EditorGuiLogWindow::editorLog;

void EditorGuiLogWindow::AddLog( const MString& log, int logType )
{
	const char* categories[3] = { "info", "warn", "error" };
	editorLog.AddLog( "[%05d] [%s] %.1f : '%s'\n", ImGui::GetFrameCount(), categories[logType % IM_ARRAYSIZE(categories)], ImGui::GetTime(), log.GetCStr() );
}