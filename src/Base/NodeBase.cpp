#include "NodeBase.hpp"

std::vector<std::unique_ptr<mpb::NodeBase>> mpb::NodeBase::instances_;

mpb::NodeBase::NodeBase(const MTypeId id, const MString & name, const MPxNode::Type type) noexcept
	: id_(id), name_(name), type_(type), own_classification_(false), classification_(""){}

mpb::NodeBase::NodeBase(const MTypeId id, const MString & name, const MString & classification, const MPxNode::Type type) noexcept
	: id_(id), name_(name), type_(type), own_classification_(true), classification_(classification) {}

mpb::NodeBase::~NodeBase(void)
{}

void mpb::NodeBase::computeProcess(const MPlug & plug, MDataBlock & data)
{
}

void mpb::NodeBase::setAttributeOption(MFnAttribute & attr, const bool isReadable, const bool isWritable, const bool isStorable, const bool isCache)
{
}

void mpb::NodeBase::attributeAffects(MObjects_const & whenChanges, MObjects_const & isAffect)
{
}

void mpb::NodeBase::addAttributeE(const MObject & obj, const MFnAttribute & attr)
{
}