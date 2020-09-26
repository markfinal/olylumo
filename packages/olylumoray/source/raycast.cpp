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

// determine this from the build system
#ifdef USE_TBB
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#endif

#include <limits>
#include <cassert>

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

#ifdef USE_TBB
class RaycastBody
{
public:
    RaycastBody(
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
        bool *inAbortState,
        const uint32_t inTileWidth,
        const uint32_t inTileHeight,
        const Vec4 &camera_image_plane_bottom_left,
        const Vec4 &camera_image_plane_horizonal,
        const Vec4 &camera_image_plane_vertical,
        const Vec4 &camera_origin)
        :
        _scene(inScene),
        _width(inWidth),
        _height(inHeight),
        _sampleCount(inSampleCount),
        _maxRaysCast(inMaxRaysCast),
        _tileCount(inTileCount),
        _mode(inMode),
        _progressTick(inProgressTick),
        _progressCallback(inProgressCallback),
        _tileCompleteCallback(inTileCompleteCallback),
        _completeCallback(inCompleteCallback),
        _abortState(inAbortState),
        _tileWidth(inTileWidth),
        _tileHeight(inTileHeight),
        _camera_image_plane_bottom_left(camera_image_plane_bottom_left),
        _camera_image_plane_horizonal(camera_image_plane_horizonal),
        _camera_image_plane_vertical(camera_image_plane_vertical),
        _camera_origin(camera_origin)
    {}

    void
    operator()(
        const tbb::blocked_range2d<unsigned int> &inR) const
    {
        for (auto row = inR.rows().begin(); row < inR.rows().end(); ++row)
        {
            for (auto col = inR.cols().begin(); col < inR.cols().end(); ++col)
            {
                // new tile
                std::unique_ptr<Image> image(new Image(this->_tileWidth, this->_tileHeight));
                auto current_pixel = image->pixels();

                auto y = row * this->_tileHeight;
                for (auto tile_y = 0u; tile_y < this->_tileHeight; ++tile_y, ++y)
                {
                    auto x = col * this->_tileWidth;
                    for (auto tile_x = 0u; tile_x < this->_tileWidth; ++tile_x, ++x)
                    {
                        if (nullptr != this->_abortState && *this->_abortState)
                        {
                            return;
                        }

                        RGBA colour;
                        for (auto sample = 0u; sample < this->_sampleCount; ++sample)
                        {
                            const auto u = static_cast<float>(x + random_between_zero_and_one()) / this->_width;
                            // NDC is (-1,-1) in bottom left, but pixels have (0,0) in top-left
                            const auto v = static_cast<float>(this->_height - y + random_between_zero_and_one()) / this->_height;

                            Ray ray(
                                    this->_camera_origin,
                                    (this->_camera_image_plane_bottom_left + this->_camera_image_plane_horizonal * u + this->_camera_image_plane_vertical * v).normalise()
                                    );
                            const auto minT = 0.0001f;
                            //const auto minT = camera_image_plane_bottom_left.z(); // for camera near plane for clipping
                            colour += calculate_colour(this->_scene, ray, minT, this->_maxRaysCast, this->_mode);
                            //++progress;
                        }

                        *current_pixel = colour / static_cast<float>(this->_sampleCount);
                        *current_pixel = current_pixel->gamma_correct();
                        current_pixel->make_opaque();
                        current_pixel++;
                    }

                    /*
                    // calling progress updates is very expensive, so only do it
                    // when we've exceeded one % of total
                    // it's likely only to happen outside of the inner loops
                    const auto tick = progress / inProgressTick;
                    if (tick > last_progress_tick)
                    {
                        inProgressCallback(tick);
                        last_progress_tick = tick;
                    }
                     */
                }

                this->_tileCompleteCallback(
                    col * this->_tileWidth,
                    row * this->_tileHeight,
                    std::move(image)
                );
            }
        }

        //inCompleteCallback();
    }

private:
    Scene *_scene;
    uint32_t _width;
    uint32_t _height;
    uint32_t _sampleCount;
    uint32_t _maxRaysCast;
    uint32_t _tileCount;
    EMode _mode;
    uint32_t _progressTick;
    std::function<void(int)> _progressCallback;
    std::function<void(uint32_t, uint32_t, std::unique_ptr<Image>)> _tileCompleteCallback;
    std::function<void(void)> _completeCallback;
    bool *_abortState;
    uint32_t _tileWidth;
    uint32_t _tileHeight;
    Vec4 _camera_image_plane_bottom_left;
    Vec4 _camera_image_plane_horizonal;
    Vec4 _camera_image_plane_vertical;
    Vec4 _camera_origin;
};
#endif

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

    const auto tile_width = inWidth / inTileCount;
    assert(tile_width * inTileCount == inWidth);
    const auto tile_height = inHeight / inTileCount;
    assert(tile_height * inTileCount == inHeight);
#ifdef USE_TBB
    tbb::parallel_for(
        tbb::blocked_range2d<unsigned int>(0u, inTileCount, 4u, 0u, inTileCount, 4u),
        RaycastBody(
            inScene,
            inWidth,
            inHeight,
            inSampleCount,
            inMaxRaysCast,
            inTileCount,
            inMode,
            inProgressTick,
            inProgressCallback,
            inTileCompleteCallback,
            inCompleteCallback,
            inAbortState,
            tile_width,
            tile_height,
            camera_image_plane_bottom_left,
            camera_image_plane_horizonal,
            camera_image_plane_vertical,
            camera_origin
        )
    );
#else
    auto progress = 0u;
    auto last_progress_tick = 0u;
    inProgressCallback(progress);

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
#endif
}

} // namespace olylumoray
