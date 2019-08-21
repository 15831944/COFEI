#ifndef ITEMEX_H
#define ITEMEX_H
#include "Item.h"
class CoItemEx : public CoItem
{
    Q_OBJECT

public:
    CoItemEx(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~CoItemEx();

public slots:
    virtual void Copy(CoItemEx *ItemExFrom);

    virtual void SetNameId(const QString &NameId);
    virtual QString GetNameId() const;

    virtual void SetServiceId(int Id);
    virtual int GetServiceId() const;

    // 备注信息
    virtual void SetHint(const QString &Hint);
    virtual QString GetHint() const;

    virtual void SetHostScene(QObject *HostScene);
    virtual QObject *GetHostScene() const;

    virtual QGraphicsScene *GetCurScene() const;

    virtual void SetUserDataName1(const QString &UserDataName);
    virtual QString GetUserDataName1() const;
    virtual void SetUserDataName2(const QString &UserDataName);
    virtual QString GetUserDataName2() const;
    virtual void SetUserData1(const double UserData);
    virtual double GetUserData1() const;
    virtual void SetUserData2(const double UserData);
    virtual double GetUserData2()const;
    virtual void SetFont(QFont);
    virtual QFont GetFont() const;
    virtual void SetAutoTime(int Time);
    virtual int GetAutoTime();

    virtual void StartAuto(int itype = 0);

protected:
    QString m_NameId;
    int m_ServiceId;
    QString m_Hint;
    QTimer *m_Timer;
    int m_AutoTime;
    QString m_UserDataName1;
    QString m_UserDataName2;
    double m_UserData1;
    double m_UserData2;

    QObject *m_HostScene;
    QFont m_font;
public:
//    bool    m_isfirstSelected;
};

#endif // ITEMEX_H
