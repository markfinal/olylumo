#ifndef OLYLUMOGUI_RAYCASTWORKER_H
#define OLYLUMOGUI_RAYCASTWORKER_H

#include "QtCore/QThread"
#include "QtCore/QSize"

#include <cstdint>

namespace olylumoray
{
    enum class EMode;
    class Scene;
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
        olylumoray::Scene *inScene,
        const QSize inSize,
        const uint32_t inSampleCount,
        const uint32_t inMaxRaysCast,
        const uint32_t inTileCount,
        const olylumoray::EMode inRenderMode
    );

    void
    abort();

private:
    uint32_t
    progress_tick() const;

signals:
    void progress_changed(int);
    void image_available(QImage*);

protected:
    void
    run() override;

private:
    olylumoray::Scene *_scene;
    QSize              _size;
    uint32_t           _sample_count;
    uint32_t           _max_rays_cast;
    uint32_t           _tile_count;
    olylumoray::EMode  _render_mode;
    uint32_t           _progress_tick;
    bool               _abort = false;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_RAYCASTWORKER_H
