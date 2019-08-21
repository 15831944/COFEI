#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QScrollArea>
#include <QDebug>
#include "DialogNewExcuteJs.h"
#include <DycLibBase/ScriptManager/scriptmanager.h>

DialogNewExcuteJs::DialogNewExcuteJs(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    this->setWindowTitle(tr("脚本程序"));
    this->resize(800,600);
    this->setStyleSheet(tr("QLabel{color:red;}"));
    this->setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    m_pTextEditJsString = new QTextEdit();
    m_pTextEditJsString->setAcceptRichText(false);

    m_pFuncTree = new QTreeWidget();
    connect(m_pFuncTree,&QTreeWidget::itemDoubleClicked,this,&DialogNewExcuteJs::AddFunc);
    InitFuncTree();
    QHBoxLayout *pTopLayout = new QHBoxLayout();
    pTopLayout->addWidget(m_pTextEditJsString,3);
    pTopLayout->addWidget(m_pFuncTree,1);
    pMainLayout->addLayout(pTopLayout);

    QHBoxLayout *pBottomLayout = new QHBoxLayout();
    pMainLayout->addLayout(pBottomLayout);

    m_pLblError = TBase::Label(tr(""),tr("LblError"));
    m_pLblError->setFixedWidth(500);
    m_pLblError->setWordWrap(true);
    pBottomLayout->addWidget(m_pLblError,6,Qt::AlignLeft);

    m_pBtnImport = TBase::Button(tr("导入本地JS"),tr("BtnImport"),90,30);
    pBottomLayout->addWidget(m_pBtnImport,1,Qt::AlignTop);
    connect(m_pBtnImport,&QPushButton::clicked,this,&DialogNewExcuteJs::ImpotJs);

    m_pBtnOK = TBase::Button(tr("确定"),tr("BtnOK"),65,30);
    pBottomLayout->addWidget(m_pBtnOK,1,Qt::AlignTop);
    connect(m_pBtnOK,&QPushButton::clicked,this,&DialogNewExcuteJs::SendJsString);

    QPushButton *pBtnQuit = TBase::Button(tr("取消"),tr("BtnQuit"),60,30);
    pBottomLayout->addWidget(pBtnQuit,1,Qt::AlignTop);
    connect(pBtnQuit,&QPushButton::clicked,this,&DialogNewExcuteJs::close);
}

void DialogNewExcuteJs::InitFuncTree()
{
    m_pFuncTree->setColumnCount(1);
    m_pFuncTree->setHeaderLabel(tr("函数选择"));
    QTreeWidgetItem *pSysFuncItem = new QTreeWidgetItem(m_pFuncTree,QStringList(QString(tr("系统函数"))));
    QTreeWidgetItem *pRunFuncItem = new QTreeWidgetItem(m_pFuncTree,QStringList(QString(tr("运行函数"))));
    QTreeWidgetItem *pGraphicFuncItem = new QTreeWidgetItem(m_pFuncTree,QStringList(QString(tr("图元函数"))));
    QTreeWidgetItem *pGraphicVarItem = new QTreeWidgetItem(m_pFuncTree,QStringList(QString(tr("变量函数"))));

    // add system function items
    QList<QString> funcTexts;
    funcTexts << tr("获取文件文本") << tr("创建空文件") << tr("IF~ELSE");

    QList<QTreeWidgetItem*> runFuncItems;
    for(int i = 0; i < funcTexts.size(); i++)
    {
        runFuncItems.append(new QTreeWidgetItem(pSysFuncItem,QStringList(QString(funcTexts.at(i)))));
    }

    pSysFuncItem->addChildren(runFuncItems);

    // add run function items
    funcTexts.clear();
    funcTexts << tr("设置总速度") << tr("设置总加速度") << tr("设置总减速度")
            << tr("设置总速率") << tr("设置轴励磁") << tr("轴复位")
            << tr("轴回原点") << tr("轴运行") << tr("轴停止") << tr("pp写入XML")
            << tr("获取轴位置") << tr("获取扭力") << tr("获取轴ErrorCode")
            << tr("获取轴故障Bit") << tr("获取轴Ready");

    runFuncItems.clear();
    for(int i = 0; i < funcTexts.size(); i++)
    {
        runFuncItems.append(new QTreeWidgetItem(pRunFuncItem,QStringList(QString(funcTexts.at(i)))));
    }

    pRunFuncItem->addChildren(runFuncItems);

    // add graphics function items
    funcTexts.clear();
    funcTexts << tr("刷新") << tr("获取文本") << tr("设置文本") << tr("设置背景色");
    runFuncItems.clear();
    for(int i = 0; i < funcTexts.size(); i++)
    {
        runFuncItems.append(new QTreeWidgetItem(pGraphicFuncItem,QStringList(QString(funcTexts.at(i)))));
    }

    pGraphicFuncItem->addChildren(runFuncItems);

    // add var function items
    funcTexts.clear();
    funcTexts << tr("获取变量的值");
    runFuncItems.clear();
    for(int i = 0; i < funcTexts.size(); i++)
    {
        runFuncItems.append(new QTreeWidgetItem(pGraphicVarItem,QStringList(QString(funcTexts.at(i)))));
    }

    pGraphicVarItem->addChildren(runFuncItems);
    m_pFuncTree->expandAll();
}

