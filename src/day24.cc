#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <random>
#include <string>

int64_t process_complet(std::string num)
{
    assert(num.size() == 14);

    if (num.find('0') != std::string::npos) {
        return -1;
    }

    auto it = num.cbegin();

    int64_t w = 0, x = 0, y = 0, z = 0;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 1;
    x += 13;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 5;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 1;
    x += 15;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 14;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 1;
    x += 15;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 15;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 1;
    x += 11;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 16;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 26;
    x += -16;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 8;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 26;
    x += -11;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 9;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 26;
    x += -6;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 2;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 1;
    x += 11;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 13;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 1;
    x += 10;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 16;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 26;
    x += -10;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 6;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 26;
    x += -8;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 6;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 26;
    x += -11;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 9;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 1;
    x += 12;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 11;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    w = *it++ - '0';
    x *= 0;
    x += z;
    x %= 26;
    z /= 26;
    x += -15;
    x = x == w;
    x = x == 0;
    y *= 0;
    y += 25;
    y *= x;
    y += 1;
    z *= y;
    y *= 0;
    y += w;
    y += 5;
    y *= x;
    z += y;

    // std::cout << "w: " << w << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;

    // std::cout << "num: " << num << ", x: " << x << ", y: " << y << ", z: " << z << "\n" << std::endl;

    assert(it == num.cend());

    return z;
}


int64_t digit(int w, int z)
{
    int64_t x = z % 26;
    z = z / 26;

    x = x - 15;

    if (x != w) {
        z = z * 26;
        z = z + (w + 5);
    }

    return z;
}

int main()
{
    // A=9
    // B=1
    // C=5
    // D=9
    // E=9
    // F=9
    // G=9
    // H=4
    // I=3
    // J=9
    // K=9
    // L=3
    // M=9
    // N=5

    std::string r1{"91599994399395"};
    if (process_complet(r1) == 0) {
        std::cout << "1: " << r1 << std::endl;
    }
    // A=7
    // B=1
    // C=1
    // D=1
    // E=1
    // F=5
    // G=9
    // H=1
    // I=1
    // J=7
    // K=6
    // L=1
    // M=5
    // N=1

    std::string r2{"71111591176151"};
    if (process_complet(r2) == 0) {
        std::cout << "2: " << r2 << std::endl;
    }

    return 0;
}
