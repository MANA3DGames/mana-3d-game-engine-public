#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <functional>
#include <list>

namespace MANA3D
{
	struct Command
	{
		std::function<void()> cmd;
		std::function<void()> undo;

		Command( std::function<void()> cmd, std::function<void()> undo );
		void Undo();
		void Redo();
	};

	class CommandManager
	{
		friend class EditorGuiManager;

	private:

		static std::list<Command> undoCommands;
		static std::list<Command> redoCommands;

		static void UndoCommand();
		static void RedoCommand();

	public:
		
		// Registers an undonable command.
		// * cmd: actual command, will be use as redo as well.
		// * undo: reverse cmd.
		static void RegisterCommand( std::function<void()> cmd, std::function<void()> undo );

	};
}

#endif // !COMMAND_MANAGER_H

