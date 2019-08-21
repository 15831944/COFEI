#ifndef ATTRITABLEVIEW_H
#define ATTRITABLEVIEW_H

#include <QWidget>
#include "CoGraphics/Base/Base.h"
#include "../AttributeWidget.h"

class AttriTableView : public GraphicAttrWidget
{
    Q_OBJECT
public:
     explicit AttriTableView(QWidget *parent = 0);
    ~AttriTableView(){}

private slots:
    void RowSumChanged();

private:
    QWidget *CreateAttri();
    void SetItem(CoItemEx*);

private:
    QLineEdit *m_pRowSumEdit;
};

#endif // ATTRITABLEVIEW_H
