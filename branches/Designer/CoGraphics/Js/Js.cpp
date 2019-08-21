#include "Js.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QThread>
#include <QDebug>

TJs::TJs()
{
    m_Init = false;
    m_Parent = 0;
    m_JsExtend = new CoJS;
}

TJs::TJs(QObject *Parent, const QString &FileName)
{
    m_Init = false;
    m_JsExtend = new CoJS;
    m_Parent = Parent;
    m_FileName = FileName;
    Init();
}

TJs::~TJs()
{
    if(m_JsExtend != NULL)
        delete m_JsExtend;
    m_JsExtend = NULL;
}

void TJs::SetParent(QObject *Parent)
{
    m_Parent = Parent;
}

QObject* TJs::GetParent() const
{
    return m_Parent;
}

void TJs::SetFile(const QString &filename)
{
    m_FileName = filename;
}

QString TJs::GetFile() const
{
    return m_FileName;
}

void TJs::SetCoJs(CoJS *js)
{
    if(js != NULL)
        m_JsExtend = js;
}

CoJS *TJs::GetCoJs() const
{
    return m_JsExtend;
}

bool TJs::GetInit()
{
    return m_Init;
}

bool TJs::Init()
{
    if(m_FileName.length() <= 0)
    {
        return false;
    }

    QFile jsFile(m_FileName);

    if (!jsFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot Open js File";
        return false;
    }

    QTextStream jsStream(&jsFile);
    jsStream.setCodec("UTF-8");

    m_JsString = jsStream.readAll();

    jsFile.close();
    m_Init = true;

    return true;
}

bool TJs::Run(QObject *Object, const QString &FunName ,QString &errorMsg)
{
    if(!GetInit())
    {
        SetParent(Object);
        Init();
    }

    return Call(FunName,errorMsg);
}

bool TJs::Call(const QString &FunName,QString &errorMsg)
{
    if(!m_Init || NULL == m_Parent)
    {
        return false;
    }

    QScriptEngine JsIinterpreter;
    JsIinterpreter.evaluate(m_JsString); // 执行
    if(JsIinterpreter.hasUncaughtException())
    {
        errorMsg = tr("Cannot Run js File") + JsIinterpreter.uncaughtException().toString();
        return false;
    }

    QScriptValue coJs = JsIinterpreter.newQObject(m_JsExtend);
    JsIinterpreter.globalObject().setProperty("CoJs",coJs);

    QScriptValue ScriptValue = JsIinterpreter.newQObject(m_Parent);
    QScriptValueList Args;
    Args.append(ScriptValue);

    QScriptValue JsRunValue = JsIinterpreter.globalObject().property(FunName);
    JsRunValue.call(JsRunValue, Args);

    if(JsIinterpreter.hasUncaughtException())
    {
       errorMsg = tr("Cannot Exc ") + JsIinterpreter.uncaughtException().toString() ;
        return false;
    }

    return true;
}
bool TJs::CheckSyntax(const QString &program, QString &errorMessage)
{
    QScriptSyntaxCheckResult result = QScriptEngine::checkSyntax(program);
    if(result.state() == result.Valid)
    {
        errorMessage.clear();
        return true;
    }
    else
    {
        errorMessage = QString("Error Line:%1  Message:%2").arg(result.errorLineNumber()).arg(result.errorMessage());
        return false;
    }
}
