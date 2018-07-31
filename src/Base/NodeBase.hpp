#pragma once
#ifndef _MAYA_PLUGIN_BASE_NODE_BASE_HPP_
#define _MAYA_PLUGIN_BASE_NODE_BASE_HPP_

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MPxNode.h>
#include <vector>
#include <memory>

class MFnPlugin;

namespace mpb {

/// @brief The base class for command.
///
/// When you implement your own command, you have to inherit this class.
///
///
class NodeBase : public MPxNode {
public:

	const MString name_;			///< Node name initialized by constructor.

	const MTypeId id_;				///< Node id.

	const MPxNode::Type type_;		///< Node type.

	const MString classification_;	///< Custom classification.
	

	/// @brief Constructor
	///
	/// @param [in] id Node ID. Do Not Overlap each others.
	/// @param [in] name Node name.
	/// @param [in] type Node type. Default is kDependNode.
	NodeBase(const MTypeId id, const MString & name, const MPxNode::Type type = MPxNode::Type::kDependNode) noexcept;


	/// @brief Constructor
	///
	/// More information for "classification", see this page
	/// [http://download.autodesk.com/us/maya/2010help/API/class_m_fn_plugin.html#7df9f8e677ea8c71e33016361109506a]
	///
	/// @param [in] id Node ID. Do Not Overlap each others.
	/// @param [in] name Node name.
	/// @param [in] classification Custom classification.
	/// @param [in] type Node type. Default is kDependNode.
	NodeBase(const MTypeId id, const MString & name, const MString & classification, const MPxNode::Type type = MPxNode::Type::kDependNode) noexcept;


	/// @brief Destructor
	///
	/// Currently, do nothing.
	virtual ~NodeBase(void);
	

	/// @brief node addition.
	///
	/// YOU HAVE TO EDIT THIS FUNCTION AT main.cpp .
	///
	/// @retval MStatus::kSuccess Succeed.
	/// @retval else Failed to add nodes.
	static MStatus addNodes(void);


	/// @brief remove nodes.
	///
	/// You must not call this function.
	/// This function is for internal.
	///
	/// @param [in,out] plugin MFnPlugin instance.
	///
	/// @retval MStatus::kSuccess Succeed.
	/// @retval else Failed to add nodes.
	static MStatus removeNodes(MFnPlugin & plugin);

	static void _setMFnPluginPtr(MFnPlugin * plugin);

private:


	const bool own_classification_;


	static MFnPlugin * plugin_;
	static std::vector<std::unique_ptr<NodeBase>> instances_;

	template <class _INHERIT_FROM_NODEBASE, class ...Args>
	static void addNode(Args... args);
	static void _addNode(void * (*creator)(), MStatus(*initialize)(), std::unique_ptr<NodeBase> && node);

};


template<class _INHERIT_FROM_NODEBASE, class ...Args>
inline void NodeBase::addNode(Args ...args){
	NodeBase::_addNode(&_INHERIT_FROM_NODEBASE::create, &_INHERIT_FROM_NODEBASE::initialize, std::make_unique<_INHERIT_FROM_NODEBASE>(args));
}

// end of CommandBase
}; // end of mpb
#endif // end of _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_