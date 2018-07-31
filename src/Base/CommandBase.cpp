#include "CommandBase.hpp"

std::vector<std::unique_ptr<mpb::CommandBase>> mpb::CommandBase::instances_;

mpb::CommandBase::CommandBase(const MString & command, const bool is_undoable) noexcept
	: command_(command), is_undoable_(is_undoable) {}

mpb::CommandBase::~CommandBase(void){}

MStatus mpb::CommandBase::doIt(const MArgList & args)
{
	std::cout << "Command : " << this->command_ << " said to do It! but nothing to do..." << std::endl;
	return MStatus::kSuccess;
}

MStatus mpb::CommandBase::redoIt()
{
	std::cout << "Command : " << this->command_ << " said to redo It! but nothing to do..." << std::endl;
	return MStatus();
}

MStatus mpb::CommandBase::undoIt()
{
	std::cout << "Command : " << this->command_ << " said to undo It! but nothing to do..." << std::endl;
	return MStatus();
}

bool mpb::CommandBase::isUndoable() const
{ return this->is_undoable_; }
