#include <algorithm>
#include "flow/flow_node.h"

namespace lflow
{

FlowNode::FlowNode(CreateFlowFunc create_flow_func) :
    create_flow_func_(create_flow_func),
    flow_class_(nullptr),
    flow_thread_(nullptr)
{

}

FlowNode::~FlowNode()
{
    DeInit();
}

bool FlowNode::Init(const char *param_str, void *opaque)
{
    flow_ = create_flow_func_();
    if (!flow_)
    {
        return false;
    }

    if (!flow_->Init(param_str, opaque))
    {
        return false;
    }

    return true;
}

void FlowNode::DeInit()
{
    if (!flow_)
    {
        return;
    }

    flow_->DeInit();
    flow_ = nullptr;
}

bool FlowNode::Modify(const char *param_str, void *opaque)
{
    if (!flow_)
    {
        return Init(param_str, opaque);
    }

    return flow_->Modify(param_str, opaque);
}

void FlowNode::EnqueuePacketList(std::shared_ptr<IPacketList> in_packet_list)
{
    if ((!in_packet_list) || (0 == in_packet_list->Size()))
    {
        return;
    }

    std::lock_guard<std::mutex> lock(in_packet_mutex_);
    if (!in_packet_list_)
    {
        in_packet_list_ = CreatePacketList();
    }
    in_packet_list_->Merge(in_packet_list.get());
}

bool FlowNode::Process()
{
    bool ret = false;

    std::shared_ptr<IPacketList> in_packet_list = nullptr;
    {
        std::lock_guard<std::mutex> lock(in_packet_mutex_);
        in_packet_list = in_packet_list_;
        in_packet_list_ = nullptr;
    }

    if ((in_packet_list) && (in_packet_list->Size() > 0))
    {
        ret = true;
    }

    std::shared_ptr<IPacketList> out_packet_list = flow_->Process(in_packet_list);

    if ((out_packet_list) && (out_packet_list->Size() > 0))
    {
        ret = true;
        std::lock_guard<std::mutex> lock(child_flow_mutex_);
        for (auto &&flow : child_flows_)
        {
            auto ref_packet_list = out_packet_list->Reference();
            flow->EnqueuePacketList(ref_packet_list);
        }
    }
    out_packet_list = nullptr;

    return ret;
}

bool FlowNode::Control(const char *type, const char *param_str, void *opaque)
{
    return flow_->Control(type, param_str, opaque);
}

void FlowNode::ConnectFlow(FlowNode *child_flow)
{
    std::lock_guard<std::mutex> lock(child_flow_mutex_);
    auto &&child_flow_it = std::find(child_flows_.begin(), child_flows_.end(), child_flow);
    if (child_flows_.end() != child_flow_it)
    {
        return;
    }

    child_flows_.push_back(child_flow);
}

void FlowNode::DisconnectFlow(FlowNode *child_flow)
{
    std::lock_guard<std::mutex> lock(child_flow_mutex_);
    auto child_flow_it = std::find(child_flows_.begin(), child_flows_.end(), child_flow);
    if (child_flows_.end() == child_flow_it)
    {
        return;
    }

    child_flows_.erase(child_flow_it);
}

FlowClass *FlowNode::GetFlowClass()
{
    return flow_class_;
}

void FlowNode::SetFlowClass(FlowClass *flow_class)
{
    flow_class_ = flow_class;
}

FlowThread *FlowNode::GetFlowThread()
{
    return flow_thread_;
}

void FlowNode::SetFlowThread(FlowThread *flow_thread)
{
    flow_thread_ = flow_thread;
}

}
