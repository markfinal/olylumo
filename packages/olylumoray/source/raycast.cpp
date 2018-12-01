#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"
#include "olylumoray/vec4.h"
#include "olylumoray/ray.h"
#include "olylumoray/sphere.h"
#include "olylumoray/hitrecord.h"
#include "olylumoray/material.h"
#include "olylumoray/utils.h"
#include "olylumoray/scene.h"

#include <limits>

namespace olylumoray
{

RGBA
calculate_colour(
    Scene *inScene,
    const Ray &inRay,
    const float inMinT, // near plane
    const uint32_t inMaxRaysCast,
    const EMode inMode)
{
    auto world = inScene->world();
    HitRecord record;
    if ((inMaxRaysCast > 0) && (nullptr != world) && world->hit(inRay, inMinT, std::numeric_limits<float>::max(), record))
    {
        switch (inMode)
        {
        case EMode::Colour:
        {
            Ray scattered;
            RGBA attenuation;
            if (record._material->scatter(inRay, record, attenuation, scattered))
            {
                return attenuation * calculate_colour(inScene, scattered, inMinT, inMaxRaysCast - 1, inMode);
            }
            else
            {
                return RGBA(0, 0, 0, 1);
            }
        }
        break;

        case EMode::WorldSpaceNormals:
            return RGBA(record._normal + 1) * 0.5f;
        }
    }
    const auto unit = inRay.direction().normalise();
    const auto background_t = 0.5f * (unit.y() + 1); // rescale [-1,1] to [0,1]
    const auto lerp = inScene->environment_gradient_bottom() * (1.0f - background_t) + inScene->environment_gradient_top() * background_t;
    return lerp;
}

void
raycast(
    Scene *inScene,
    const uint32_t inWidth,
    const uint32_t inHeight,
    const uint32_t inSampleCount,
    const uint32_t inMaxRaysCast,
    const uint32_t inTileCount,
    const EMode inMode,
    const uint32_t inProgressTick,
    std::function<void(int)> inProgressCallback,
    std::function<void(uint32_t, uint32_t, std::unique_ptr<Image>)> inTileCompleteCallback,
    std::function<void(void)> inCompleteCallback,
    bool *inAbortState)
{
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

    auto progress = 0u;
    auto last_progress_tick = 0u;
    inProgressCallback(progress);

    const auto tile_width = inWidth / inTileCount;
    assert(tile_width * inTileCount == inWidth);
    const auto tile_height = inHeight / inTileCount;
    assert(tile_height * inTileCount == inHeight);
    for (auto row = 0u; row < inTileCount; ++row)
    {
        for (auto col = 0u; col < inTileCount; ++col)
        {
            // new tile
            std::unique_ptr<Image> image(new Image(tile_width, tile_height));
            auto current_pixel = image->pixels();

            auto y = row * tile_height;
            for (auto tile_y = 0u; tile_y < tile_height; ++tile_y, ++y)
            {
                auto x = col * tile_width;
                for (auto tile_x = 0u; tile_x < tile_width; ++tile_x, ++x)
                {
                    if (nullptr != inAbortState && *inAbortState)
                    {
                        return;
                    }

                    RGBA colour;
                    for (auto sample = 0u; sample < inSampleCount; ++sample)
                    {
                        const auto u = static_cast<float>(x + random_between_zero_and_one()) / inWidth;
                        // NDC is (-1,-1) in bottom left, but pixels have (0,0) in top-left
                        const auto v = static_cast<float>(inHeight - y + random_between_zero_and_one()) / inHeight;

                        Ray ray(
                            camera_origin,
                            (camera_image_plane_bottom_left + camera_image_plane_horizonal * u + camera_image_plane_vertical * v).normalise()
                        );
                        const auto minT = 0.0001f;
                        //const auto minT = camera_image_plane_bottom_left.z(); // for camera near plane for clipping
                        colour += calculate_colour(inScene, ray, minT, inMaxRaysCast, inMode);
                        ++progress;
                    }

                    *current_pixel = colour / static_cast<float>(inSampleCount);
                    *current_pixel = current_pixel->gamma_correct();
                    current_pixel->make_opaque();
                    current_pixel++;
                }

                // calling progress updates is very expensive, so only do it
                // when we've exceeded one % of total
                // it's likely only to happen outside of the inner loops
                const auto tick = progress / inProgressTick;
                if (tick > last_progress_tick)
                {
                    inProgressCallback(tick);
                    last_progress_tick = tick;
                }
            }

            inTileCompleteCallback(
                col * tile_width,
                row * tile_height,
                std::move(image)
            );
        }
    }

    inCompleteCallback();
}

} // namespace olylumoray
