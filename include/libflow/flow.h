#ifndef LIBFLOW_INCLUDE_LIBFLOW_FLOW_H_
#define LIBFLOW_INCLUDE_LIBFLOW_FLOW_H_

#include "libflow/packet.h"

namespace lflow
{

class IFlow
{
public:
    virtual ~IFlow() = default;

    virtual bool Init(const char *param, void *opaque) = 0;
    virtual void DeInit() = 0;
    virtual bool Modify(const char *param, void *opaque) = 0;

    virtual std::shared_ptr<IPacketList> Process(std::shared_ptr<IPacketList> in_packet_list) = 0;
    virtual bool Control(const char *type, const char *param, void *opaque) = 0;
};

using CreateFlowFunc = std::shared_ptr<IFlow>(*)();

}

#endif // !LIBFLOW_INCLUDE_LIBFLOW_FLOW_H_
