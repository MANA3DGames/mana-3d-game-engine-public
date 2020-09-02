#ifndef EDITOR_GUI_LOG_WINDOW_H
#define EDITOR_GUI_LOG_WINDOW_H

#include <ImGui/imgui.h>

namespace MANA3D
{
    struct EditorLog
    {
        friend class EditorGuiLogWindow;

    private:
        ImGuiTextBuffer     Buf;
        ImGuiTextFilter     Filter;
        ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
        bool                AutoScroll;
        bool                ScrollToBottom;

        int logLineCount;


        EditorLog();

        void Clear();

        void AddLog(const char* fmt, ...) IM_FMTARGS(2);

        void Draw();
    };

    class MString;

	class EditorGuiLogWindow
	{
		friend class EditorGuiUtilitesWindow;

	private:
		EditorGuiLogWindow();
		~EditorGuiLogWindow();

		void Display();



        static EditorLog editorLog;

    public: 
        static void AddLog( const MString& log, int logType );

	};
}

#endif // !EDITOR_GUI_LOG_WINDOW_H
