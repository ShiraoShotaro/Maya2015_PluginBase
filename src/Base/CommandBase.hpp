#pragma once
#ifndef _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_
#define _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_

#include <maya/MString.h>
#include <maya/MPxCommand.h>
#include <vector>
#include <memory>

class MFnPlugin;

namespace mpb {

/// @brief The base class for command.
///
/// When you implement your own command, you have to inherit this class.
///
///
class CommandBase : public MPxCommand {
public:

	const MString command_;		///< Command string initialized by constructor.

	const bool is_undoable_;	///< Is undoable.

	CommandBase(void) = delete;
	
	/// @brief Constructor
	///
	/// @param [in] command Command string. 
	CommandBase(const MString & command, const bool is_undoable) noexcept;

	
	/// @brief Destructor
	///
	/// Currently, do nothing.
	virtual ~CommandBase(void);


	/// @brief do It function.
	///
	/// You can override this function.
	/// this function is doing nothing.
	///
	/// @param [in] args command arguments.
	///
	/// @return result of processing.
	virtual MStatus doIt(const MArgList& args) override;

	
	/// @brief redo It function
	///
	/// You can override this function.
	/// this function is doing nothing.
	///
	/// @return result of processing.
	virtual MStatus redoIt() override;
	

	/// @brief undo It function
	///
	/// You can override this function.
	/// this function is doing nothing.
	///
	/// @return result of processing.
	virtual MStatus undoIt() override;
	

	/// @brief redo It function
	///
	/// You can override this function.
	/// This function returns this->is_undoable_.
	///
	/// @return result of processing.
	virtual bool isUndoable() const override;


	/// @brief command addition.
	///
	/// YOU HAVE TO EDIT THIS FUNCTION AT main.cpp .
	///
	/// @param [in,out] plugin MFnPlugin instance.
	///
	/// @retval MStatus::kSuccess Succeed.
	/// @retval else Failed to add nodes.
	static MStatus addCommands(void);


	/// @brief remove commands.
	///
	/// You must not call this function.
	/// This function is for internal.
	///
	/// @param [in,out] plugin MFnPlugin instance.
	///
	/// @retval MStatus::kSuccess Succeed.
	/// @retval else Failed to add nodes.
	static MStatus removeCommands(MFnPlugin & plugin);

	static void _setMFnPluginPtr(MFnPlugin * plugin);

private:

	static MFnPlugin * plugin_;
	static std::vector<std::unique_ptr<CommandBase>> instances_;

	template <class _INHERIT_FROM_COMMANDBASE, class ...Args>
	static void addCommand(Args... args);
	static void _addCommand(void * (*creator)(), std::unique_ptr<CommandBase> && command);

};
template<class _INHERIT_FROM_COMMANDBASE, class ...Args>
inline void CommandBase::addCommand(Args ...args)
{
}
// end of CommandBase
}; // end of mpb
#endif // end of _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_