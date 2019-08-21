#ifndef COACTION_H
#define COACTION_H

#include "coaction_global.h"
#include <QAction>

class COACTIONSHARED_EXPORT CoAction: public QAction
{
    Q_OBJECT

public:
    CoAction(const QString &text, QObject* parent = 0);

    int m_Type;
};

#endif // COACTION_H
