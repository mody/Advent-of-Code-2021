#include <cassert>
#include <iostream>
#include <iterator>
#include <limits>
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

    virtual int64_t get_value() const = 0;
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
        for (bool last = false; !last;) {
            last = !*it++;
            int val = 0;
            val |= ((*it++ ? 1 : 0) << 3);
            val |= ((*it++ ? 1 : 0) << 2);
            val |= ((*it++ ? 1 : 0) << 1);
            val |= ((*it++ ? 1 : 0) << 0);
            value <<= 4;
            value |= val;
        }
        return it;
    }

    int sum_versions() const override
    {
        return version;
    }

    int64_t get_value() const override
    {
        return value;
    }

protected:
    uint64_t value = 0;
};

struct Operator : public Packet
{
    Operator(int version, int id);

    int sum_versions() const override;
    int64_t get_value() const override;

protected:
    std::vector<PacketPtr> packets;
};

struct OperatorRaw : public Operator
{
    OperatorRaw(int version, int id);
    Data::iterator parse(Data::iterator it) override;
};

struct OperatorRepeat : public Operator
{
    OperatorRepeat(int version, int id);
    Data::iterator parse(Data::iterator it) override;
};

Operator::Operator(int version, int id)
    : Packet(version, id)
{ }

int64_t Operator::get_value() const
{
    int64_t result = 0;
    if (id == 0) {
        for (auto const& ptr : packets) {
            result += ptr->get_value();
        }
    } else if (id == 1) {
        result = 1;
        for (auto const& ptr : packets) {
            result *= ptr->get_value();
        }
    } else if (id == 2) {
        result = std::numeric_limits<int>::max();
        for (auto const& ptr : packets) {
            result = std::min(result, ptr->get_value());
        }
    } else if (id == 3) {
        result = std::numeric_limits<int>::min();
        for (auto const& ptr : packets) {
            result = std::max(result, ptr->get_value());
        }
    } else if (id == 5) {
        assert(packets.size() == 2);
        result = packets.at(0)->get_value() > packets.at(1)->get_value() ? 1 : 0;
    } else if (id == 6) {
        assert(packets.size() == 2);
        result = packets.at(0)->get_value() < packets.at(1)->get_value() ? 1 : 0;
    } else if (id == 7) {
        assert(packets.size() == 2);
        result = packets.at(0)->get_value() == packets.at(1)->get_value() ? 1 : 0;
    } else {
        assert(false);
    }
    return result;
}

int Operator::sum_versions() const
{
    int result = version;
    for (auto const& ptr : packets) {
        result += ptr->sum_versions();
    }
    return result;
}

OperatorRaw::OperatorRaw(int version, int id)
    : Operator(version, id)
{ }

Data::iterator OperatorRaw::parse(Data::iterator it)
{
    unsigned val = 0;
    for (int i = 0; i < 15; ++i) {
        val <<= 1;
        val |= (*it++ ? 1 : 0);
    }

    auto from = it;

    for (; std::distance(from, it) < val;) {
        PacketPtr ptr;

        auto const& [version, id] = parse_header(it);

        if (id == 4) {
            ptr = std::make_shared<LiteralValue>(version, id);
        } else {
            if (!*it++) {
                ptr = std::make_shared<OperatorRaw>(version, id);
            } else {
                ptr = std::make_shared<OperatorRepeat>(version, id);
            }
        }
        it = ptr->parse(it);
        packets.push_back(std::move(ptr));
    }

    return it;
}


OperatorRepeat::OperatorRepeat(int version, int id)
    : Operator(version, id)
{ }

Data::iterator OperatorRepeat::parse(Data::iterator it)
{
    unsigned val = 0;
    for (int i = 0; i < 11; ++i) {
        val <<= 1;
        val |= (*it++ ? 1 : 0);
    }

    for (unsigned i = 0; i < val; ++i) {
        PacketPtr ptr;

        auto const& [version, id] = parse_header(it);

        if (id == 4) {
            ptr = std::make_shared<LiteralValue>(version, id);
        } else {
            if (!*it++) {
                ptr = std::make_shared<OperatorRaw>(version, id);
            } else {
                ptr = std::make_shared<OperatorRepeat>(version, id);
            }
        }
        it = ptr->parse(it);
        packets.push_back(std::move(ptr));
    }

    return it;
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

    // for (auto const b : data) {
    //     std::cout << (b?'1':'0');
    // }
    // std::cout << "\n";

    std::vector<PacketPtr> packets;

    for (auto it = data.begin(); it != data.end();) {
        auto const& [version, id] = parse_header(it);
        if (version == 0 && id == 0) {
            break;
        }

        PacketPtr ptr;
        if (id == 4) {
            ptr = std::make_shared<LiteralValue>(version, id);
        } else {
            if (!*it++) {
                ptr = std::make_shared<OperatorRaw>(version, id);
            } else {
                ptr = std::make_shared<OperatorRepeat>(version, id);
            }
        }
        it = ptr->parse(it);
        packets.push_back(std::move(ptr));
    }

    int result1 = 0;
    for (auto const& ptr : packets) {
        result1 += ptr->sum_versions();
    }
    std::cout << "1: " << result1 << "\n";

    int64_t result2 = 0;
    for (auto const& ptr : packets) {
        result2 = ptr->get_value();
    }
    std::cout << "2: " << result2 << "\n";

    return 0;
}
