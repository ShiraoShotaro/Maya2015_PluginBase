#include "NodeBase.hpp"

std::vector<std::unique_ptr<mpb::NodeBase>> mpb::NodeBase::instances_;

mpb::NodeBase::NodeBase(const MTypeId id, const MString & name, const MPxNode::Type type) noexcept
	: id_(id), name_(name), type_(type), own_classification_(false), classification_(""){}

mpb::NodeBase::NodeBase(const MTypeId id, const MString & name, const MString & classification, const MPxNode::Type type) noexcept
	: id_(id), name_(name), type_(type), own_classification_(true), classification_(classification) {}

mpb::NodeBase::~NodeBase(void)
{}
