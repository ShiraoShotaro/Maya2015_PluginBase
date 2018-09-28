// ___replace___ : Node Name
// ___namespace___ : namespace

#pragma once
#ifndef ___replace____HPP
#define ___replace____HPP

#include "base/NodeBase.hpp"

namespace ___namespace___ {

class ___replace___ : public mpb::NodeBase{
public:

	/// @brief コンストラクタ
	explicit ___replace___(void);

	/// @brief デストラクタ
	virtual ~___replace___(void);

	///
	/// @brief 初期化関数
	///
	/// アトリビュートの登録を行います。
	///
	/// @retval MStatus::kSuccess 成功
	/// @retval else 失敗
	///
	static MStatus initialize(void) throw (mpb::MStatusException);

	///
	/// @brief インスタンス生成関数
	///
	/// @return インスタンスのアドレス
	///
	static void * create(void);

	///
	/// @brief 計算処理関数
	///
	/// @param [in] plug 再計算要求のプラグ
	/// @param [in,out] data データセット
	///
	/// @throw MStatusException 何かエラーが発生した場合
	///
	virtual void computeProcess(const MPlug & plug, MDataBlock & data);

private:

	static MObject m_in_dummy_;	// プラグ
	
};

}

#endif