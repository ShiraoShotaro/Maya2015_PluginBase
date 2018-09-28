#include "base/NodeBase.hpp"
#include "base/CommandBase.hpp"
#include "base/TranslatorBase.hpp"
#include "exception/MStatusException.hpp"
#include <maya/MFnPlugin.h>

//*** INCLUDE HEADERS ***

//***********************

namespace {
constexpr char kProjectName[] = __PROJECT_NAME;
constexpr char kVersion[] = "0.1";
}

MStatus mpb::NodeBase::addNodes(void) throw(mpb::MStatusException)
{
	MStatus ret = MStatus::kSuccess;

	//ADD NODES

	//addNode<HOGEHOGE>();

	return ret;
}

MStatus mpb::CommandBase::addCommands(void) throw(mpb::MStatusException)
{
	MStatus ret = MStatus::kSuccess;

	//ADD COMMANDS

	//addCommand<HOGEHOGE>();

	return ret;
}

MStatus mpb::TranslatorBase::addTranslators(void) throw(mpb::MStatusException)
{
	MStatus ret = MStatus::kSuccess;

	//ADD TRANSLATORS

	//addTranslator<HOGEHOGE>();

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
MFnPlugin * mpb::TranslatorBase::plugin_ = nullptr;

// プラグイン初期化関数
MStatus initializePlugin(MObject obj) {
	std::unique_ptr<MFnPlugin> plugin = std::make_unique<MFnPlugin>(obj, "Autodesk", "2015 update2");

	MStatus stat = MStatus::kSuccess;

	std::cerr << "* " << kProjectName << " plug-in version " << kVersion << std::endl;

#ifdef _DEBUG
	std::cout.rdbuf(std::cerr.rdbuf());
	std::cout << "- [NOTICE] This plug-in is builded in development mode." << kVersion << std::endl;
#endif

	mpb::NodeBase::_setMFnPluginPtr(plugin.get());
	mpb::CommandBase::_setMFnPluginPtr(plugin.get());
	mpb::TranslatorBase::_setMFnPluginPtr(plugin.get());

	try {
		std::cout << "- add Nodes." << std::endl;
		mpb::NodeBase::addNodes();

		std::cout << "- add Commands." << std::endl;
		mpb::CommandBase::addCommands();
		
		std::cout << "- add Translator." << std::endl;
		mpb::TranslatorBase::addTranslators();


		// ALL Succeed!!
		std::cout << "- Completed initializing successfully." << std::endl;
	}
	catch (mpb::MStatusException e) {
		std::cerr << e << std::endl;
		std::cerr << "Failed to load " << kProjectName << " plug-in." << std::endl;
		stat = e;
	}

	return stat;
}

// プラグイン除去関数
MStatus uninitializePlugin(MObject obj) {
	MFnPlugin plugin(obj);
	MStatus stat = MStatus::kSuccess;

	std::cout << "* [NOTICE] Start to uninitialize " << kProjectName << " plug-in." << std::endl;

	do {

		std::cout << "- remove Nodes." << std::endl;
		if ((stat = mpb::NodeBase::removeNodes(plugin)) != MStatus::kSuccess) break;

		std::cout << "- remove Commands." << std::endl;
		if ((stat = mpb::CommandBase::removeCommands(plugin)) != MStatus::kSuccess) break;

		std::cout << "- remove Translators." << std::endl;
		if ((stat = mpb::TranslatorBase::removeTranslators(plugin)) != MStatus::kSuccess) break;

	} while (false);

	return stat;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NODE
MStatus mpb::NodeBase::removeNodes(MFnPlugin & plugin)
{
	MStatus ret = MStatus::kSuccess;
	for (auto ptr = NodeBase::instances_.begin(); ptr != NodeBase::instances_.end(); ++ptr) {
		if ((ret = plugin.deregisterNode((*ptr)->id_)) == MStatus::kSuccess) {
			std::cout << "-- deregistered " << (*ptr)->name_ << std::endl;
		}
		else {
			std::cerr << "-- Failed to deregister node. NODE : " << (*ptr)->name_ << std::endl;
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
		if ((ret = plugin.deregisterCommand((*ptr)->command_)) == MStatus::kSuccess) {
			std::cout << "-- deregistered " << (*ptr)->command_ << std::endl;
		}else{
			std::cerr << "Failed to deregister command. COMMAND : " << (*ptr)->command_ << std::endl;
			break;
		}
	}
	return ret;
}

void mpb::CommandBase::_setMFnPluginPtr(MFnPlugin * plugin) { CommandBase::plugin_ = plugin; }
void mpb::CommandBase::_addCommand(void *(*creator)(), std::unique_ptr<CommandBase>&& command)
{ CommandBase::plugin_->registerCommand(command->command_, creator); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TRANSLATOR
MStatus mpb::TranslatorBase::removeTranslators(MFnPlugin & plugin)
{
	MStatus ret = MStatus::kSuccess;
	for (auto ptr = TranslatorBase::instances_.begin(); ptr != TranslatorBase::instances_.end(); ++ptr) {
		if ((ret = plugin.deregisterFileTranslator((*ptr)->name_)) == MStatus::kSuccess) {
			std::cout << "-- deregistered " << (*ptr)->name_ << std::endl;
		}
		else {
			std::cerr << "Failed to deregister command. COMMAND : " << (*ptr)->name_ << std::endl;
			break;
		}
	}
	return ret;
}

void mpb::TranslatorBase::_setMFnPluginPtr(MFnPlugin * plugin) { TranslatorBase::plugin_ = plugin; }
void mpb::TranslatorBase::_addTranslator(void * (*creator)(), std::unique_ptr<TranslatorBase> && translator)
{ TranslatorBase::plugin_->registerFileTranslator(translator->name_, nullptr, creator); }
