#include <cassert>
#include <iostream>
#include <regex>
#include <string>

long velocity_to_distance(long velocity) {
    return velocity * ((1.0 + velocity) / 2.0);
}

int main()
{
    static const std::regex INPUT_RX {"^target area: x=([0-9-]+)\\.\\.([0-9-]+), y=([0-9-]+)\\.\\.([0-9-]+)$"};

    std::string line;
    std::getline(std::cin, line);
    assert(!line.empty());
    std::smatch m;
    if (!std::regex_match(line, m, INPUT_RX)) {
        std::cout << "ERROR! Unknown command! [" << line << "]\n";
        exit(1);
    }
    long dst_x_from = std::stoul(m.str(1));
    long dst_x_to = std::stoul(m.str(2));
    long dst_y_from = std::stoul(m.str(3));
    long dst_y_to = std::stoul(m.str(4));

    std::cout << "target area: x=" << dst_x_from << ".." << dst_x_to << ", y=" << dst_y_from << ".." << dst_y_to
              << "\n";

    long velocity = 1;
    for (; velocity_to_distance(velocity) < dst_x_from; ++velocity) {
    }
    std::cout << "min velocity: " << velocity << ", distance: " << velocity_to_distance(velocity) << "\n";

    auto shoot = [&](long v, long s) -> std::pair<long, bool> {
        long max_y = 0;
        long x = 0, y = 0;
        bool hit = false;
        for(;;) {
            if (x >= dst_x_from && x <= dst_x_to && y >= dst_y_from && y <= dst_y_to) {
                hit = true;
                break;
            }
            if (x > dst_x_to) {
                break;
            }
            if (y < std::min(dst_y_from, dst_y_to)) {
                break;
            }

            x += v;
            y += s;
            max_y = std::max(y, max_y);

            if (v) {
                --v;
            }
            --s;
        }
        return {max_y, hit};
    };


    long max_y = 0, speed = 0;
    for(long s = 0; s < 200; ++s) {
        auto const& [yy, hit] = shoot(velocity, s);
        if (!hit) {
            // miss
            continue;
        }
        if (yy > max_y) {
            max_y = yy;
            speed = s;
        }
    }
    // std::cout << "Best speed: " << speed << ", with height: " << max_y << "\n";

    std::cout << "1: " << max_y << "\n";

    long count = 0;
    for (long v = velocity; v <= dst_x_to; ++v) {
        for (long s = -200; s < 200; ++s) {
            auto const&[_, hit] = shoot(v, s);
            if (hit) {
                ++count;
            }
        }
    }

    std::cout << "2: " << count << "\n";

    return 0;
}
