#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"
#include "olylumoray/vec4.h"
#include "olylumoray/ray.h"
#include "olylumoray/sphere.h"
#include "olylumoray/hitrecord.h"
#include "olylumoray/hitablelist.h"

#include <limits>
#include <random>

#define DIFFUSE

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> unit_dist(0, 1);
std::uniform_real_distribution<float> cube_dist(-1, 1);

namespace olylumoray
{

Vec4
random_in_unit_sphere()
{
    Vec4 p;
    do
    {
        p = Vec4(cube_dist(gen), cube_dist(gen), cube_dist(gen), 0);
    }
    while (p.squared_length() >= 1.0f);
    return p;
}

RGBA
calculate_colour(
    const Ray &inRay,
    const float inMinT) // near plane
{
    HitableList hit_list;
    hit_list.append(new Sphere({ 0,0,-1,1 }, 0.5f));
    hit_list.append(new Sphere({ 0,-100.5f,-1,1 }, 100));
    HitRecord record;
    if (hit_list.hit(inRay, inMinT, std::numeric_limits<float>::max(), record))
    {
#ifdef DIFFUSE
        const auto target = record._pos + record._normal + random_in_unit_sphere();
        const auto material_absorption = 0.5f;
        return calculate_colour(Ray(record._pos, target - record._pos), inMinT) * (1 - material_absorption);
#else
        return RGBA(record._normal + 1) * 0.5f;
#endif
    }
    const auto unit = inRay.direction().normalise();
    const auto background_t = 0.5f * (unit.y() + 1); // rescale [-1,1] to [0,1]
    const auto lerp = RGBA(1, 1, 1, 1) * (1.0f - background_t) + RGBA(0.5f, 0.7f, 1.0f, 1.0f) * background_t;
    return lerp;
}

std::unique_ptr<Image>
raycast(
    const uint32_t inWidth,
    const uint32_t inHeight)
{
    const auto num_samples = 1u;
    std::unique_ptr<Image> image(new Image(inWidth, inHeight));

    // use right-handed coordinate system:
    // right   -> +x
    // up      -> +y
    // forward -> -z
    // this way, normals facing the camera plane will have a positive Z

    const auto aspect_ratio = static_cast<float>(inWidth) / inHeight;

    // define a camera image plane for the pin-hole camera
    Vec4 camera_image_plane_bottom_left(-1 * aspect_ratio, -1.0f, -1.0f, 0.0f); // used as direction
    Vec4 camera_image_plane_horizonal(2 * aspect_ratio, 0.0f, 0.0f, 0.0f); // direction
    Vec4 camera_image_plane_vertical(0, 2.0f, 0, 0.0f); // direction
    Vec4 camera_origin(0, 0, 0, 1); // position

    auto current_pixel = image->pixels();
    for (auto row = 0u; row < inHeight; ++row)
    {
        for (auto col = 0u; col < inWidth; ++col)
        {
            RGBA colour;
            for (auto sample = 0u; sample < num_samples; ++sample)
            {
                const auto u = static_cast<float>(col + unit_dist(gen)) / inWidth;
                // NDC is (-1,-1) in bottom left, but pixels have (0,0) in top-left
                const auto v = static_cast<float>(inHeight - row + unit_dist(gen)) / inHeight;

                Ray ray(
                    camera_origin,
                    (camera_image_plane_bottom_left + camera_image_plane_horizonal * u + camera_image_plane_vertical * v).normalise()
                );
                const auto minT = 0.0001f;
                //const auto minT = camera_image_plane_bottom_left.z(); // for camera near plane for clipping
                colour += calculate_colour(ray, minT);
            }

            *current_pixel = colour / num_samples;
            *current_pixel = current_pixel->gamma_correct();
            current_pixel->make_opaque();
            current_pixel++;
        }
    }

    return image;
}

} // namespace olylumoray
