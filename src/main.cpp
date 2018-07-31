#include <maya/MFnPlugin.h>
#include "Base/NodeBase.hpp"
#include "Base/CommandBase.hpp"

//*** INCLUDE HEADERS ***

//***********************

namespace {
constexpr char kProjectName[] = "__PROJECT_NAME__";
constexpr char kVersion[] = "0.1";
}

MStatus mpb::NodeBase::addNodes(void)
{
	MStatus ret = MStatus::kSuccess;

	//ADD NODES

	//addNode<HOGEHOGE>();

	return ret;
}

MStatus mpb::CommandBase::addCommands(void)
{
	MStatus ret = MStatus::kSuccess;

	//ADD COMMANDS

	//addCommand<HOGEHOGE>();

	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// DO NOT TOUCH BELOW CODES.
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

MFnPlugin * mpb::NodeBase::plugin_ = nullptr;
MFnPlugin * mpb::CommandBase::plugin_ = nullptr;

MStatus initializePlugin(MObject obj) {
	std::unique_ptr<MFnPlugin> plugin = std::make_unique<MFnPlugin>(obj, "Autodesk", "2015 update2");

	MStatus stat = MStatus::kSuccess;

#ifdef _DEBUG
	std::cout.rdbuf(std::cerr.rdbuf());
	std::cout << "[Development Mode] Start to load " << kProjectName << " plug-in." << std::endl;
#endif

	mpb::NodeBase::_setMFnPluginPtr(plugin.get());
	mpb::CommandBase::_setMFnPluginPtr(plugin.get());

	do {
		std::cout << "add Nodes." << std::endl;
		if ((stat = mpb::NodeBase::addNodes()) != MStatus::kSuccess) break;

		std::cout << "add Commands." << std::endl;
		if ((stat = mpb::CommandBase::addCommands()) != MStatus::kSuccess) break;

		//ADD HERE

	} while (false);

	if (stat == MStatus::kSuccess) {
#ifdef _DEBUG
		std::cerr << "[Development Mode]" << kProjectName << " - version " << kVersion << std::endl;
#else
		std::cerr << kProjectName << " - version " << kVersion << std::endl;
#endif
	}
	else std::cerr << "Failed to load " << kProjectName << " plug-in." << std::endl;
	return stat;
}

MStatus uninitializePlugin(MObject obj) {
	MFnPlugin plugin(obj);
	MStatus stat = MStatus::kSuccess;

	std::cout << "[Development Mode] Start to uninitialize " << kProjectName << " plug-in." << std::endl;

	do {

		std::cout << "remove Nodes." << std::endl;
		if ((stat = mpb::NodeBase::removeNodes(plugin)) != MStatus::kSuccess) break;

	} while (false);

	return stat;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NODE
MStatus mpb::NodeBase::removeNodes(MFnPlugin & plugin)
{
	MStatus ret = MStatus::kSuccess;
	for (auto ptr = NodeBase::instances_.begin(); ptr != NodeBase::instances_.end(); ++ptr) {
		if ((ret = plugin.deregisterNode((*ptr)->id_)) != MStatus::kSuccess) {
			std::cerr << "Failed to deregister node. NODE : " << (*ptr)->name_ << std::endl;
			break;
		}
	}
	return ret;
}

void mpb::NodeBase::_setMFnPluginPtr(MFnPlugin * plugin) { NodeBase::plugin_ = plugin; }
void mpb::NodeBase::_addNode(void *(*creator)(), MStatus(*initialize)(), std::unique_ptr<NodeBase> && node)
{ NodeBase::plugin_->registerNode(node->name_, node->id_, creator, initialize, node->type_, (node->own_classification_ ? &node->classification_ : nullptr)); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND
MStatus mpb::CommandBase::removeCommands(MFnPlugin & plugin)
{
	MStatus ret = MStatus::kSuccess;
	for (auto ptr = CommandBase::instances_.begin(); ptr != CommandBase::instances_.end(); ++ptr) {
		if ((ret = plugin.deregisterCommand((*ptr)->command_)) != MStatus::kSuccess) {
			std::cerr << "Failed to deregister command. COMMAND : " << (*ptr)->command_ << std::endl;
			break;
		}
	}
	return ret;
}

void mpb::CommandBase::_setMFnPluginPtr(MFnPlugin * plugin) { CommandBase::plugin_ = plugin; }
void mpb::CommandBase::_addCommand(void *(*creator)(), std::unique_ptr<CommandBase>&& command)
{ CommandBase::plugin_->registerCommand(command->command_, creator); }