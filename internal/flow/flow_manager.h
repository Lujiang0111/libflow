#ifndef LIBFLOW_INTERNAL_FLOW_FLOW_MANAGER_H_
#define LIBFLOW_INTERNAL_FLOW_FLOW_MANAGER_H_

#include <mutex>
#include <string>
#include <unordered_map>
#include "libflow/flow_manager.h"
#include "flow/flow_class.h"

namespace lflow
{

class FlowManager : public IFlowManager
{
public:
    FlowManager(const FlowManager &) = delete;
    FlowManager &operator=(const FlowManager &) = delete;

    FlowManager();
    virtual ~FlowManager();

    virtual bool RegisterFlowClass(const char *flow_class_name, CreateFlowFunc create_flow_func, size_t thread_bundle);

    virtual FlowId CreateFlow(const char *flow_class_name, const char *param_str, void *opaque);
    virtual void DeleteFlow(FlowId flow_id);

    virtual bool ControlFlow(FlowId flow_id, const char *type, const char *param_str, void *opaque);

    virtual bool ConnectFlow(FlowId parent_id, FlowId child_id);
    virtual void DisconnectFlow(FlowId parent_id, FlowId child_id);

    virtual void AttachToThread(FlowId flow_id);
    virtual void DetachFromThread(FlowId flow_id);

private:
    std::mutex flow_class_mutex_;
    std::unordered_map<std::string, std::shared_ptr<FlowClass>> flow_classes_;

    std::mutex flow_mutex_;
    std::unordered_map<FlowId, std::shared_ptr<FlowNode>> flows_;
    FlowId next_flow_id_;
};

}

#endif // !LIBFLOW_INTERNAL_FLOW_FLOW_MANAGER_H_
