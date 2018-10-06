#include "olylumoray/utils.h"

#include <random>

namespace
{

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> minus_one_one_dist(-1, 1);
std::uniform_real_distribution<float> zero_one_dist(0, 1);

} // anonymous namespace

namespace olylumoray
{

float
random_between_zero_and_one()
{
    return zero_one_dist(gen);
}

float
random_between_minus_one_and_one()
{
    return minus_one_one_dist(gen);
}

Vec4
random_in_unit_sphere()
{
    Vec4 p;
    do
    {
        p = Vec4(minus_one_one_dist(gen), minus_one_one_dist(gen), minus_one_one_dist(gen), 0);
    }
    while (p.squared_length() >= 1.0f);
    return p;
}

Vec4
reflect(
    const Vec4 &inV,
    const Vec4 &inN)
{
    return inV - inN * 2 * inV.dot(inN);
}

} // namespace olylumoray
