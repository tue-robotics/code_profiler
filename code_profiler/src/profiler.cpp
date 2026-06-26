#include "tue/profiling/profiler.h"

#include <iostream>
#include <string>
#include <utility>

namespace tue
{

// ----------------------------------------------------------------------------------------------------

Profiler::Profiler() : head_(nullptr), parent_(nullptr) {}

// ----------------------------------------------------------------------------------------------------

Profiler::Profiler(std::string name) : name_(std::move(name)), head_(nullptr), parent_(nullptr) {}

// ----------------------------------------------------------------------------------------------------

Profiler::Profiler(std::string name, Profiler* parent) : name_(std::move(name)), head_(nullptr), parent_(parent) {}

// ----------------------------------------------------------------------------------------------------

Profiler::~Profiler()
{
    for (auto& it : children_)
    {
        delete it.second;
    }
}

// ----------------------------------------------------------------------------------------------------

void Profiler::startTimer(const std::string& name)
{
    if (!head_)
    {
        head_ = this;
    }

    Profiler* child = nullptr;
    auto it_child = head_->children_.find(name);
    if (it_child != head_->children_.end())
    {
        child = it_child->second;
    }
    else
    {
        child = new Profiler(name, head_);
        head_->children_[name] = child;
    }

    head_ = child;

    child->timer_.start();
}

// ----------------------------------------------------------------------------------------------------

void Profiler::stopTimer()
{
    if (!head_)
    {
        std::cout << "[tue::Profiler] stopTimer() called, but no timer is active." << '\n';
        return;
    }

    head_->timer_.stop();
    head_ = head_->parent_;
}

// ----------------------------------------------------------------------------------------------------

// NOLINTNEXTLINE(misc-no-recursion)
void Profiler::addToStream(std::ostream& out, const std::string& prefix) const
{
    if (!parent_)
    {
        out << prefix << "[" << name_ << "]" << '\n';
    }
    else
    {
        out << prefix << name_ << ": " << timer_.getElapsedTimeInMilliSec() << " ms" << '\n';
    }

    for (const auto& it : children_)
    {
        it.second->addToStream(out, prefix + "  ");
    }
}

// ----------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const Profiler& p)
{
    p.addToStream(out);
    return out;
}

} // namespace tue
