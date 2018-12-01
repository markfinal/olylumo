#ifndef OLYLUMOGUI_RAYCASTWORKER_H
#define OLYLUMOGUI_RAYCASTWORKER_H

#include "QtCore/QThread"
#include "QtCore/QSize"

#include <cstdint>
#include <memory>

namespace olylumoray
{
    enum class EMode;
    class Scene;
    class Image;
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

    void
    tile_complete(
        const uint32_t inX,
        const uint32_t inY,
        std::unique_ptr<olylumoray::Image> inTile);

signals:
    void progress_changed(int);
    void tile_available(int, int, QImage*);
    void complete();

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
