#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

using Data = std::vector<bool>;

namespace {

int from_char(char c)
{
    int r = 0;
    if (c >= '0' && c <= '9') {
        r = c - '0';
    } else if (c >= 'A' || c <= 'F') {
        r = (c - 'A') + 10;
    } else if (c >= 'a' || c <= 'f') {
        r = (c - 'a') + 10;
    }
    return r;
}


std::pair<int, int> parse_header(Data::iterator& it)
{
    int version = 0;
    version |= ((*it++ ? 1 : 0) << 2);
    version |= ((*it++ ? 1 : 0) << 1);
    version |= ((*it++ ? 1 : 0) << 0);

    int id = 0;
    id |= ((*it++ ? 1 : 0) << 2);
    id |= ((*it++ ? 1 : 0) << 1);
    id |= ((*it++ ? 1 : 0) << 0);

    std::cout << "Parsed header: " << version << ", ID: " << id << "\n";
    return {version, id};
};


}  // namespace


struct Packet
{
    Packet(int _version, int _id)
        : version {_version}
        , id {_id}
    { }
    virtual ~Packet() = default;
    virtual Data::iterator parse(Data::iterator it) = 0;

    // virtual int get_value() const = 0;
    virtual int sum_versions() const = 0;

    int get_version() const
    {
        return version;
    }
    int get_id() const
    {
        return id;
    }

protected:
    int version = 0;
    int id = 0;
};

using PacketPtr = std::shared_ptr<Packet>;


struct LiteralValue : public Packet
{
    LiteralValue(int version, int id)
        : Packet(version, id)
    { }

    Data::iterator parse(Data::iterator it) override
    {
        auto bck = it;
        // int bits = 6;
        for (bool last = false; !last;) {
            last = !*it++;
            int val = 0;
            val |= ((*it++ ? 1 : 0) << 3);
            val |= ((*it++ ? 1 : 0) << 2);
            val |= ((*it++ ? 1 : 0) << 1);
            val |= ((*it++ ? 1 : 0) << 0);
            value <<= 4;
            value |= val;
            // bits += 5;
        }
        std::cout << "Literal value: " << value << "\n";

        std::cout << "parsed " << std::distance(bck, it) << " bits\n";

        // skip padding
        // if (bits % 8 != 0) {
        //     std::advance(it, 8 - (bits % 8));
        // }
        return it;
    }

    int sum_versions() const override
    {
        return version;
    }

protected:
    uint64_t value = 0;
};


struct OperatorRaw : public Packet
{
    OperatorRaw(int version, int id);
    Data::iterator parse(Data::iterator it) override;

    int sum_versions() const override;

protected:
    std::vector<PacketPtr> packets;
};


struct OperatorRepeat : public Packet
{
    OperatorRepeat(int version, int id);
    Data::iterator parse(Data::iterator it) override;

    int sum_versions() const override;

protected:
    std::vector<PacketPtr> packets;
};


OperatorRaw::OperatorRaw(int version, int id)
    : Packet(version, id)
{ }

Data::iterator OperatorRaw::parse(Data::iterator it)
{
    unsigned val = 0;
    for (int i = 0; i < 15; ++i) {
        val <<= 1;
        val |= (*it++ ? 1 : 0);
    }
    std::cout << "Raw data size: " << val << "\n";

    auto from = it;

    for (; std::distance(from, it) < val;) {
        PacketPtr ptr;
        auto bck = it;

        auto const& [version, id] = parse_header(it);
        // if (version == 0 && id == 0) {
        //     break;
        // }

        if (id == 4) {
            std::cout << "[RAW] Found LiteralValue\n";
            ptr = std::make_shared<LiteralValue>(version, id);
        } else {
            // operators
            if (!*it++) {
                // raw bits, length in next 15bits
                std::cout << "[RAW] Found OperatorRaw\n";
                ptr = std::make_shared<OperatorRaw>(version, id);
            } else {
                // next 11 bits are a number that represents the number of sub-packets immediately contained
                std::cout << "[RAW] Found OperatorRepeat\n";
                ptr = std::make_shared<OperatorRepeat>(version, id);
            }
        }
        it = ptr->parse(it);
        packets.push_back(std::move(ptr));
        std::cout << "parsed " << std::distance(bck, it) << " bits\n";
    }

    // skip padding
    // bits += std::distance(from, it);
    // if (bits % 8 != 0) {
    //     std::advance(it, 8 - (bits % 8));
    // }

    return it;
}

int OperatorRaw::sum_versions() const
{
    int result = version;
    for (auto const& ptr : packets) {
        result += ptr->sum_versions();
    }
    return result;
}

OperatorRepeat::OperatorRepeat(int version, int id)
    : Packet(version, id)
{ }

Data::iterator OperatorRepeat::parse(Data::iterator it)
{
    unsigned val = 0;
    for (int i = 0; i < 11; ++i) {
        val <<= 1;
        val |= (*it++ ? 1 : 0);
    }
    std::cout << "Messages: " << val << "\n";

    for (unsigned i = 0; i < val; ++i) {
        PacketPtr ptr;
        auto bck = it;

        auto const& [version, id] = parse_header(it);
        // if (version == 0 && id == 0) {
        //     break;
        // }

        if (id == 4) {
            std::cout << "[REP] Found LiteralValue\n";
            ptr = std::make_shared<LiteralValue>(version, id);
        } else {
            // operators
            if (!*it++) {
                // raw bits, length in next 15bits
                std::cout << "[REP] Found OperatorRaw\n";
                ptr = std::make_shared<OperatorRaw>(version, id);
            } else {
                // next 11 bits are a number that represents the number of sub-packets immediately contained
                std::cout << "[REP] Found OperatorRepeat\n";
                ptr = std::make_shared<OperatorRepeat>(version, id);
            }
        }
        it = ptr->parse(it);
        packets.push_back(std::move(ptr));
        std::cout << "parsed " << std::distance(bck, it) << " bits\n";
    }

    return it;
}


int OperatorRepeat::sum_versions() const
{
    int result = version;
    for (auto const& ptr : packets) {
        result += ptr->sum_versions();
    }
    return result;
}

int main()
{
    std::vector<bool> data;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        for (char c : line) {
            int r = from_char(c);
            data.push_back((r & 0x8) != 0);
            data.push_back((r & 0x4) != 0);
            data.push_back((r & 0x2) != 0);
            data.push_back((r & 0x1) != 0);
        }
    }

    for (auto const b : data) {
        std::cout << (b?'1':'0');
    }
    std::cout << "\n";

    std::vector<PacketPtr> packets;

    for (auto it = data.begin(); it != data.end();) {
        auto const& [version, id] = parse_header(it);
        if (version == 0 && id == 0) {
            break;
        }

        PacketPtr ptr;
        if (id == 4) {
            std::cout << "Found LiteralValue\n";
            ptr = std::make_shared<LiteralValue>(version, id);
        } else {
            // operators
            if (!*it++) {
                // raw bits, length in next 15bits
                std::cout << "Found OperatorRaw\n";
                ptr = std::make_shared<OperatorRaw>(version, id);
            } else {
                // next 11 bits are a number that represents the number of sub-packets immediately contained
                std::cout << "Found OperatorRepeat\n";
                ptr = std::make_shared<OperatorRepeat>(version, id);
            }
        }
        it = ptr->parse(it);
        packets.push_back(std::move(ptr));
    }

    std::cout << "Loaded " << packets.size() << " packets\n";

    int result1 = 0;
    for (auto const& ptr : packets) {
        result1 += ptr->sum_versions();
    }

    std::cout << "1: " << result1 << "\n";

    return 0;
}
