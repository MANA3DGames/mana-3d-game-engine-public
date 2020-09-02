#include "CommandManager.h"
#include "Debug.h"
using namespace MANA3D;

using std::list;
using std::function;



Command::Command( std::function<void()> cmd, std::function<void()> undo ) : cmd( cmd ), undo( undo )
{
}
void Command::Undo()
{
	undo();
}
void Command::Redo()
{
	cmd();
}





list<Command> CommandManager::undoCommands;
list<Command> CommandManager::redoCommands;


void CommandManager::UndoCommand()
{
	if ( undoCommands.size() > 0 )
	{
		auto cmd = undoCommands.front();
		undoCommands.pop_front();
		redoCommands.push_front( cmd );
		cmd.Undo();
	}
	else
		Debug::LogWarning( "Commands stack is empty!" );
}

void CommandManager::RedoCommand()
{
	if ( redoCommands.size() > 0 )
	{
		auto cmd = redoCommands.front();
		redoCommands.pop_front();
		undoCommands.push_front( cmd );
		cmd.Redo();
	}
	else
		Debug::LogWarning( "Commands stack is empty!" );
}


void CommandManager::RegisterCommand( function<void()> cmd, function<void()> undo )
{
	if ( cmd == nullptr )
		return;

	cmd();

	// Check if there is 10 or more commands in the stack. 
	if ( undoCommands.size() >= 10 )
	{
		// Remove oldest command in the list.
		undoCommands.pop_back();
	}

	undoCommands.push_front( Command( cmd, undo ) );
}
