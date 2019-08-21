#include "smtcommlib.h"
#include <QByteArray>
#include <QTextCodec>
SMTCommLib::SMTCommLib()
{
    m_readData = "";
    m_CommInfoMap.clear();
}

SMTCommLib *SMTCommLib::GetInstance()
{
    static SMTCommLib smt;
    return &smt;
}

bool SMTCommLib::OpenPort(QString strId, QString name, int BaudRate)
{
    bool commisok = true;
    if(!m_CommInfoMap.contains(strId))
    {
        COMMINFO cominfo;
        commisok  = cominfo.com->OpenPort(name,BaudRate,8,0,1);

        if(commisok)
        {
            m_CommInfoMap.insert(strId,cominfo);
            connect(cominfo.com,&Communication::reseive,this,&SMTCommLib::PortRead);
        }
    }
    return commisok;
}

void SMTCommLib::WriteData(QString strId,QByteArray buf)
{
    if(m_CommInfoMap.contains(strId))
    {
        m_CommInfoMap[strId].com->WriteDate(buf);
    }
}

ushort SMTCommLib::CalculateIOVaule(QString strId,uchar ipos, ushort uc)
{
    ushort buf = 0;
    if(m_CommInfoMap.contains(strId))
    {
        if(uc == 1)
        {
            uc = uc << ipos;
            m_CommInfoMap[strId].oValue = m_CommInfoMap[strId].oValue | uc;
        }
        else
        {
            uc = ~(1 << ipos);
            m_CommInfoMap[strId].oValue = m_CommInfoMap[strId].oValue & uc;
        }
        buf = m_CommInfoMap[strId].oValue;
    }
    return buf;
}

QString SMTCommLib::ReadData()
{
    return m_readData;
}

void SMTCommLib::PortRead(QByteArray value)
{
    QTextCodec *ptc = QTextCodec::codecForName("GBK");
    m_readData = ptc->toUnicode(value);
}
