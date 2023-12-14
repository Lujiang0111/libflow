#include <cstring>
#include "packet/packet.h"

namespace lflow
{

Packet::Packet(size_t size)
{
    data_ = std::make_shared<std::vector<uint8_t>>(size);
}

Packet::~Packet()
{

}

std::shared_ptr<IPacket> Packet::Clone()
{
    std::shared_ptr<Packet> rhs = std::make_shared<Packet>(0);
    *rhs->data_ = *data_;
    rhs->prop_ = prop_;
    return rhs;
}

std::shared_ptr<IPacket> Packet::Reference()
{
    std::shared_ptr<Packet> rhs = std::make_shared<Packet>(0);
    rhs->data_ = data_;
    rhs->prop_ = prop_;
    return rhs;
}

uint8_t *Packet::Buf()
{
    if (0 == data_->size())
    {
        return nullptr;
    }

    return &(*data_)[0];
}

size_t Packet::Size() const
{
    return data_->size();
}

void Packet::SetSize(size_t size)
{
    data_->resize(size);
}

bool Packet::GetProperty(int type, int64_t &value)
{
    auto &&iter = prop_.find(type);
    if (prop_.end() == iter)
    {
        return false;
    }

    value = iter->second;
    return true;
}

void Packet::SetProperty(int type, int64_t value)
{
    prop_[type] = value;
}

std::shared_ptr<IPacket> CreatePacket(size_t size)
{
    return std::make_shared<Packet>(size);
}

}
