#pragma once
#ifndef _MAYA_PLUGIN_BASE_NODE_BASE_HPP_
#define _MAYA_PLUGIN_BASE_NODE_BASE_HPP_

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MPxNode.h>
#include <maya/MFnAttribute.h>
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
	
	NodeBase(void) = delete;

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


	/// @brief compute関数の独自オーバーライド
	///
	/// ノードの計算処理部分のオーバーライド。もしもこの関数を使うのであれば、computeProcess関数を継承先のクラスでオーバーライドすること。
	/// 主に例外処理に対応する。computeProcess関数にてMStatusExceptionを投げることができ、それをこのcompute関数で受け取り適宜エラー表示する。
	/// @param plug 計算中のプラグ
	/// @param data 編集可能な内部データ
	/// @return 計算に成功した場合kSuccessを返す
	/// @sa MPxNode::compute http://download.autodesk.com/us/maya/2011help/api/class_m_px_node.html#3dd5454e3fa92598836018ae9ab220c3
	virtual MStatus compute(const MPlug & plug, MDataBlock & data) override;


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

protected:

	/// @brief 継承先のクラスでオーバーライドすべきcompute関数
	///
	/// このクラスで用意した例外対策済のcompute関数を使うならば、このcomputeProcess関数をオーバーライドして使用してください。
	/// もしも継承先のクラスで、computeもcomputeProcessもオーバーライド定義されていない場合は、問答無用でkUnknowParameterがmayaへ通達されます。
	/// また、MStatusExceptionの第3引数を省略する場合、compute関数にて自動的にリネームされ、computeProcessと表示される。
	/// @param plug 計算中のプラグ
	/// @param data 編集可能な内部データ
	virtual void computeProcess(const MPlug & plug, MDataBlock & data);


	/// @brief アトリビュートの属性(readable, writable, storable, cached)を一括指定します。
	/// @param isReadable 読み込み可能か
	/// @param isWritable 書き込み可能か
	/// @param isStorable 保存可能か
	/// @throws MStatusException アトリビュート属性の指定時に何らかのエラーが発生した場合
	static void setAttributeOption(MFnAttribute & attr, const bool isReadable, const bool isWritable, const bool isStorable, const bool isCache);

	
	using MObjects_const = const std::vector<const MObject *>;


	/// @brief アトリビュートの変更の影響設定を一括で行います。
	/// @param whenChanges 変更を監視するアトリビュート
	/// @param isWritable 変更を行うアトリビュート
	/// @param isStorable 保存可能か
	/// @throws MStatusException アトリビュート属性の指定時に何らかのエラーが発生した場合
	static void setMultiAttributeAffects(MObjects_const & whenChanges, MObjects_const & isAffect);


	/// @brief アトリビュートを追加します。
	///
	/// エラーが起きたときの対応を含めた、addAttribute関数のラッパー関数です。
	/// @param obj プラグオブジェクト
	/// @param attr アトリビュート 
	static void addAttributeE(const MObject & obj, const MFnAttribute & attr);

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