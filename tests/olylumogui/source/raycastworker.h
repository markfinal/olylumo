#ifndef OLYLUMOGUI_RAYCASTWORKER_H
#define OLYLUMOGUI_RAYCASTWORKER_H

#include "QtCore/QThread"
#include "QtCore/QSize"

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
        const int inSampleCount,
        const int inMaxRaysCast,
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
    int               _sample_count;
    int               _max_rays_cast;
    olylumoray::EMode _render_mode;
    QImage           *_result = nullptr;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_RAYCASTWORKER_H
