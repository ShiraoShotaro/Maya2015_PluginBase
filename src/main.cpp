#include "NodeBase.hpp"
#include "CommandBase.hpp"

//*** INCLUDE HEADERS ***

//***********************

namespace {
constexpr char kProjectName[] = "__PROJECT_NAME__";
constexpr char kVersion[] = "0.1";
}

MStatus mpb::NodeBase::addNodes(MFnPlugin & plugin)
{
	MStatus ret = MStatus::kSuccess;


	return ret;
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// DO NOT TOUCH BELOW CODES.
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::unique_ptr<mpb::NodeBase>> mpb::NodeBase::instances_;

MStatus initializePlugin(MObject obj) {
	MFnPlugin plugin(obj, "Autodesk", "2015 update2");
	MStatus stat = MStatus::kSuccess;

#ifdef _DEBUG
	std::cout.rdbuf(std::cerr.rdbuf());
	std::cout << "[Development Mode] Start to load " << kProjectName << " plug-in." << std::endl;
#endif

	do {
		std::cout << "add Nodes." << std::endl;
		if ((stat = mpb::NodeBase::addNodes(plugin)) != MStatus::kSuccess) break;

		// ADD HERE

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
