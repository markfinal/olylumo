#ifndef OLYLUMORAY_RGBA_H
#define OLYLUMORAY_RGBA_H

#include <cstdint>

namespace olylumoray
{

class RGBA
{
public:
    void
    clear();

    void
    set(
        const float inRed,
        const float inGreen,
        const float inBlue,
        const float inAlpha);

private:
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
    uint8_t _alpha;
};

} // namespace olylumoray

#endif // OLYLUMORAY_RGBA_H
