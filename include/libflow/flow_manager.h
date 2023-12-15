#ifndef LIBFLOW_INCLUDE_LIBFLOW_FLOW_MANAGER_H_
#define LIBFLOW_INCLUDE_LIBFLOW_FLOW_MANAGER_H_

#include "libflow/flow.h"

namespace lflow
{

using FlowId = int;

constexpr FlowId kInvalidFlowId = -1;

class IFlowManager
{
public:
    virtual ~IFlowManager() = default;

    virtual bool RegisterFlowClass(const char *flow_class_name, CreateFlowFunc create_flow_func, size_t thread_bundle) = 0;

    virtual FlowId CreateFlow(const char *flow_class_name, const char *param, void *opaque) = 0;
    virtual void DeleteFlow(FlowId flow_id) = 0;

    virtual bool ConnectFlow(FlowId parent_id, FlowId child_id) = 0;
    virtual void DisconnectFlow(FlowId parent_id, FlowId child_id) = 0;

    virtual void AttachToThread(FlowId flow_id) = 0;
    virtual void DetachFromThread(FlowId flow_id) = 0;
};

LIBFLOW_API std::shared_ptr<IFlowManager> CreateFlowManager();

}

#endif // !LIBFLOW_INCLUDE_LIBFLOW_FLOW_MANAGER_H_
