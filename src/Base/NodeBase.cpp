#include "NodeBase.hpp"
#include "exception\MStatusException.hpp"

std::vector<std::unique_ptr<mpb::NodeBase>> mpb::NodeBase::instances_;

mpb::NodeBase::NodeBase(const MTypeId id, const MString & name, const MPxNode::Type type) noexcept
	: id_(id), name_(name), type_(type), own_classification_(false), classification_(""){}

mpb::NodeBase::NodeBase(const MTypeId id, const MString & name, const MString & classification, const MPxNode::Type type) noexcept
	: id_(id), name_(name), type_(type), own_classification_(true), classification_(classification) {}

mpb::NodeBase::~NodeBase(void)
{}

MStatus mpb::NodeBase::compute(const MPlug & plug, MDataBlock & data)
{
	MStatus ret;
	try {
		this->computeProcess(plug, data);
	}
	catch (MStatusException e) {
		std::cerr << e.toString("NODE : " + this->name_) << std::endl;
		ret = e;
	}
	return ret;
}

void mpb::NodeBase::computeProcess(const MPlug & plug, MDataBlock & data)
{
	MStatusException::throwIf(MStatus::kUnknownParameter, "computeProcess関数が定義されていません", "wlib::NodeBase::computeProcess<default>");
}

void mpb::NodeBase::setAttributeOption(MFnAttribute & attr, const bool isReadable, const bool isWritable, const bool isStorable, const bool isCache)
{
	MStatusException::throwIf(attr.setReadable(isReadable), attr.name() + "アトリビュートのReadableを変更できません");
	MStatusException::throwIf(attr.setWritable(isWritable), attr.name() + "アトリビュートのWritableを変更できません");
	MStatusException::throwIf(attr.setStorable(isStorable), attr.name() + "アトリビュートのStorableを変更できません");
	MStatusException::throwIf(attr.setCached(isCache), attr.name() + "アトリビュートのCachableを変更できません");
}

void mpb::NodeBase::setMultiAttributeAffects(MObjects_const & whenChanges, MObjects_const & isAffect)
{
	for (auto w = whenChanges.begin(); w != whenChanges.end(); ++w) {
		for (auto i = isAffect.begin(); i != isAffect.end(); ++i) {
			MStatusException::throwIf(MPxNode::attributeAffects(*(*w), *(*i)), "アトリビュートの影響設定に失敗");
		}
	}
}

void mpb::NodeBase::addAttributeE(const MObject & obj, const MFnAttribute & attr)
{
	MStatusException::throwIf(addAttribute(obj), attr.name() + "アトリビュートの追加に失敗");
}
