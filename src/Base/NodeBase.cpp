#include "NodeBase.hpp"
#include "exception\MStatusException.hpp"
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <string>

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

void mpb::NodeBase::setMultiAttributeAffects(const std::vector<const MObject *> & whenChanges, const std::vector<const MObject *> & isAffect)
{
	int widx = 0;
	for (int widx = 0; widx < whenChanges.size(); ++widx) {
		for (int iidx = 0; iidx < isAffect.size(); ++iidx) {
			MStatusException::throwIf(attributeAffects(*whenChanges.at(widx), *isAffect.at(iidx)), std::string("アトリビュートの影響設定に失敗 : widx = " + std::to_string(widx) + " -> iidx = " + std::to_string(iidx)).c_str());
		}
	}

	for (int iidx = 0; iidx < isAffect.size(); ++iidx) {
		MStatusException::throwIf(attributeAffects(state, *isAffect.at(iidx)), std::string("アトリビュートの影響設定に失敗 : state -> iidx = " + std::to_string(iidx)).c_str());
	}
}

void mpb::NodeBase::addAttr(const MObject & obj, const MFnAttribute & attr)
{
	MStatusException::throwIf(MPxNode::addAttribute(obj), attr.name() + "アトリビュートの追加に失敗");
}

void mpb::NodeBase::addEnumAttr(MObject & target, const MString & longname, const MString & shortname, const AttributeOptions & options, const std::vector<std::pair<MString, short>>& enums, const short def_value) {
	MFnEnumAttribute attr;
	target = attr.create(longname, shortname, def_value);
	options.apply(attr);
	for (const auto & p : enums) {
		attr.addField(p.first, p.second);
	}
	addAttr(target, attr);
}

void mpb::NodeBase::addNumericAttr(MObject & target, const MString & longname, const MString & shortname, const AttributeOptions & options, const MFnNumericData::Type & numeric_data, const double & def_value) {
	MFnNumericAttribute attr;
	target = attr.create(longname, shortname, numeric_data, def_value);
	options.apply(attr);
	addAttr(target, attr);
}

void mpb::NodeBase::addNumericAttr(MObject & target, const MString & longname, const MString & shortname, const AttributeOptions & options, const MObject & child1, const MObject & child2, const MObject & child3) {
	MFnNumericAttribute attr;
	target = attr.create(longname, shortname, child1, child2, child3);
	options.apply(attr);
	addAttr(target, attr);
}

template<class T>
void mpb::NodeBase::addUnitAttr(MObject & target, const MString & longname, const MString & shortname, const T & def_value, const AttributeOptions & options) {
	MFnUnitAttribute attr;
	target = attr.create(longname, shortname, def_value);
	options.apply(attr);
	addAttr(target, attr);
}

template void mpb::NodeBase::addUnitAttr(MObject &, const MString &, const MString &, const MAngle &, const AttributeOptions & options);
template void mpb::NodeBase::addUnitAttr(MObject &, const MString &, const MString &, const MTime &, const AttributeOptions & options);
template void mpb::NodeBase::addUnitAttr(MObject &, const MString &, const MString &, const MDistance &, const AttributeOptions & options);


////////////////////////////////////////////////

mpb::NodeBase::AttributeOptions::AttributeOptions(const bool is_readable, const bool is_writable, const bool is_cached, const bool is_keyable, const bool is_storable) noexcept 
	: is_readable(is_readable), is_writable(is_writable), is_cached(is_cached), is_keyable(is_keyable), is_storable(is_storable) {
	/*if (!this->is_writable) { this->is_keyable = false; this->is_storable = false; }
	if (!this->is_readable) { this->is_cached = false; }*/
}

void mpb::NodeBase::AttributeOptions::apply(MFnAttribute & attr) const {
	MStatusException::throwIf(attr.setReadable(this->is_readable), attr.name() + "アトリビュートのReadableを変更できません");
	MStatusException::throwIf(attr.setWritable(this->is_writable), attr.name() + "アトリビュートのWritableを変更できません");
	MStatusException::throwIf(attr.setStorable(this->is_storable), attr.name() + "アトリビュートのStorableを変更できません");
	MStatusException::throwIf(attr.setCached(this->is_cached), attr.name() + "アトリビュートのCachableを変更できません");
	MStatusException::throwIf(attr.setKeyable(this->is_keyable), attr.name() + "アトリビュートのKeyableを変更できません");
}
