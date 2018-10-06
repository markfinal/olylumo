#include "raycastworker.h"

#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"

#include "QtGui/QImage"

namespace olylumogui
{

RayCastWorker::RayCastWorker(
    const QSize inSize,
    const int inSampleCount,
    const olylumoray::EMode inRenderMode
)
    :
    _size(inSize),
    _sample_count(inSampleCount),
    _render_mode(inRenderMode)
{}

QImage *
RayCastWorker::result() const
{
    return this->_result;
}

void
RayCastWorker::run()
{
    auto image = olylumoray::raycast(
        this->_size.width(),
        this->_size.height(),
        this->_sample_count,
        this->_render_mode
    );
    auto qimage = new QImage(image->width(), image->height(), QImage::Format_RGBA8888);
    auto src = image->pixels();
    for (auto row = 0u; row < image->height(); ++row)
    {
        auto dst = qimage->scanLine(row);
        for (auto col = 0u; col < image->width(); ++col)
        {
            src->convert_to_bytes(dst);
            ++src;
            dst += 4;
        }
    }

    this->_result = qimage;
}

} // namespace olylumogui
