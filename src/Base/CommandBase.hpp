#pragma once
#ifndef _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_
#define _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_

#include "exception/MStatusException.hpp"
#include <maya/MString.h>
#include <maya/MPxCommand.h>
#include <vector>
#include <memory>

class MFnPlugin;

namespace mpb {

/// @brief コマンドのベースクラスです
///
/// 新しくコマンドを追加実装する場合は、このクラスを継承して実装してください
///
class CommandBase : public MPxCommand {
public:

	const MString command_;		///< コマンド文字列

	const bool is_undoable_;	///< UNDOできるか

	CommandBase(void) = delete;
	
	/// @brief コンストラクタ
	///
	/// @param [in] command コマンド文字列
	/// @param [in] is_undoable UNDOできるか
	///
	CommandBase(const MString & command, const bool is_undoable) noexcept;

	
	/// @brief デストラクタ
	///
	/// 現在は何も処理をしていません
	///
	virtual ~CommandBase(void);


	/// @brief 実行関数
	///
	/// コマンドで実行したい動作をオーバーライドして定義します。
	///
	/// 1コマンドに対し、一度しか呼び出されません。
	/// そのため、Undoableなコマンドの場合は、この関数で準備を行い、redo()で実際に変更を加えるような実装が好ましいでしょう。
	///
	/// デフォルトでは何も処理をしません
	///
	/// @param [in] args コマンドライン引数
	///
	/// @return コマンドの実行結果
	///
	virtual MStatus doIt(const MArgList& args) override;

	
	/// @brief 再実行関数
	///
	/// コマンドで再実行したい動作をオーバーライドして定義します。
	///
	/// Ctrl-Yでredoされた場合に毎回呼び出されます。
	///
	/// デフォルトでは何も処理をしません。Undoableの場合は必ず実装します。
	///
	/// @return コマンドの再実行の結果
	///
	virtual MStatus redoIt() override;
	

	/// @brief 元に戻す関数
	///
	/// コマンドを取り消す際の動作をオーバーライドして定義します。
	///
	/// Ctrl-Zでundoされた場合に毎回呼び出されます。
	///
	/// デフォルトでは何も処理をしません。Undoableの場合は必ず実装します。
	///
	/// @return コマンドを元に戻すの結果
	///
	virtual MStatus undoIt() override;
	

	/// @brief 元に戻す、が有効か調べる
	///
	/// デフォルトではis_undoable_の変数をそのまま返します。
	///
	/// また、オーバーライドして独自の定義をすることもできます。
	///
	/// @retval true 有効
	/// @retval false 無効
	///
	virtual bool isUndoable() const override;


	/// @brief コマンドの追加関数
	///
	/// ***main.cppにて、ユーザーが定義実装する必要があります。***
	///
	/// @param [in,out] plugin MFnPluginのインスタンス
	///
	/// @retval MStatus::kSuccess 成功
	/// @retval else 失敗
	///
	static MStatus addCommands(void);


	/// @brief (INTERNAL FUNCTION)コマンドを削除
	///
	/// 内部関数。ユーザーによって呼び出さないでください。
	///
	/// @param [in,out] plugin MFnPluginのインスタンス
	///
	/// @retval MStatus::kSuccess 成功
	/// @retval else 失敗
	///
	static MStatus removeCommands(MFnPlugin & plugin);


	/// @brief (INTERNAL FUNCTION)MFnPluginのポインタを登録する
	///
	/// 内部関数。ユーザーによって呼び出さないでください。
	///
	/// @param [in,out] MFnPluginインスタンス
	///
	static void _setMFnPluginPtr(MFnPlugin * plugin);

private:

	static MFnPlugin * plugin_;
	static std::vector<std::unique_ptr<CommandBase>> instances_;

	template <class _INHERIT_FROM_COMMANDBASE> static void addCommand(void);
	template <class _INHERIT_FROM_COMMANDBASE, class ...Args> static void addCommand(Args... args);
	static void _addCommand(void * (*creator)(), std::unique_ptr<CommandBase> && command);

};
template<class _INHERIT_FROM_COMMANDBASE>
inline void CommandBase::addCommand(void) {
	CommandBase::_addCommand(&_INHERIT_FROM_COMMANDBASE::create, std::make_unique<_INHERIT_FROM_COMMANDBASE>());
}
template<class _INHERIT_FROM_COMMANDBASE, class ...Args>
inline void CommandBase::addCommand(Args ...args) {
	CommandBase::_addCommand(&_INHERIT_FROM_COMMANDBASE::create, std::make_unique<_INHERIT_FROM_COMMANDBASE>(args));
}
// end of CommandBase
}; // end of mpb
#endif // end of _MAYA_PLUGIN_BASE_COMMAND_BASE_HPP_