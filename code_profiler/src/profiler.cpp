#include "tue/profiling/profiler.h"

#include <iostream>
#include <memory>
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

void Profiler::startTimer(const std::string& name)
{
    if (!head_)
    {
        head_ = this;
    }

    // Single lookup; the child is only constructed when the label is new
    auto [it_child, inserted] = head_->children_.try_emplace(name);
    if (inserted)
    {
        // std::make_unique can't access the protected constructor
        it_child->second.reset(new Profiler(name, head_));
    }

    head_ = it_child->second.get();

    head_->timer_.start();
}

// ----------------------------------------------------------------------------------------------------

void Profiler::stopTimer()
{
    if (!head_ || head_ == this)
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
