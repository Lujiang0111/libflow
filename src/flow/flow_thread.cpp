#include <algorithm>
#include "flow/flow_thread.h"

namespace lflow
{

FlowThread::FlowThread(const std::string &name) :
    name_(name),
    work_thread_running_(false),
    has_opt_(false)
{

}

FlowThread::~FlowThread()
{
    StopThread();
}

size_t FlowThread::PushBackFlow(FlowNode *flow)
{
    std::shared_ptr<Opt> opt = std::make_shared<Opt>();
    opt->type = OptTypes::kPushBackFlow;
    opt->flow = flow;

    auto fut = opt->promise.get_future();
    EnqueueOpt(opt);
    fut.get();

    return flows_.size();
}

size_t FlowThread::RemoveFlow(FlowNode *flow)
{
    std::shared_ptr<Opt> opt = std::make_shared<Opt>();
    opt->type = OptTypes::kRemoveFlow;
    opt->flow = flow;

    auto fut = opt->promise.get_future();
    EnqueueOpt(opt);
    fut.get();

    return flows_.size();
}

void FlowThread::StartThread()
{
    work_thread_running_ = true;
    work_thread_ = std::thread(&FlowThread::WorkThread, this);

#ifdef _WIN32
#else
    std::string thread_name = "lflow_";
    thread_name += name_;
    pthread_setname_np(work_thread_.native_handle(), thread_name.c_str());
#endif
}

void FlowThread::StopThread()
{
    work_thread_running_ = false;
    if (work_thread_.joinable())
    {
        work_thread_.join();
    }
}

void FlowThread::WorkThread()
{
    bool is_busy = false;
    while (work_thread_running_)
    {
        is_busy = false;
        for (auto &&flow : flows_)
        {
            if (flow->Process())
            {
                is_busy = true;
            }
        }

        if (DoOpts())
        {
            is_busy = true;
        }

        if (!is_busy)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    DoOpts();
}

void FlowThread::EnqueueOpt(std::shared_ptr<Opt> opt)
{
    std::lock_guard<std::mutex> lock(opt_mutex_);
    opts_.push_back(opt);
    has_opt_ = true;
}

bool FlowThread::DoOpts()
{
    if (!has_opt_)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(opt_mutex_);
    for (auto &&opt : opts_)
    {
        switch (opt->type)
        {
        case OptTypes::kPushBackFlow:
        {
            DoPushBackFlow(opt.get());
            break;
        }

        case OptTypes::kRemoveFlow:
        {
            DoRemoveFlow(opt.get());
            break;
        }

        default:
            break;
        }
    }

    opts_.clear();
    has_opt_ = false;
    return true;
}

void FlowThread::DoPushBackFlow(Opt *opt)
{
    flows_.push_back(opt->flow);
    opt->flow->SetFlowThread(this);
    opt->promise.set_value(true);
}

void FlowThread::DoRemoveFlow(Opt *opt)
{
    opt->flow->SetFlowThread(nullptr);

    auto flow_it = std::find(flows_.begin(), flows_.end(), opt->flow);
    if (flows_.end() == flow_it)
    {
        opt->promise.set_value(true);
        return;
    }

    flows_.erase(flow_it);
    opt->promise.set_value(true);
}

}
