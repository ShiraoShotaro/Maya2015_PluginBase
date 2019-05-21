#pragma once
#ifndef _MAYA_PLUGIN_BASE_NODE_BASE_HPP_
#define _MAYA_PLUGIN_BASE_NODE_BASE_HPP_

#include "exception/MStatusException.hpp"
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MPxNode.h>
#include <maya/MFnAttribute.h>
#include <vector>
#include <memory>

class MFnPlugin;

namespace mpb {

/// @brief ノードのベースクラス
///
/// ノードを実装するときは、このクラスを継承して定義してください。
///
class NodeBase : public MPxNode {
public:

	const MString name_;			///< ノード名

	const MTypeId id_;				///< ノードID

	const MPxNode::Type type_;		///< ノードタイプ

	const MString classification_;	///< カスタムクラシフィケーション
	
	NodeBase(void) = delete;

	/// @brief 引数付きコンストラクタ
	///
	/// @param [in] id ノードID。他と被らないように指定してください。
	/// @param [in] name ノード名
	/// @param [in] type ノードタイプ。デフォルトのタイプはkDependNode
	///
	NodeBase(const MTypeId id, const MString & name, const MPxNode::Type type = MPxNode::Type::kDependNode) noexcept;


	/// @brief 引数付きコンストラクタ
	///
	/// ノードクラシフィケーションについてはこちらのページを参照してください。
	/// [http://download.autodesk.com/us/maya/2010help/API/class_m_fn_plugin.html#7df9f8e677ea8c71e33016361109506a]
	///
	/// @param [in] id ノードID。他と被らないように指定してください。
	/// @param [in] name ノード名
	/// @param [in] classification カスタムクラシフィケーション
	/// @param [in] type ノードタイプ。デフォルトのタイプはkDependNode
	///
	NodeBase(const MTypeId id, const MString & name, const MString & classification, const MPxNode::Type type = MPxNode::Type::kDependNode) noexcept;


	/// @brief デストラクタ
	///
	/// 現時点ではNOPです。
	///
	virtual ~NodeBase(void);


	/// @brief compute関数の独自オーバーライド
	///
	/// ノードの計算処理部分のオーバーライド。
	///
	/// もしもこの関数を使うのであれば、computeProcess関数を継承先のクラスでオーバーライドすること。
	/// 主に例外処理に対応する。computeProcess関数にてMStatusExceptionを投げることができ、それをこのcompute関数で受け取り適宜エラー表示する。
	///
	/// @param [in] plug 計算中のプラグ
	/// @param [in,out] data 編集可能な内部データ
	/// @retval MStatus::kSuccess 計算に成功した場合
	/// @retval else 何らかのエラーで失敗した場合
	/// @sa MPxNode::compute http://download.autodesk.com/us/maya/2011help/api/class_m_px_node.html#3dd5454e3fa92598836018ae9ab220c3
	///
	virtual MStatus compute(const MPlug & plug, MDataBlock & data) override;


	/// @brief ノード追加定義の関数
	///
	/// ***main.cppにて、ユーザーが定義実装する必要があります。***
	///
	/// @retval MStatus::kSuccess すべてのノード追加に成功した場合
	/// @retval else ノード追加に失敗した場合
	///
	static MStatus addNodes(void);


	/// @brief (INTERNAL FUNCTION)ノードを削除します
	///
	/// 内部関数。ユーザーによって呼び出さないでください。
	///
	/// @param [in,out] plugin MFnPluginインスタンス
	///
	/// @retval MStatus::kSuccess 削除に成功した場合
	/// @retval else 削除に失敗した場合
	///
	static MStatus removeNodes(MFnPlugin & plugin);

	/// @brief (INTERNAL FUNCTION)MFnPluginのポインタを登録する
	///
	/// 内部関数。ユーザーによって呼び出さないでください。
	///
	/// @param [in,out] MFnPluginインスタンス
	///
	static void _setMFnPluginPtr(MFnPlugin * plugin);

protected:

