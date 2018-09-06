#include "DS4MotionControl.hpp"
#include "exception/MStatusException.hpp"
#include "ds4gyro/core/DS4gyro.hpp"

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MTime.h>
#include <cmath>


MObject wlib::DS4MotionControl::m_in_time_;
MObject wlib::DS4MotionControl::m_out_quaternion_x_;
MObject wlib::DS4MotionControl::m_out_quaternion_y_;
MObject wlib::DS4MotionControl::m_out_quaternion_z_;
MObject wlib::DS4MotionControl::m_out_quaternion_w_;
MObject wlib::DS4MotionControl::m_out_quaternion_;

wlib::DS4MotionControl::DS4MotionControl(void) : NodeBase(0x70030, "DS4MotionControl"), ds4gyro(nullptr) {
	this->ds4gyro = std::make_unique<DS4gyro>(0x054C, 0x09CC, wlib::DS4gyro::kUsb);
}

wlib::DS4MotionControl::~DS4MotionControl(void){}

MStatus wlib::DS4MotionControl::initialize(void)
{
	try {

		{
			// 強制更新のためのtime入力
			MFnUnitAttribute attr;
			m_in_time_ = attr.create("time", "time", MTime(0.0));
			setAttributeOption(attr, false, true, false, false);
			addAttr(m_in_time_, attr);
		}

		{
			// クォータニオン出力
			MFnNumericAttribute attrX, attrY, attrZ, attrW;
			MFnCompoundAttribute attr;
			m_out_quaternion_x_ = attrX.create("outQuaternionX", "oqtx", MFnNumericData::kDouble, 0.0);
			m_out_quaternion_y_ = attrY.create("outQuaternionY", "oqty", MFnNumericData::kDouble, 0.0);
			m_out_quaternion_z_ = attrZ.create("outQuaternionZ", "oqtz", MFnNumericData::kDouble, 0.0);
			m_out_quaternion_w_ = attrW.create("outQuaternionW", "oqtw", MFnNumericData::kDouble, 0.0);
			setAttributeOption(attrX, true, false, false, false);
			setAttributeOption(attrY, true, false, false, false);
			setAttributeOption(attrZ, true, false, false, false);
			setAttributeOption(attrW, true, false, false, false);
			addAttr(m_out_quaternion_x_, attrX);
			addAttr(m_out_quaternion_y_, attrY);
			addAttr(m_out_quaternion_z_, attrZ);
			addAttr(m_out_quaternion_w_, attrW);

			m_out_quaternion_ = attr.create("outQuaternion", "oqt");
			attr.addChild(m_out_quaternion_x_);
			attr.addChild(m_out_quaternion_y_);
			attr.addChild(m_out_quaternion_z_);
			attr.addChild(m_out_quaternion_w_);
			setAttributeOption(attr, true, false, false, false);
			addAttr(m_out_quaternion_, attr);
		}

		//影響登録
		setMultiAttributeAffects({&m_in_time_}, {&m_out_quaternion_, &m_out_quaternion_x_,  &m_out_quaternion_y_,  &m_out_quaternion_z_});

	}
	catch (mpb::MStatusException e) {
		std::cerr << e.toString("DS4MotionControl") << std::endl;
		return e.stat;
	}

	return MStatus::kSuccess;
}

void * wlib::DS4MotionControl::create(void)
{
	return new DS4MotionControl();
}

void wlib::DS4MotionControl::computeProcess(const MPlug & plug, MDataBlock & data)
{
	if (plug == m_out_quaternion_ || plug == m_out_quaternion_x_ || plug == m_out_quaternion_y_ || plug == m_out_quaternion_z_ || plug == m_out_quaternion_w_) {
		
		data.inputValue(m_in_time_).asTime();

		std::cout << "updating...";

		this->ds4gyro->update();
		
		const DS4gyro::Vector3<double> a = this->ds4gyro->getAccel();
		const DS4gyro::Vector3<double> b(0, 0, -1);
		DS4gyro::Vector3<double> axis;

		std::cout << " x:" << a.x << " y:" << a.y << " z:" << a.z << std::endl;

		axis.x = a.y * b.z - a.z * b.y;
		axis.y = a.z * b.x - a.x * b.z;
		axis.z = a.x * b.y - a.y * b.x;
		const double cos_theta = (a.x * b.x + a.y * b.y + a.z * b.z) / ((std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z) * std::sqrt(b.x * b.x + b.y * b.y + b.z * b.z)));
		const double sin_harf_theta = std::sqrt((1.0 - cos_theta) / 2.0);
		const double cos_harf_theta = std::sqrt((1.0 + cos_theta) / 2.0);
		DS4gyro::Quaternion out(axis.x * sin_harf_theta, axis.y * sin_harf_theta, axis.z * sin_harf_theta, cos_harf_theta);

		data.outputValue(m_out_quaternion_x_).setDouble(out.x);
		data.outputValue(m_out_quaternion_y_).setDouble(out.y);
		data.outputValue(m_out_quaternion_z_).setDouble(out.z);
		data.outputValue(m_out_quaternion_w_).setDouble(out.w);

		data.setClean(m_out_quaternion_);
		data.setClean(m_out_quaternion_x_);
		data.setClean(m_out_quaternion_y_);
		data.setClean(m_out_quaternion_z_);
		data.setClean(m_out_quaternion_w_);
		data.setClean(plug);
	}
	else throw mpb::MStatusException(MStatus::kInvalidParameter, "予期しないプラグの再計算要求");
}