QString DialogNewExcuteJs::GetTextByItem(QTreeWidgetItem *item)
{
    QString strText = "";
    if(item->text(0) == tr("获取文件文本"))
    {
        strText = tr("    /* 参数:请输入绝对路径 */\n"
                     "    var text = CoJs.GetFileContent(\"path\");");
    }
    else if(item->text(0) == tr("创建空文件"))
    {
        strText = tr("    /* 参数1:请输入绝对路径; 参数2:请输入文件名称 */\n"
                     "    CoJs.CreateFile(\"path\", \"\");");
    }
    else if(item->text(0) == tr("IF~ELSE"))
    {
        strText = tr("if()\n{\n    \n}\nelse\n{\n    \n}");
    }
    else if(item->text(0) == tr("设置总速度"))
    {
        strText = tr("    /* 参数:请输入总速度值 */\n"
                     "    CoJs.SetSpeed(500);");
    }
    else if(item->text(0) == tr("设置总加速度"))
    {
        strText = tr("    /* 参数:请输入总加速度值 */\n"
                     "    CoJs.SetAcc(400);");
    }
    else if(item->text(0) == tr("设置总减速度"))
    {
        strText = tr("    /* 参数:请输入总减速度值 */\n"
                     "    CoJs.SetDcc(400);");
    }
    else if(item->text(0) == tr("设置总速率"))
    {
        strText = tr("    /* 参数:请输入总速率值 */\n"
                     "    CoJs.SetRatio(20);");
    }
    else if(item->text(0) == tr("设置轴励磁"))
    {
        strText = tr("    /* 参数1:请输入轴名称; 参数2:请输入是否励磁(0:不励磁;1:励磁) */\n"
                     "    CoJs.SetAxisSon(\"X\", 1);");
    }
    else if(item->text(0) == tr("轴复位"))
    {
        strText = tr("    /* 参数:请输入轴名称 */\n"
                     "    CoJs.SetAxisReset(\"X\");");
    }
    else if(item->text(0) == tr("轴回原点"))
    {
        strText = tr("    /* 参数:请输入轴名称 */\n"
                     "    CoJs.SetAxisORG(\"X\");");
    }
    else if(item->text(0) == tr("轴运行"))
    {
        strText = tr("    /* 参数1:请输入轴名称; 参数2:请输入轴运行的目标位置 */\n"
                     "    CoJs.SetAxisRun(\"X\",500);");
    }
    else if(item->text(0) == tr("轴停止"))
    {
        strText = tr("    /* 参数:请输入轴名称 */\n"
                     "    CoJs.SetAxisStop(\"X\");");
    }
    else if(item->text(0) == tr("pp写入XML"))
    {
        strText = tr("    CoJs.WriteSWXml();");
    }
    else if(item->text(0) == tr("获取轴位置"))
    {
        strText = tr("    /* 参数:请输入轴名称 */\n"
                     "    var pos = CoJs.GetAxisCurPos(\"X\");");
    }
    else if(item->text(0) == tr("获取扭力"))
    {
        strText = tr("    /* 参数:请输入轴名称 */\n"
                     "    var tq = CoJs.GetAxisCurtq(\"X\");");
    }
    else if(item->text(0) == tr("获取轴ErrorCode"))
    {
        strText = tr("    /* 参数:请输入轴名称 */\n"
                     "    var code = CoJs.GetAxisErrorCode(\"X\");");
    }
    else if(item->text(0) == tr("获取轴故障Bit"))
    {
        strText = tr("    /* 参数:请输入轴名称 */\n"
                     "    var bit = CoJs.GetAxisFault(\"X\");");
    }
    else if(item->text(0) == tr("获取轴Ready"))
    {
        strText = tr("    /* 参数:请输入轴名称 */\n"
                     "    var ready = CoJs.GetAxisReadySw(\"X\");");
    }
    else if(item->text(0) == tr("刷新"))
    {
        strText = tr("    item.Update();");
    }
    else if(item->text(0) == tr("获取文本"))
    {
        strText = tr("    var text = item.GetText();");
    }
    else if(item->text(0) == tr("设置文本"))
    {
        strText = tr("    /* 参数:请输入图元文本 */\n"
                     "    item.SetText(\"Text\");");
    }
    else if(item->text(0) == tr("设置背景色"))
    {
        strText = tr("    /* 参数:请输入颜色的R,G,B分量,例如白色,参数如下 */\n"
                     "    item.SetBackGroundColor(255,255,255);");
    }
    else if(item->text(0) == tr("获取变量的值"))
    {
        strText = tr("    /* 参数1:请输入变量名; 参数2:请输入变量类型,\n"
                     "                    例如:字符串型:RegText; 数值型:RegNum; 计数:Cnt; 计时:Time; 开关:Switch */\n"
                     "    var value = CoJs.GetVarValueText(\"\", \"Time\");");
    }

    return strText;
}

