#ifndef TPIXMAP_H
#define TPIXMAP_H

#include <CoGraphics/Graphics.h>
/////////////////////////////////////////////////  图片  /////////////////////////////////////////////////////
class TPixmap : public CoGraphics
{
    Q_OBJECT
public:
    TPixmap(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white,
            const QString &FileDir = tr(":/images/draw/Pixmap.png"));

    virtual ~TPixmap();

public slots:
    CoItem *Copy();
    void Copy(TPixmap *PixmapFrom);
    void SetAttributeEvent(const int , const QString , uchar);

    void SetFileDir(const QString &FileDir);
    QString GetFileDir() const;

    void SetStateVarName(const QString);
    QString GetStateVarName() const;
    uchar GetStateVarType() const;

    void SetStateExpre(const QString);
    QString GetStateExpre() const;

    void SetStateBGPix(const QString,const uchar);
    QVector<QString> GetStateBGPix() const;

private:
    QString m_FileDir;
    QString m_CurDir;
    QVector<QString> m_StateBGPixVec;   //状态属性|0，1状态背景图
    QString m_stateVarName;  //状态属性|控制变量名
    char m_stateVarType;  //状态属性|控制变类型
    QString m_StateExpre;    //状态属性|表达式


    QImage m_Image;
    void Draw(QPainter *painter);
    void AutoRunRepeat();
    QString RelativePathToAbsolute(QString relative);
};
#endif // TPIXMAP_H
