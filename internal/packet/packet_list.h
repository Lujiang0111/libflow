#ifndef LIBFLOW_INTERNAL_PACKET_PACKET_LIST_H_
#define LIBFLOW_INTERNAL_PACKET_PACKET_LIST_H_

#include <deque>
#include "libflow/packet.h"

namespace lflow
{

class PacketList : public IPacketList
{
public:
    PacketList(const PacketList &) = delete;
    PacketList &operator=(const PacketList &) = delete;

    PacketList();
    virtual ~PacketList();

    virtual std::shared_ptr<IPacketList> Clone();
    virtual std::shared_ptr<IPacketList> Reference();

    virtual void PushBack(std::shared_ptr<IPacket> packet);
    virtual std::shared_ptr<IPacket> PopFront();
    virtual void Merge(IPacketList *packet_list);

    virtual IPacket *GetIndex(size_t index);
    virtual void EraseIndex(size_t index);

    virtual size_t Size() const;

private:
    std::deque<std::shared_ptr<IPacket>> packets_;
};

}

#endif // !LIBFLOW_INTERNAL_PACKET_PACKET_LIST_H_
