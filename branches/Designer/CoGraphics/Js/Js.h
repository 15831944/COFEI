#ifndef JS_H
#define JS_H

#include <QApplication>
#include "CoJs.h"
#include <QString>
#include <QtScript/QScriptEngine>

class TJs
{
    Q_DECLARE_TR_FUNCTIONS(TJs)

public:
    TJs();
    TJs(QObject *Parent, const QString &filename);
    ~TJs();

    void SetParent(QObject *Parent);
    QObject *GetParent() const;
    void SetFile(const QString &filename);
    QString GetFile() const;
    void SetCoJs(CoJS *js);
    CoJS *GetCoJs() const;

    bool Init();
    bool GetInit();
    bool Call(const QString &FunName,QString &errorMsg);
    bool Run(QObject *Object, const QString &FunName, QString &errorMsg);
    bool CheckSyntax(const QString &program, QString &errorMessage);

protected:
    bool m_Init;
    QObject *m_Parent;

    QString m_FileName;

    QString m_JsString;
    CoJS *m_JsExtend;
};

#endif // JS_H
