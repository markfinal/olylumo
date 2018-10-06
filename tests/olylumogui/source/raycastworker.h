#ifndef OLYLUMOGUI_RAYCASTWORKER_H
#define OLYLUMOGUI_RAYCASTWORKER_H

#include "QtCore/QThread"
#include "QtCore/QSize"

#include <cstdint>

namespace olylumoray
{
    enum class EMode;
} // namespace olylumoray

class QImage;

namespace olylumogui
{

class RayCastWorker final :
    public QThread
{
    Q_OBJECT

public:
    RayCastWorker(
        const QSize inSize,
        const uint32_t inSampleCount,
        const uint32_t inMaxRaysCast,
        const olylumoray::EMode inRenderMode
    );

    QImage *
    result() const;

    int
    progress_max() const;

signals:
    void progress_changed(int);

protected:
    void
    run() override;

private:
    QSize             _size;
    uint32_t          _sample_count;
    uint32_t          _max_rays_cast;
    olylumoray::EMode _render_mode;
    QImage           *_result = nullptr;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_RAYCASTWORKER_H
