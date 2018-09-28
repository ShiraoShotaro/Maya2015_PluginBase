#include "TranslatorTemplate.hpp"

___namespace___::___replaceT___::___replaceT___(void) noexcept
	: mpb::TranslatorBase("___replaceT___", ".extension", true, true)
{}

___namespace___::___replaceT___::~___replaceT___(void)
{}

void * ___namespace___::___replaceT___::create(void)
{
	return new ___replaceT___;
}

MStatus ___namespace___::___replaceT___::writer(const MFileObject & file, const MString & options_string, MPxFileTranslator::FileAccessMode mode)
{
	std::cout << "writting... dummy ;-)" << std::endl;
	return MStatus::kSuccess;
}

MStatus ___namespace___::___replaceT___::reader(const MFileObject & file, const MString & options_string, MPxFileTranslator::FileAccessMode mode)
{
	std::cout << "reading... dummy ;-)" << std::endl;
	return MStatus();
}
