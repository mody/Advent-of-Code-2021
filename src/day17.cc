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
        // std::cout << "velocity: " << velocity << ", distance: " << velocity_to_distance(velocity) << "\n";
    }
    std::cout << "min velocity: " << velocity << ", distance: " << velocity_to_distance(velocity) << "\n";

    auto shoot = [&](long v, long s) -> long {
        long max_y = 0;
        long x = 0, y = 0;
        bool hit = false;
        for(;;) {
            // std::cout << "shoot(" << v << ", " << s << ") (" << x << ", " << y << ")\n";
            if (x >= dst_x_from && x <= dst_x_to && y >= dst_y_from && y <= dst_y_to) {
                // std::cout << "hit!\n";
                hit = true;
                break;
            }
            if (x > dst_x_to) {
                // std::cout << "overshoot!\n";
                break;
            }
            if (y < dst_y_to) {
                // std::cout << "too low!\n";
                break;
            }
            max_y = std::max(y, max_y);

            x += v;
            if (v) {
                --v;
            }
            y += s;
            --s;
        }
        if (!hit) {
            max_y = 0;
        }
        return max_y;
    };


    long speed = 1;
    long max_y = 0, max_speed = 0;
    for(; speed < 200; ++speed) {
        long yy = shoot(velocity, speed);
        if (yy == 0) {
            // miss
            continue;
        }
        if (yy > max_y) {
            max_y = yy;
            max_speed = speed;
        }
    }
    std::cout << "Best speed: " << max_speed << ", with height: " << max_y << "\n";

    std::cout << "1: " << max_y << "\n";

    return 0;
}
