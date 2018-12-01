#include "raycastworker.h"

#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"

#include "QtGui/QImage"

namespace olylumogui
{

RayCastWorker::RayCastWorker(
    olylumoray::Scene *inScene,
    const QSize inSize,
    const uint32_t inSampleCount,
    const uint32_t inMaxRaysCast,
    const uint32_t inTileCount,
    const olylumoray::EMode inRenderMode
)
    :
    _scene(inScene),
    _size(inSize),
    _sample_count(inSampleCount),
    _max_rays_cast(inMaxRaysCast),
    _tile_count(inTileCount),
    _render_mode(inRenderMode)
{
    this->_progress_tick = this->progress_tick();
}

void
RayCastWorker::abort()
{
    this->_abort = true;
}

uint32_t
RayCastWorker::progress_tick() const
{
    const auto sample_count = (this->_sample_count > 0) ? this->_sample_count : 1;
    const auto ray_cost = this->_size.height() * this->_size.width() * sample_count;
    return ray_cost / 100;
}

void
RayCastWorker::tile_complete(
    const uint32_t inX,
    const uint32_t inY,
    std::unique_ptr<olylumoray::Image> inTile)
{
    // ignoring scanline convertion to bytes in the progress
    auto qimage = new QImage(inTile->width(), inTile->height(), QImage::Format_RGBA8888);
    auto src = inTile->pixels();
    for (auto row = 0u; row < inTile->height(); ++row)
    {
        if (this->_abort)
        {
            return;
        }
        auto dst = qimage->scanLine(row);
        for (auto col = 0u; col < inTile->width(); ++col)
        {
            src->convert_to_bytes(dst);
            ++src;
            dst += 4;
        }
    }

    emit this->tile_available(inX, inY, qimage);
}

void
RayCastWorker::run()
{
    emit this->progress_changed(0);
    olylumoray::raycast(
        this->_scene,
        this->_size.width(),
        this->_size.height(),
        this->_sample_count,
        this->_max_rays_cast,
        this->_tile_count,
        this->_render_mode,
        this->_progress_tick,
        [this](const int inProgress)
        {
            emit this->progress_changed(inProgress);
        },
        std::bind(&RayCastWorker::tile_complete, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        &this->_abort
    );
}

} // namespace olylumogui
