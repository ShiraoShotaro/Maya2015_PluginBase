#pragma once
#ifndef _MAYA_PLUGIN_BASE_TRANSLATOR_BASE_HPP_
#define _MAYA_PLUGIN_BASE_TRANSLATOR_BASE_HPP_
/*
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MPxNode.h>
#include <maya/MFnAttribute.h>
*/

#include "exception/MStatusException.hpp"
#include <maya/MPxFileTranslator.h>
#include <maya/MString.h>
#include <vector>
#include <memory>

class MFnPlugin;

namespace mpb {

/// @brief ノードのベースクラス
///
/// ノードを実装するときは、このクラスを継承して定義してください。
///
class TranslatorBase : public MPxFileTranslator {
public:

	const MString name_;			///< トランスレーター名

	// Forbidden to use default constructor.
	TranslatorBase(void) = delete;


	/// @brief 引数付きコンストラクタ
	///
	/// @param [in] name ノード名
	/// @param [in] file_extension 扱うファイルの拡張子
	/// @param [in] can_import インポート可能か
	/// @param [in] can_export エクスポート可能か
	///
	TranslatorBase(
		const MString & name,
		const MString & file_extension,
		const bool can_import,
		const bool can_export,
		const MString & options_script_name = "",
		const MString & default_options_string = "",
		const MString & pixmap_name = "") noexcept;



	/// @brief デストラクタ
	///
	/// 現時点ではNOPです。
	///
	virtual ~TranslatorBase(void);


	/// @brief 書き込み処理関数
	///
	/// @param [in] file ファイルに関する情報
	/// @param [in] options_string オプション指定文字列
	/// @param [in] mode アクセスモード
	///
	virtual MStatus writer(const MFileObject & file, const MString & options_string, MPxFileTranslator::FileAccessMode mode) override;
	

	/// @brief 書き込み処理関数
	///
	/// @param [in] file ファイルに関する情報
	/// @param [in] options_string オプション指定文字列
	/// @param [in] mode アクセスモード
	///
	virtual MStatus reader(const MFileObject & file, const MString & options_string, MPxFileTranslator::FileAccessMode mode) override;

	
	/// @brief エクスポート可能か
	///
	/// コンストラクタのcan_import, can_exportを適切に設定していること。
	///
	/// @retval true 可能
	/// @retval false 不可能
	virtual bool haveWriteMethod() const override final;


	/// @brief インポート可能か
	///
	/// コンストラクタのcan_import, can_exportを適切に設定していること。
	///
	/// @retval true 可能
	/// @retval false 不可能
	virtual bool haveReadMethod() const override final;


	/// @brief 扱うファイルの拡張子を取得する
	///
	/// @return このトランスポーターで扱える拡張子
	///
	virtual MString defaultExtension() const override final;


	/// @brief トランスレーター追加定義の関数
	///
	/// ***main.cppにて、ユーザーが定義実装する必要があります。***
	///
	/// @retval MStatus::kSuccess すべてのトランスレーター追加に成功した場合
	/// @retval else トランスレーター追加に失敗した場合
	///
	static MStatus addTranslators(void) throw(mpb::MStatusException);


	/// @brief (INTERNAL FUNCTION)トランスレーターを削除します
	///
	/// 内部関数。ユーザーによって呼び出さないでください。
	///
	/// @param [in,out] plugin MFnPluginインスタンス
	///
	/// @retval MStatus::kSuccess 削除に成功した場合
	/// @retval else 削除に失敗した場合
	///
	static MStatus removeTranslators(MFnPlugin & plugin);

	/// @brief (INTERNAL FUNCTION)MFnPluginのポインタを登録する
	///
	/// 内部関数。ユーザーによって呼び出さないでください。
	///
	/// @param [in,out] MFnPluginインスタンス
	///
	static void _setMFnPluginPtr(MFnPlugin * plugin);

protected:


private:
	const bool can_import_;			///< インポート可能か
	const bool can_export_;			///< エクスポート可能か
	const MString file_extension_;	///< ファイル拡張子
	const MString pixmap_name_;
	const MString options_script_name_;
	const MString default_options_string_;
	
		
	static MFnPlugin * plugin_;
	static std::vector<std::unique_ptr<TranslatorBase>> instances_;

	template <class _INHERIT_FROM_TRANSLATORBASE> static void addTranslator(void);
	template <class _INHERIT_FROM_TRANSLATORBASE, class ...Args> static void addTranslator(Args... args);
	static void _addTranslator(void * (*creator)(), std::unique_ptr<TranslatorBase> && translator);

};


template<class _INHERIT_FROM_TRANSLATORBASE>
inline void TranslatorBase::addTranslator(void) {
	TranslatorBase::_addTranslator(&_INHERIT_FROM_TRANSLATORBASE::create, std::make_unique<_INHERIT_FROM_TRANSLATORBASE>());
}
template<class _INHERIT_FROM_TRANSLATORBASE, class ...Args>
inline void TranslatorBase::addTranslator(Args ...args) {
	TranslatorBase::_addTranslator(&_INHERIT_FROM_TRANSLATORBASE::create, std::make_unique<_INHERIT_FROM_TRANSLATORBASE>(args));
}

// end of CommandBase
}; // end of mpb
#endif // end of _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_
