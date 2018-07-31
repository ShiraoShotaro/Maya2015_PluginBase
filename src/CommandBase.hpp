#pragma once
#ifndef _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_
#define _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_

#include <maya/MString.h>

namespace mpb {

/// @brief The base class for command.
///
/// When you implement your own command, you have to inherit this class.
///
///
class CommandBase {
public:

	const MString command_;	///< Command string initialized by constructor.

	/// @brief Constructor
	///
	/// @param [in] command Command string. 
	CommandBase(const MString & command) noexcept;

	/// @brief Destructor
	///
	/// Currently, do nothing.
	virtual ~CommandBase(void);

	/// @brief Instance creator function.
	///
	/// You can override this function in inheritted class.
	/// And create own instance, then return the adress.
	///
	/// If default function is used, this function returns null then automatecally create ONLY the instance.
	///
	/// @return When you inherit this function, return the address of the instance. Default returns nullptr.
	virtual void * creator(void);

private:
	static void addCommands(void);
}; // end of CommandBase
}; // end of mpb
#endif // end of _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_