	/// @brief 継承先のクラスでオーバーライドすべきcompute関数
	///
	/// このクラスで用意した例外対策済のcompute関数を使うならば、このcomputeProcess関数をオーバーライドして使用してください。
	/// もしも継承先のクラスで、computeもcomputeProcessもオーバーライド定義されていない場合は、問答無用でkUnknowParameterがmayaへ通達されます。
	/// また、MStatusExceptionの第3引数を省略する場合、compute関数にて自動的にリネームされ、computeProcessと表示される。
	///
	/// @param [in] plug 計算中のプラグ
	/// @param [in,out] data 編集可能な内部データ
	///
	virtual void computeProcess(const MPlug & plug, MDataBlock & data);

	struct AttributeOptions{
		bool is_readable, is_writable, is_cached, is_keyable, is_storable;
		AttributeOptions(const bool is_readable = true, const bool is_writable = true, const bool is_cached = true, const bool is_keyable = true, const bool is_storable = true) noexcept;
		AttributeOptions(const AttributeOptions&) = default;
		void apply(MFnAttribute & attr) const;
	};
	

	/// @brief アトリビュートの変更の影響設定を一括で行います。
	///
	/// @param [in] when_changes 変更を監視するアトリビュート
	/// @param [in] is_affect 変更を行うアトリビュート
	///
	/// @throws MStatusException アトリビュート属性の指定時に何らかのエラーが発生した場合
	///
	static void setMultiAttributeAffects(const std::vector<const MObject *> & when_changes, const std::vector<const MObject *> & is_affect);


	/// @brief アトリビュートを追加します。
	///
	/// エラーが起きたときの対応を含めた、addAttribute関数のラッパー関数です。
	///
	/// @param [in] obj プラグオブジェクト
	/// @param [in] attr アトリビュート 
	///
	/// @throws MStatusException アトリビュートの追加に失敗した場合
	///
	static void addAttr(const MObject & obj, const MFnAttribute & attr);


	/// @brief Enumアトリビュートの追加のショートカット
	static void addEnumAttr(MObject & target, const MString & longname, const MString & shortname, const AttributeOptions & options, const std::vector<std::pair<MString, short>> & enums, const short def_value);

	/// @brief Typeアトリビュート追加のショートカット
	template <class T> static void addUnitAttr(MObject & target, const MString & longname, const MString & shortname, const T & def_value, const AttributeOptions & options);

	/// @brief Numericアトリビュートの追加のショートカット
	static void addNumericAttr(MObject & target, const MString & longname, const MString & shortname, const AttributeOptions & options, const MFnNumericData::Type & numeric_data = MFnNumericData::Type::kDouble, const double & def_value = 0.0);
	static void addNumericAttr(MObject & target, const MString & longname, const MString & shortname, const AttributeOptions & options, const MObject & child1, const MObject & child2, const MObject & child3 = MObject::kNullObj);


private:
	
	const bool own_classification_;
	
	static MFnPlugin * plugin_;
	static std::vector<std::unique_ptr<NodeBase>> instances_;

	template <class _INHERIT_FROM_NODEBASE> static void addNode(void);
	template <class _INHERIT_FROM_NODEBASE, class ...Args> static void addNode(Args... args);
	static void _addNode(void * (*creator)(), MStatus(*initialize)(), std::unique_ptr<NodeBase> && node);

};


template<class _INHERIT_FROM_NODEBASE>
inline void NodeBase::addNode(void) {
	NodeBase::_addNode(&_INHERIT_FROM_NODEBASE::create, &_INHERIT_FROM_NODEBASE::initialize, std::make_unique<_INHERIT_FROM_NODEBASE>());
}
template<class _INHERIT_FROM_NODEBASE, class ...Args>
inline void NodeBase::addNode(Args ...args) {
	NodeBase::_addNode(&_INHERIT_FROM_NODEBASE::create, &_INHERIT_FROM_NODEBASE::initialize, std::make_unique<_INHERIT_FROM_NODEBASE>(args));
}

// end of CommandBase
}; // end of mpb
#endif // end of _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_
