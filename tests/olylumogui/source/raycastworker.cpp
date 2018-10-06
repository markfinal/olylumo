#include "raycastworker.h"

#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"

#include "QtGui/QImage"

namespace olylumogui
{

RayCastWorker::RayCastWorker(
    olylumoray::Hitable *inWorld,
    const QSize inSize,
    const uint32_t inSampleCount,
    const uint32_t inMaxRaysCast,
    const olylumoray::EMode inRenderMode
)
    :
    _world(inWorld),
    _size(inSize),
    _sample_count(inSampleCount),
    _max_rays_cast(inMaxRaysCast),
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
RayCastWorker::run()
{
    emit this->progress_changed(0);
    auto image = olylumoray::raycast(
        this->_world,
        this->_size.width(),
        this->_size.height(),
        this->_sample_count,
        this->_max_rays_cast,
        this->_render_mode,
        this->_progress_tick,
        [this](const int inProgress)
        {
            emit this->progress_changed(inProgress);
        },
        &this->_abort
    );

    if (this->_abort)
    {
        return;
    }

    // ignoring scanline convertion to bytes in the progress
    auto qimage = new QImage(image->width(), image->height(), QImage::Format_RGBA8888);
    auto src = image->pixels();
    for (auto row = 0u; row < image->height(); ++row)
    {
        if (this->_abort)
        {
            return;
        }
        auto dst = qimage->scanLine(row);
        for (auto col = 0u; col < image->width(); ++col)
        {
            src->convert_to_bytes(dst);
            ++src;
            dst += 4;
        }
    }

    if (this->_abort)
    {
        return;
    }
    emit this->image_available(qimage);
}

} // namespace olylumogui
