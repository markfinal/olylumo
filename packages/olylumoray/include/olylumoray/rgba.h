#ifndef OLYLUMORAY_RGBA_H
#define OLYLUMORAY_RGBA_H

namespace olylumoray
{

class RGBA
{
public:
    RGBA();
    RGBA(const float inRed, const float inGreen, const float inBlue, const float inAlpha);

    RGBA operator+(const RGBA &inOther) const;
    RGBA operator*(const float inScale) const;

    void
    clear();

    void
    make_opaque();

private:
    float _red;
    float _green;
    float _blue;
    float _alpha;
};

} // namespace olylumoray

#endif // OLYLUMORAY_RGBA_H
