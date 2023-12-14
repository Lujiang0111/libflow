#ifndef LIBFLOW_INCLUDE_LIBFLOW_PACKET_H_
#define LIBFLOW_INCLUDE_LIBFLOW_PACKET_H_

#include "libflow/common.h"

namespace lflow
{

class IPacket
{
public:
    virtual ~IPacket() = default;

    virtual std::shared_ptr<IPacket> Clone() = 0;
    virtual std::shared_ptr<IPacket> Reference() = 0;

    virtual uint8_t *Buf() = 0;
    
    virtual size_t Size() const = 0;
    virtual void SetSize(size_t size) = 0;

    virtual bool GetProperty(int type, int64_t &value) = 0;
    virtual void SetProperty(int type, int64_t value) = 0;
};

LIBFLOW_API std::shared_ptr<IPacket> CreatePacket(size_t size);

class IPacketList
{
public:
    virtual ~IPacketList() = default;

    virtual std::shared_ptr<IPacketList> Clone() = 0;
    virtual std::shared_ptr<IPacketList> Reference() = 0;

    virtual void PushBack(std::shared_ptr<IPacket> packet) = 0;
    virtual std::shared_ptr<IPacket> PopFront() = 0;
    virtual void Merge(IPacketList *packet_list) = 0;

    virtual IPacket *GetIndex(size_t index) = 0;
    virtual void EraseIndex(size_t index) = 0;

    virtual size_t Size() const = 0;
};

LIBFLOW_API std::shared_ptr<IPacketList> CreatePacketList();

}

#endif // !LIBFLOW_INCLUDE_LIBFLOW_PACKET_H_
