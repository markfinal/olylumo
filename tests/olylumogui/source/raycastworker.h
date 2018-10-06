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
        const olylumoray::EMode inRenderMode
    );

    QImage *
    result() const;

protected:
    void
    run() override;

private:
    QSize             _size;
    int               _sample_count;
    olylumoray::EMode _render_mode;
    QImage           *_result = nullptr;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_RAYCASTWORKER_H
