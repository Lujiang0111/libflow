#include <algorithm>
#include "flow/flow_class.h"

namespace lflow
{

FlowClass::FlowClass(const std::string &name, CreateFlowFunc create_flow_func, size_t thread_bundle) :
    name_(name),
    create_flow_func_(create_flow_func),
    thread_bundle_(thread_bundle)
{

}

FlowClass::~FlowClass()
{

}

std::shared_ptr<FlowNode> FlowClass::CreateFlow(const char *param, void *opaque)
{
    std::shared_ptr<FlowNode> flow = std::make_shared<FlowNode>(create_flow_func_);
    if (!flow->Init(param, opaque))
    {
        return nullptr;
    }
    flow->SetFlowClass(this);
    return flow;
}

void FlowClass::AttachToThread(FlowNode *flow)
{
    if (flow->GetFlowThread())
    {
        return;
    }

    if (unfilled_threads_.size() > 0)
    {
        FlowThread *flow_thread = unfilled_threads_.front();
        if (thread_bundle_ == flow_thread->PushBackFlow(flow))
        {
            unfilled_threads_.pop_front();
        }
        return;
    }

    if (idle_threads_.empty())
    {
        std::string name = name_;
        name += "_";
        name += std::to_string(threads_.size());

        std::shared_ptr<FlowThread> flow_thread = std::make_shared<FlowThread>(name);
        threads_.push_back(flow_thread);
        idle_threads_.push_front(flow_thread.get());
    }

    FlowThread *flow_thread = idle_threads_.front();
    flow_thread->StartThread();

    idle_threads_.pop_front();
    if (thread_bundle_ != flow_thread->PushBackFlow(flow))
    {
        unfilled_threads_.push_back(flow_thread);
    }
}

void FlowClass::DetachFromThread(FlowNode *flow)
{
    FlowThread *flow_thread = flow->GetFlowThread();
    if (!flow_thread)
    {
        return;
    }

    size_t thread_bundle = flow_thread->RemoveFlow(flow);
    if (0 == thread_bundle)
    {
        flow_thread->StopThread();
        idle_threads_.push_back(flow_thread);

        auto unfilled_threads_it = std::find(unfilled_threads_.begin(), unfilled_threads_.end(), flow_thread);
        if (unfilled_threads_.end() != unfilled_threads_it)
        {
            unfilled_threads_.erase(unfilled_threads_it);
        }
    }
    else if (thread_bundle_ - 1 == thread_bundle)
    {
        unfilled_threads_.push_back(flow_thread);
    }
}

}
