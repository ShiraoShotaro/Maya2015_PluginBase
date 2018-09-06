#pragma once
#ifndef DS4MOTIONCONTROL_HPP
#define DS4MOTIONCONTROL_HPP

#include "base/NodeBase.hpp"
#include <memory>

namespace wlib {
class DS4gyro;
}

namespace wlib {

class DS4MotionControl : public mpb::NodeBase{
public:
	DS4MotionControl(void);

	virtual ~DS4MotionControl(void);

	static MStatus initialize(void);

	static void * create(void);

	virtual void computeProcess(const MPlug & plug, MDataBlock & data);

private:
	static MObject m_in_time_;
	static MObject m_out_quaternion_x_;
	static MObject m_out_quaternion_y_;
	static MObject m_out_quaternion_z_;
	static MObject m_out_quaternion_w_;
	static MObject m_out_quaternion_;

	std::unique_ptr<DS4gyro> ds4gyro;

};

}

#endif