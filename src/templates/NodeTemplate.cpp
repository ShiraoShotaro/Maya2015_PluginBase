#include "NodeTemplate.hpp"
#include "exception/MStatusException.hpp"

//#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
//#include <maya/MFnCompoundAttribute.h>
//#include <maya/MTime.h>

MObject ___namespace___::___replace___::m_in_dummy_;

___namespace___::___replace___::___replace___(void) : NodeBase(0x70050, "___replace___") {}

___namespace___::___replace___::~___replace___(void) {}

void * ___namespace___::___replace___::create(void) { return new ___replace___; }

MStatus ___namespace___::___replace___::initialize(void){
	try {

		////////////ADD HERE/////////////

		//////////////TEMPORARY
		{
			MFnNumericAttribute attr;
			m_in_dummy_ = attr.create("fullname", "shortname", MFnNumericData::kDouble, 0.0);
			setAttributeOption(attr, true, true, true, true);
			addAttr(m_in_dummy_, attr);
		}

		setMultiAttributeAffects({ &m_in_dummy_ }, { &m_in_dummy_ });
		//////////////END OF TEMPORARY

	}
	catch (mpb::MStatusException e) {
		std::cerr << e.toString("___replace___") << std::endl;
		return e.stat;
	}
	return MStatus::kSuccess;
}

void ___namespace___::___replace___::computeProcess(const MPlug & plug, MDataBlock & data){
	if (plug == m_in_dummy_) {
		data.setClean(plug);
	}
	else throw mpb::MStatusException(MStatus::kInvalidParameter, "予期しないプラグの再計算要求 : " + plug.name());
}




