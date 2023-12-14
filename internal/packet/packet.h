#ifndef LIBFLOW_INTERNAL_PACKET_PACKET_H_
#define LIBFLOW_INTERNAL_PACKET_PACKET_H_

#include <unordered_map>
#include <vector>
#include "libflow/packet.h"

namespace lflow
{

class Packet : public IPacket
{
public:
    Packet() = delete;
    Packet(const Packet &) = delete;
    Packet &operator=(const Packet &) = delete;

    explicit Packet(size_t size);
    virtual ~Packet();

    virtual std::shared_ptr<IPacket> Clone();
    virtual std::shared_ptr<IPacket> Reference();

    virtual uint8_t *Buf();

    virtual size_t Size() const;
    virtual void SetSize(size_t size);

    virtual bool GetProperty(int type, int64_t &value);
    virtual void SetProperty(int type, int64_t value);

private:
    std::shared_ptr<std::vector<uint8_t>> data_;
    std::unordered_map<int, int64_t> prop_;
};

}

#endif // !LIBFLOW_INTERNAL_PACKET_PACKET_H_
