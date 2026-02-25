#ifndef TUE_CODE_PROFILER_PROFILING_PROFILER_H_
#define TUE_CODE_PROFILER_PROFILING_PROFILER_H_

#include "timer.h"
#include <map>
#include <string>

namespace tue
{

class Profiler
{

public:
    Profiler();

    Profiler(std::string name);

    virtual ~Profiler();

    void setName(const std::string& name) { name_ = name; }

    const std::string& getName() const { return name_; }

    void startTimer(const std::string& name);

    void stopTimer();

    friend std::ostream& operator<<(std::ostream& out, const Profiler& p);

protected:
    std::string name_;

    Profiler* head_;

    Profiler* parent_;

    Timer timer_;

    std::map<std::string, Profiler*> children_;

    Profiler(std::string name, Profiler* parent);

    void addToStream(std::ostream& out, const std::string& prefix = "") const;
};

} // namespace tue

#endif
