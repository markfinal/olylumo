#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"
#include "olylumoray/vec4.h"
#include "olylumoray/ray.h"
#include "olylumoray/sphere.h"
#include "olylumoray/hit_record.h"

#include <limits>

namespace olylumoray
{

RGBA
colour(
    const Ray &inRay)
{
    Sphere sphere({ 0,0,1,1 }, 0.5f);
    HitRecord record;
    if (sphere.hit(inRay, 0.0f, std::numeric_limits<float>::max(), record))
    {
        return RGBA(record._normal + 1) * 0.5f;
    }
    const auto unit = inRay.direction().normalise();
    const auto background_t = 0.5f * (unit.y() + 1); // rescale [-1,1] to [0,1]
    const auto lerp = RGBA(1, 1, 1, 1) * (1.0f - background_t) + RGBA(0.5f, 0.7f, 1.0f, 1.0f) * background_t;
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
