#pragma once
#ifndef _MAYA_PLUGIN_BASE_NODE_BASE_HPP_
#define _MAYA_PLUGIN_BASE_NODE_BASE_HPP_

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxNode.h>
#include <type_traits>
#include <vector>
#include <memory>

namespace mpb {

/// @brief The base class for command.
///
/// When you implement your own command, you have to inherit this class.
///
///
class NodeBase {
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


	/// @brief Instance creator function.
	///
	/// You must override this function in inheritted class.
	/// And create own instance, then return the adress.
	///
	/// @return When you inherit this function, return the address of the instance. Default returns nullptr.
	virtual void * creator(void) = 0;


	/// @brief Initializer
	///
	/// You can override this function in inheritted class.
	/// And initialize node's attributes.
	///
	/// If default function is used, do nothing and return MStatus::Success
	///
	/// @return Initializing status.
	virtual MStatus initialize(void);
	

	/// @brief node addition.
	///
	/// YOU HAVE TO EDIT THIS FUNCTION AT main.cpp .
	///
	/// @param [in,out] plugin MFnPlugin instance.
	///
	/// @retval MStatus::kSuccess Succeed.
	/// @retval else Failed to add nodes.
	static MStatus addNodes(MFnPlugin & plugin);


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

private:

	const bool own_classification_;

	static std::vector<std::unique_ptr<NodeBase>> instances_;

	template <class _INHERIT_FROM_NODEBASE, class ...Args>
	static void addNode(MFnPlugin & plugin, Args... args);

};

// inline implements
template<class _INHERIT_FROM_NODEBASE, class ...Args>
inline void NodeBase::addNode(MFnPlugin & plugin, Args ...args)
{
	if (std::is_base_of<NodeBase, _INHERIT_FROM_NODEBASE>) {
		std::unique_ptr<NodeBase> ptr = std::make_unique<_INHERIT_FROM_NODEBASE>(args);
		plugin.registerNode(ptr->name_, ptr->id, ptr->creator, ptr->initialize, ptr->type, (ptr->own_classification ? &ptr->classification_, nullptr));
		NodeBase::instances_.emplace_back(std::move(ptr));
	}
}

// end of CommandBase
}; // end of mpb
#endif // end of _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_