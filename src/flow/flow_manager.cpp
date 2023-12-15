#include "flow/flow_manager.h"

namespace lflow
{

FlowManager::FlowManager() :
    next_flow_id_(0)
{

}

FlowManager::~FlowManager()
{
    while (!flows_.empty())
    {
        DeleteFlow(flows_.begin()->first);
    }

    flow_classes_.clear();
}

bool FlowManager::RegisterFlowClass(const char *flow_class_name, CreateFlowFunc create_flow_func, size_t thread_bundle)
{
    std::string name = (flow_class_name) ? flow_class_name : "";
    std::lock_guard<std::mutex> lock(flow_class_mutex_);
    if (flow_classes_.end() != flow_classes_.find(name))
    {
        return false;
    }

    std::shared_ptr<FlowClass> flow_class = std::make_shared<FlowClass>(name, create_flow_func, thread_bundle);
    flow_classes_[name] = flow_class;
    return true;
}

FlowId FlowManager::CreateFlow(const char *flow_class_name, const char *param, void *opaque)
{
    FlowClass *flow_class = nullptr;
    {
        std::string name = (flow_class_name) ? flow_class_name : "";
        std::lock_guard<std::mutex> lock(flow_class_mutex_);
        auto &&it = flow_classes_.find(name);
        if (flow_classes_.end() == it)
        {
            return kInvalidFlowId;
        }

        flow_class = it->second.get();
    }

    std::shared_ptr<FlowNode> flow = flow_class->CreateFlow(param, opaque);

    FlowId flow_id = 0;
    {
        std::lock_guard<std::mutex> lock(flow_mutex_);
        flow_id = next_flow_id_++;
        flows_.emplace(flow_id, flow);
    }
    return flow_id;
}

void FlowManager::DeleteFlow(FlowId flow_id)
{
    std::lock_guard<std::mutex> lock(flow_mutex_);
    auto it = flows_.find(flow_id);
    if (flows_.end() == it)
    {
        return;
    }

    FlowNode *flow = it->second.get();
    flow->GetFlowClass()->DetachFromThread(flow);
    flows_.erase(it);
}

bool FlowManager::ControlFlow(FlowId flow_id, const char *type, const char *param, void *opaque)
{
    std::lock_guard<std::mutex> lock(flow_mutex_);
    auto it = flows_.find(flow_id);
    if (flows_.end() == it)
    {
        return false;
    }

    FlowNode *flow = it->second.get();
    return flow->Control(type, param, opaque);
}

bool FlowManager::ConnectFlow(FlowId parent_id, FlowId child_id)
{
    std::lock_guard<std::mutex> lock(flow_mutex_);
    auto &&parent_it = flows_.find(parent_id);
    if (flows_.end() == parent_it)
    {
        return false;
    }
    FlowNode *parent_flow = parent_it->second.get();

    auto &&child_it = flows_.find(child_id);
    if (flows_.end() == child_it)
    {
        return false;
    }
    FlowNode *child_flow = child_it->second.get();

    parent_flow->ConnectFlow(child_flow);
    return true;
}

void FlowManager::DisconnectFlow(FlowId parent_id, FlowId child_id)
{
    std::lock_guard<std::mutex> lock(flow_mutex_);
    auto &&parent_it = flows_.find(parent_id);
    if (flows_.end() == parent_it)
    {
        return;
    }
    FlowNode *parent_flow = parent_it->second.get();

    auto &&child_it = flows_.find(child_id);
    if (flows_.end() == child_it)
    {
        return;
    }
    FlowNode *child_flow = child_it->second.get();

    parent_flow->DisconnectFlow(child_flow);
}

void FlowManager::AttachToThread(FlowId flow_id)
{
    std::lock_guard<std::mutex> lock(flow_mutex_);
    auto &&it = flows_.find(flow_id);
    if (flows_.end() == it)
    {
        return;
    }

    FlowNode *flow = it->second.get();
    flow->GetFlowClass()->AttachToThread(flow);
}

void FlowManager::DetachFromThread(FlowId flow_id)
{
    std::lock_guard<std::mutex> lock(flow_mutex_);
    auto &&it = flows_.find(flow_id);
    if (flows_.end() == it)
    {
        return;
    }

    FlowNode *flow = it->second.get();
    flow->GetFlowClass()->DetachFromThread(flow);
}

std::shared_ptr<IFlowManager> CreateFlowManager()
{
    return std::make_shared<FlowManager>();
}

}
