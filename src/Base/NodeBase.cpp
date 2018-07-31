#include "NodeBase.hpp"

mpb::NodeBase::NodeBase(const MTypeId id, const MString & name, const MPxNode::Type type) noexcept
	: id_(id), name_(name), type_(type), own_classification_(false), classification_(""){}

mpb::NodeBase::NodeBase(const MTypeId id, const MString & name, const MString & classification, const MPxNode::Type type) noexcept
	: id_(id), name_(name), type_(type), own_classification_(true), classification_(classification) {}

mpb::NodeBase::~NodeBase(void)
{}

MStatus mpb::NodeBase::initialize(void)
{ return MStatus::kSuccess; }

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
