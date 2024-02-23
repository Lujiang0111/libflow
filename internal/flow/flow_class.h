#ifndef LIBFLOW_INTERNAL_FLOW_FLOW_CLASS_H_
#define LIBFLOW_INTERNAL_FLOW_FLOW_CLASS_H_

#include <deque>
#include <string>
#include "flow/flow_thread.h"

namespace lflow
{

class FlowClass
{
public:
    FlowClass() = delete;
    FlowClass(const FlowClass &) = delete;
    FlowClass &operator=(const FlowClass &) = delete;

    FlowClass(const std::string &name, CreateFlowFunc create_flow_func, size_t thread_bundle);
    virtual ~FlowClass();

    std::shared_ptr<FlowNode> CreateFlow(const char *param_str, void *opaque);

    void AttachToThread(FlowNode *flow);
    void DetachFromThread(FlowNode *flow);

private:
    std::string name_;
    CreateFlowFunc create_flow_func_;
    size_t thread_bundle_;

    std::deque<std::shared_ptr<FlowThread>> threads_;
    std::deque<FlowThread *> unfilled_threads_;
    std::deque<FlowThread *> idle_threads_;
};

}

#endif // !LIBFLOW_INTERNAL_FLOW_FLOW_CLASS_H_
