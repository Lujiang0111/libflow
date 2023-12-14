#ifndef LIBFLOW_INTERNAL_FLOW_FLOW_THREAD_H_
#define LIBFLOW_INTERNAL_FLOW_FLOW_THREAD_H_

#include <deque>
#include <future>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "flow/flow_node.h"

namespace lflow
{

class FlowThread
{
public:
    FlowThread() = delete;
    FlowThread(const FlowThread &) = delete;
    FlowThread &operator=(const FlowThread &) = delete;

    explicit FlowThread(const std::string &name);
    virtual ~FlowThread();

    size_t PushBackFlow(FlowNode *flow);
    size_t RemoveFlow(FlowNode *flow);

    void StartThread();
    void StopThread();

private:
    enum class OptTypes
    {
        kPushBackFlow = 0,
        kRemoveFlow,
    };

    struct Opt
    {
        OptTypes type = OptTypes::kPushBackFlow;
        FlowNode *flow = nullptr;
        std::promise<bool> promise;
    };

private:
    void WorkThread();

    void EnqueueOpt(std::shared_ptr<Opt> opt);

    bool DoOpts();
    void DoPushBackFlow(Opt *opt);
    void DoRemoveFlow(Opt *opt);

private:
    std::string name_;

    std::vector<FlowNode *> flows_;

    std::thread work_thread_;
    bool work_thread_running_;

    std::mutex opt_mutex_;
    bool has_opt_;
    std::deque<std::shared_ptr<Opt>> opts_;
};

}

#endif // !LIBFLOW_INTERNAL_FLOW_FLOW_THREAD_H_
