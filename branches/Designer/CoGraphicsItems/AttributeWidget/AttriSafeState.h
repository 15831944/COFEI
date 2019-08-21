#ifndef ATTRISAFESTATE_H
#define ATTRISAFESTATE_H

#include "../AttributeWidget.h"
#include "CoGraphics/Base/Base.h"
#include "AttriBase.h"

class AttriSafeState : public GraphicAttrWidget
{
    Q_OBJECT
public:
    explicit AttriSafeState(QWidget *parent = 0);
    ~AttriSafeState(){}

private slots:
    void StateChanged(int);
    void StateChangedExt(int);
    void FlowNameChanged(const QString&);
    void ShowSafeDetail();

private:
    virtual void SetItem(CoItemEx*);
    QWidget* CreateAttri();

private:
    QComboBox *m_pStateCmb;
    QLabel *m_pSubFlowLbl;
    QComboBox *m_pSubFlowCmb;
    QPushButton *m_pSafeDetailBtn;
    QString m_strSafeString;

};

#endif // ATTRISAFESTATE_H
