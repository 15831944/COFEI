#include "Action.h"

CoAction::CoAction(const QString &text, QObject* parent):
    QAction(text, parent)
{
    m_Type = -1;
}
