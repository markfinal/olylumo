#include "raycastworker.h"

#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"

#include "QtGui/QImage"

namespace olylumogui
{

RayCastWorker::RayCastWorker(
    const QSize inSize,
    const uint32_t inSampleCount,
    const uint32_t inMaxRaysCast,
    const olylumoray::EMode inRenderMode
)
    :
    _size(inSize),
    _sample_count(inSampleCount),
    _max_rays_cast(inMaxRaysCast),
    _render_mode(inRenderMode)
{}

int
RayCastWorker::progress_max() const
{
    const auto sample_count = (this->_sample_count > 0) ? this->_sample_count : 1;
    const auto ray_cost = this->_size.height() * this->_size.width() * sample_count;
    const auto scanline_convert_cost = this->_size.height();
    return ray_cost + scanline_convert_cost;
}

void
RayCastWorker::run()
{
    emit this->progress_changed(0);
    auto image = olylumoray::raycast(
        this->_size.width(),
        this->_size.height(),
        this->_sample_count,
        this->_max_rays_cast,
        this->_render_mode,
        [this](const int inProgress)
        {
            emit this->progress_changed(inProgress);
        }
    );
    auto qimage = new QImage(image->width(), image->height(), QImage::Format_RGBA8888);
    auto src = image->pixels();
    auto progress = this->progress_max() - this->_size.height();
    for (auto row = 0u; row < image->height(); ++row)
    {
        auto dst = qimage->scanLine(row);
        for (auto col = 0u; col < image->width(); ++col)
        {
            src->convert_to_bytes(dst);
            ++src;
            dst += 4;
        }
        emit this->progress_changed(progress++);
    }

    emit this->image_available(qimage);
}

} // namespace olylumogui
