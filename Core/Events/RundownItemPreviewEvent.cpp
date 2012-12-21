#include "RundownItemPreviewEvent.h"

#include "Global.h"

RundownItemPreviewEvent::RundownItemPreviewEvent()
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::RundownItemPreview))
{
}
