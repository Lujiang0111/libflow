#ifndef LIBFLOW_INTERNAL_FLOW_FLOW_NODE_H_
#define LIBFLOW_INTERNAL_FLOW_FLOW_NODE_H_

#include <mutex>
#include <vector>
#include "libflow/flow.h"

namespace lflow
{

class FlowClass;
class FlowThread;

class FlowNode
{
public:
    FlowNode() = delete;
    FlowNode(const FlowNode &) = delete;
    FlowNode &operator=(const FlowNode &) = delete;

    explicit FlowNode(CreateFlowFunc create_flow_func);
    virtual ~FlowNode();

    bool Init(const char *param_str, void *opaque);
    void DeInit();
    bool Modify(const char *param_str, void *opaque);

    void EnqueuePacketList(std::shared_ptr<IPacketList> in_packet_list);
    bool Process();
    bool Control(const char *type, const char *param_str, void *opaque);

    void ConnectFlow(FlowNode *child_flow);
    void DisconnectFlow(FlowNode *child_flow);

    FlowClass *GetFlowClass();
    void SetFlowClass(FlowClass *flow_class);

    FlowThread *GetFlowThread();
    void SetFlowThread(FlowThread *flow_thread);

private:
    CreateFlowFunc create_flow_func_;
    std::shared_ptr<IFlow> flow_;

    std::mutex child_flow_mutex_;
    std::vector<FlowNode *> child_flows_;

    std::mutex in_packet_mutex_;
    std::shared_ptr<IPacketList> in_packet_list_;

    FlowClass *flow_class_;
    FlowThread *flow_thread_;
};

}

#endif // !LIBFLOW_INTERNAL_FLOW_FLOW_NODE_H_