void DialogNewExcuteJs::ImpotJs()
{
    //load file
    QString strFileName = QFileDialog::getOpenFileName(NULL,tr("Open"),QCoreApplication::applicationDirPath(),tr("*.js"));
    if(strFileName.isEmpty())
        return;

    QFile file(strFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return;

    QTextStream in(&file);

    QString str = in.readAll();
    QString head = tr("function ExcFunc(item)\n{\n");
    QString tail = tr("\n}");
    str = str.right(str.length() - head.length());
    str = str.left(str.length() - tail.length());

    m_pTextEditJsString->clear();
    m_pTextEditJsString->setPlainText(str);
    file.close();
}

void DialogNewExcuteJs::SendJsString()
{
    QString str = m_pTextEditJsString->toPlainText().insert(0,tr("function ExcFunc(item)\n{\n")).append(tr("\n}"));

    //检查js语法错误
    QString strError = "";
    QScriptSyntaxCheckResult result = QScriptEngine::checkSyntax(str);
    if(result.state() == result.Valid)
    {
        m_pLblError->clear();
    }
    else
    {
        strError = QString("Error Line:%1  Message:%2").arg(result.errorLineNumber()).arg(result.errorMessage());
        m_pLblError->setText(strError);
        return ;
    }

    //保存到本地
    //TODO:路径后期需要修改
    QString strFileName = QFileDialog::getSaveFileName(NULL,
                                                       tr("Save File"),QCoreApplication::applicationDirPath().append("//脚本//untitled.js"),
                                                       tr("*.js"));
    if(strFileName.isEmpty())
        return;

    QFile file(strFileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(NULL,tr("Warning"),tr("保存文件失败"));
        qDebug() << file.errorString();
        return;
    }
    QTextStream out(&file);
    out << str;
    file.close();

    QFileInfo info(strFileName);
    QString strBase = info.fileName();
    emit signalSendNewJs(strBase);
    this->close();
}

void DialogNewExcuteJs::AddFunc(QTreeWidgetItem *item, int)
{
    if(!(item->text(0) == tr("运行函数")
         || item->text(0) == tr("图元函数")
         || item->text(0) == tr("系统函数")))
        m_pTextEditJsString->textCursor().insertText(GetTextByItem(item));
    m_pTextEditJsString->setFocus();
}
