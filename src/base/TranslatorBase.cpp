#include "TranslatorBase.hpp"

mpb::TranslatorBase::TranslatorBase(const MString & name, const MString & file_extension, const bool can_import, const bool can_export, const MString & options_script_name, const MString & default_options_string, const MString & pixmap_name) noexcept
	: name_(name), file_extension_(file_extension), can_import_(can_import), can_export_(can_export_),
	pixmap_name_(pixmap_name), options_script_name_(options_script_name), default_options_string_(default_options_string)
{}
mpb::TranslatorBase::~TranslatorBase(void){}
MStatus mpb::TranslatorBase::writer(const MFileObject & file, const MString & options_string, MPxFileTranslator::FileAccessMode mode)
{ return MStatus::kNotImplemented; }
MStatus mpb::TranslatorBase::reader(const MFileObject & file, const MString & options_string, MPxFileTranslator::FileAccessMode mode)
{ return MStatus::kNotImplemented; }
bool mpb::TranslatorBase::haveWriteMethod() const { return this->can_export_;}
bool mpb::TranslatorBase::haveReadMethod() const { return this->can_import_; }
MString mpb::TranslatorBase::defaultExtension() const { return this->file_extension_; }
