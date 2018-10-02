#ifndef OLYLUMORAY_QUADRATIC_H
#define OLYLUMORAY_QUADRATIC_H

namespace olylumoray
{

class Quadratic
{
public:
    Quadratic(
        const float inA,
        const float inB,
        const float inC);

    float
    discriminant() const;

    float
    solution1() const;

    float
    solution2() const;

private:
    void
    calculate_discriminant();

private:
    float _a;
    float _b;
    float _c;
    float _discriminant;
};

} // namespace olylumoray

#endif // OLYLUMORAY_QUADRATIC_H
