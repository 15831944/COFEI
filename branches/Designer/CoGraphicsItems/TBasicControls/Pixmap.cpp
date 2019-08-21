#include <QDebug>
#include <QtCore>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <DycLibBase/StackManager/stackmanager.h>
#include "AttributeWidget.h"
#include "Pixmap.h"

TPixmap::TPixmap(double StartX, double StartY, double StopX, double StopY,
                    const Qt::PenStyle &LineStyle, const int LineWidth,
                    const QColor &LineColor, const QColor &BackColor,
                    const QString &FileDir) :
        CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Pixmap);

    m_FileDir = FileDir;
    m_CurDir = m_FileDir;
    m_stateVarType = -1;
    m_stateVarName = tr("");
    m_StateExpre = tr("");
    m_StateBGPixVec.reserve(2);
    m_StateBGPixVec << ""<< "";
    m_Image.load("");
}

TPixmap::~TPixmap()
{}

CoItem *TPixmap::Copy()
{
    TPixmap *Pixmap = new TPixmap();

    Pixmap->Copy(this);

    return Pixmap;
}

void TPixmap::Copy(TPixmap *PixmapFrom)
{
    if(NULL == PixmapFrom)
    {
        return;
    }

    CoGraphics::Copy(PixmapFrom);

    this->SetFileDir(PixmapFrom->GetFileDir());
    this->SetAutoTime(PixmapFrom->GetAutoTime());
    this->SetStateVarName(QString("%1,%2").arg(PixmapFrom->GetStateVarName())
                          .arg(QString::number(GetStateVarType())));
    this->SetStateExpre(PixmapFrom->GetStateExpre());
    this->SetStateBGPix(PixmapFrom->GetStateBGPix()[0],0);
    this->SetStateBGPix(PixmapFrom->GetStateBGPix()[1],1);
}

void TPixmap::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    Q_UNUSED(uc)
    switch (type)
    {
    case CoAttributeWidget::PixMap:
        SetFileDir(value);
        break;
    case CoAttributeWidget::RefreshFreq:
        SetAutoTime(value.toInt());
        break;
    case CoAttributeWidget::ZValue:
        SetZValue(value.toInt());
        break;
    case CoAttributeWidget::EnableExpre:
        SetStateExpre(value);
        break;
    case CoAttributeWidget::StateVarName:
        SetStateVarName(value);
        break;
    case CoAttributeWidget::StateBGPix:
        SetStateBGPix(value,uc);
        break;
    default:
        break;
    }
}

void TPixmap::SetFileDir(const QString &FileDir)
{
    m_FileDir = FileDir;
    m_CurDir = m_FileDir;
    update();
}

QString TPixmap::GetFileDir() const
{
    return m_FileDir;
}

void TPixmap::SetStateVarName(const QString var)
{
    QStringList list = var.split(',');
    if(list.size() > 1)
    {
        m_stateVarName = list[0];
        m_stateVarType = list[1].toShort();
    }
}

QString TPixmap::GetStateVarName() const
{
    return m_stateVarName;
}

uchar TPixmap::GetStateVarType() const
{
    return m_stateVarType;
}

void TPixmap::SetStateExpre(const QString expre)
{
    m_StateExpre = expre;
}

QString TPixmap::GetStateExpre() const
{
    return m_StateExpre;
}

void TPixmap::SetStateBGPix(const QString str, const uchar uc)
{
    if(m_StateBGPixVec.size() > uc)
    {
        m_StateBGPixVec[uc] = str;
    }
}

QVector<QString> TPixmap::GetStateBGPix() const
{
    return m_StateBGPixVec;
}

void TPixmap::Draw(QPainter *painter)
{
    m_Image.load(RelativePathToAbsolute(m_CurDir));
    painter->drawPixmap(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY, QPixmap(RelativePathToAbsolute(m_CurDir)));
}

void TPixmap::AutoRunRepeat()
{
    bool expreResult = true;
    bool varResult = true;
    if(!m_StateExpre.isEmpty())
    {
        QString strExpre = m_StateExpre;
        strExpre.replace("&","$");
        StackManager stack;
        expreResult = stack.AnalysisEvent(strExpre);
    }
    if(!m_stateVarName.isEmpty())
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[m_stateVarName].ishwIO)
        {
            uchar uc = 0;
            DBInstanceManager::getDBInstance()->m_rmutex.lock();
            QString ioaddr = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[m_stateVarName].IOaddr;
            DBInstanceManager::getDBInstance()->m_rmutex.unlock();
            if(ioaddr.contains("E"))
            {
                int iresult = BaseCalcFun::AnalysisIOString(ioaddr,uc);
                if(iresult == 1)
                    varResult = IntToBool(uc);
            }
            if(ioaddr.contains("Q"))
            {
                int iresult = BaseCalcFun::GetOutIOQBitValue(ioaddr,uc);
                if(iresult == 1)
                    varResult = IntToBool(uc);
            }
        }
        else
        {
            QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_rmutex);
            varResult = IntToBool(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[m_stateVarName].initvalue);
        }
    }

    if(expreResult & varResult)
    {
        m_CurDir = m_StateBGPixVec.at(1);
    }
    else
    {
        m_CurDir = m_StateBGPixVec.at(0);
    }

    if(!m_Image.isNull())
    {
        QImage image(RelativePathToAbsolute(m_CurDir));
        if(image != m_Image)
            update();
    }
    else
        update();
}

QString TPixmap::RelativePathToAbsolute(QString relative)
{
    QString path = "";
    if(relative == tr(":/images/draw/Pixmap.png"))
    {
        path = relative;
    }
    else
    {
        QString strUser = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
        path = strUser + relative.right(relative.length() - 1);
    }
    return path;
}

