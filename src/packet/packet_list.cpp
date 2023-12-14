#include "packet/packet_list.h"

namespace lflow
{

PacketList::PacketList()
{

}

PacketList::~PacketList()
{

}

std::shared_ptr<IPacketList> PacketList::Clone()
{
    std::shared_ptr<PacketList> rhs = std::make_shared<PacketList>();
    for (auto &&packet : packets_)
    {
        rhs->packets_.push_back(packet->Clone());
    }

    return rhs;
}

std::shared_ptr<IPacketList> PacketList::Reference()
{
    std::shared_ptr<PacketList> rhs = std::make_shared<PacketList>();
    for (auto &&packet : packets_)
    {
        rhs->packets_.push_back(packet->Reference());
    }

    return rhs;
}

void PacketList::PushBack(std::shared_ptr<IPacket> packet)
{
    packets_.push_back(packet);
}

std::shared_ptr<IPacket> PacketList::PopFront()
{
    auto packet = packets_.front();
    packets_.pop_front();
    return packet;
}

void PacketList::Merge(IPacketList *packet_list)
{
    if (!packet_list)
    {
        return;
    }

    while (packet_list->Size() > 0)
    {
        PushBack(packet_list->PopFront());
    }
}

IPacket *PacketList::GetIndex(size_t index)
{
    if (index >= packets_.size())
    {
        return nullptr;
    }

    return packets_[index].get();
}

void PacketList::EraseIndex(size_t index)
{
    if (index >= packets_.size())
    {
        return;
    }

    packets_.erase(packets_.begin() + index);
}

size_t PacketList::Size() const
{
    return packets_.size();
}

std::shared_ptr<IPacketList> CreatePacketList()
{
    return std::make_shared<PacketList>();
}

}
