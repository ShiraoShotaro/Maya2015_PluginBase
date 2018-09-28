// ___replaceT___ : Node Name
// ___namespace___ : namespace

#pragma once
#ifndef ___replaceT____HPP
#define ___replaceT____HPP

#include "base/TranslatorBase.hpp"

namespace ___namespace___ {

class ___replaceT___ : public mpb::TranslatorBase{
public:

	/// @brief コンストラクタ
	explicit ___replaceT___(void) noexcept;

	/// @brief デストラクタ
	virtual ~___replaceT___(void);

	///
	/// @brief インスタンス生成関数
	///
	/// @return インスタンスのアドレス
	///
	static void * create(void);

	virtual MStatus writer(const MFileObject & file, const MString & options_string, MPxFileTranslator::FileAccessMode mode) override;

	virtual MStatus reader(const MFileObject & file, const MString & options_string, MPxFileTranslator::FileAccessMode mode) override;
	
private:
	
};

}

#endif