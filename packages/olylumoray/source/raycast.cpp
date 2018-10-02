#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"
#include "olylumoray/vec4.h"
#include "olylumoray/ray.h"

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
    determinant() const;

private:
    void
    calculate_determinant();

private:
    float _a;
    float _b;
    float _c;
    float _determinant;
};

Quadratic::Quadratic(
    const float inA,
    const float inB,
    const float inC)
    :
    _a(inA),
    _b(inB),
    _c(inC)
{
    this->calculate_determinant();
}

float
Quadratic::determinant() const
{
    return this->_determinant;
}

void
Quadratic::calculate_determinant()
{
    // b^2 - 4 * a * c
    this->_determinant = this->_b * this->_b - 4 * this->_a * this->_c;
}

bool
ray_intersect_sphere(
    const Vec4 &inSphereOrigin,
    const float inSphereRadius,
    const Ray &inRay)
{
    const auto distance = inRay.origin() - inSphereOrigin;
    Quadratic q(
        inRay.direction().dot(inRay.direction()),
        2 * inRay.direction().dot(distance),
        distance.dot(distance) - inSphereRadius * inSphereRadius
    );
    return q.determinant() > 0;
}

RGBA
colour(
    const Ray &inRay)
{
    if (ray_intersect_sphere({0,0,1,1}, 0.5f, inRay))
    {
        return RGBA(1, 0, 0, 1);
    }
    const auto unit = inRay.direction().normalise();
    const auto t = 0.5f * (unit.y() + 1); // rescale [-1,1] to [0,1]
    const auto lerp = RGBA(1, 1, 1, 1) * (1.0f - t) + RGBA(0.5f, 0.7f, 1.0f, 1.0f) * t;
    return lerp;
}

std::unique_ptr<Image>
raycast()
{
    const auto width = 640u;
    const auto height = 480u;
    std::unique_ptr<Image> image(new Image(width, height));

    // use left-handed coordinate system:
    // right   -> +x
    // up      -> +y
    // forward -> +z

    // define a camera image plane
    Vec4 camera_image_plane_bottom_left(-1.33f, -1.0f, +1.0f, 0.0f); // used as direction
    Vec4 camera_image_plane_horizonal(2.66f, 0.0f, 0.0f, 0.0f); // direction
    Vec4 camera_image_plane_vertical(0, 2.0f, 0, 0.0f); // direction
    Vec4 camera_origin(0, 0, 0, 1); // position

    auto current_pixel = image->pixels();
    for (auto row = 0u; row < height; ++row)
    {
        const auto v = static_cast<float>(row) / height;
        for (auto col = 0u; col < width; ++col)
        {
            const auto u = static_cast<float>(col) / width;

            Ray ray(
                camera_origin,
                camera_image_plane_bottom_left + camera_image_plane_horizonal * u + camera_image_plane_vertical * v
            );
            *current_pixel = colour(ray);
            current_pixel->make_opaque();
            current_pixel++;
        }
    }

    return image;
}

} // namespace olylumoray
