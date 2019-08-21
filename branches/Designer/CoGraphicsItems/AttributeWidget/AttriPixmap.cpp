#include <QScrollArea>
#include <QDir>
#include <QFileDialog>
#include <QDoubleValidator>
#include <QtCore>
#include <QMessageBox>
#include "../TBasicControls/Pixmap.h"
#include "AttriPixmap.h"

AttriPixmap::AttriPixmap(QWidget *parent)
    : GraphicAttrWidget(parent)
{
    QTabWidget *pTabWidget = new QTabWidget();
    pTabWidget->setFixedWidth(260);
    QWidget *pAttriWidget = CreateAttri();
//    QWidget *pStateWidget = CreateStateAttri();
    pTabWidget->addTab(pAttriWidget,tr("属性"));
//    pTabWidget->addTab(pStateWidget,tr("状态"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(pTabWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);
}

QWidget *AttriPixmap::CreateAttri()
{
    QWidget *pAttriWidget = new QWidget();

    QGridLayout *pLayout = new QGridLayout();
    pAttriWidget->setLayout(pLayout);

    m_pLineEditImg = TBase::LineEdit(tr(""));

    m_pBtnSelectImg = TBase::Button(tr("浏览"),tr("BackGround"));
    connect(m_pBtnSelectImg,&QPushButton::clicked,this,&AttriPixmap::SelectImg);

    m_pLineEditFreq = TBase::LineEdit(tr(""));
    QDoubleValidator *pDV = new QDoubleValidator();
    pDV->setDecimals(0);
    m_pLineEditFreq->setValidator(pDV);

    m_pTBAttri = AttriBase::topBottom();

//    m_pCmbSelectPower->setEnabled(false);
//    pLayout->addLayout(m_pGridLayout,0,0,1,5);
    pLayout->addWidget(new QLabel(tr("图片")),1,0,Qt::AlignLeft);
    pLayout->addWidget(m_pLineEditImg,1,1,1,3);
    pLayout->addWidget(m_pBtnSelectImg,1,4);
    pLayout->addWidget(m_pTBAttri,2,1,1,3);
    pLayout->addWidget(new QLabel(tr("刷新频率")),3,0,Qt::AlignLeft);
    pLayout->addWidget(m_pLineEditFreq,3,1,1,3);
    pLayout->addWidget(new QLabel(tr("ms")),3,4);


    QGroupBox *pStateGrp = new QGroupBox(tr("状态"));
    QGridLayout *pVarlayout = new QGridLayout();
    m_pVarAttr = AttriBase::varSelectedAttri();
    m_pVarAttr->SetSelectVarType(tr("Switch"));
    connect(m_pVarAttr,&VarSelectedAttri::SendTextFinfished,this,&AttriPixmap::IsStateVar);
    m_pExpreAttri = AttriBase::expreSelectAttri();
    m_pExpreAttri->SetSelectVarType(tr("Num+Switch+String"));
    connect(m_pExpreAttri,&ExpreSelectedAttri::SendTextFinfished,this,&AttriPixmap::IsStateVar);

    pVarlayout->addWidget(new QLabel(tr("控制量")),0,0);
    pVarlayout->addWidget(m_pVarAttr,0,1);
    pVarlayout->addWidget(m_pExpreAttri,1,0,1,2);
    pVarlayout->setColumnStretch(0,1);
    pVarlayout->setColumnStretch(1,4);

    m_pPicGrp = new QGroupBox(tr("图片"));
    m_pPicGrp->setEnabled(false);
    QGridLayout *pPiclayout = new QGridLayout();
    m_pPicGrp->setLayout(pPiclayout);
    m_pZeroPicBtn = TBase::Button(tr("浏览"),tr("Zero"));
    connect(m_pZeroPicBtn,&QPushButton::clicked,this,&AttriPixmap::SelectImg);
    m_pOnePicBtn = TBase::Button(tr("浏览"),tr("One"));
    connect(m_pOnePicBtn,&QPushButton::clicked,this,&AttriPixmap::SelectImg);
    m_pZeroLineEdit = TBase::LineEdit("Zero");
    m_pOneLineEdit = TBase::LineEdit("One");
    pPiclayout->addWidget(new QLabel(tr("0")),0,0);
    pPiclayout->addWidget(m_pZeroLineEdit,0,1);
    pPiclayout->addWidget(m_pZeroPicBtn,0,2);
    pPiclayout->addWidget(new QLabel(tr("1")),1,0);
    pPiclayout->addWidget(m_pOneLineEdit,1,1);
    pPiclayout->addWidget(m_pOnePicBtn,1,2);
    pPiclayout->setColumnStretch(0,1);
    pPiclayout->setColumnStretch(1,6);
    pPiclayout->setColumnStretch(2,2);

    QVBoxLayout *pStateAtrriLayout = new QVBoxLayout();
    pStateAtrriLayout->addLayout(pVarlayout);
    pStateAtrriLayout->addWidget(m_pPicGrp);
    pStateGrp->setLayout(pStateAtrriLayout);
    pLayout->addWidget(pStateGrp,4,0,1,5,Qt::AlignTop);

    return pAttriWidget;
}

QWidget *AttriPixmap::CreateStateAttri()
{
    QWidget *pStateAttriWidget = new QWidget();

//    QGridLayout *pVarlayout = new QGridLayout();
//    m_pVarAttr = AttriBase::varSelectedAttri();
//    m_pVarAttr->SetSelectVarType(tr("Switch"));
//    connect(m_pVarAttr,&VarSelectedAttri::SendTextFinfished,this,&AttriPixmap::IsStateVar);
//    m_pExpreAttri = AttriBase::expreSelectAttri();
//    m_pExpreAttri->SetSelectVarType(tr("Num+Switch+String"));
//    connect(m_pExpreAttri,&ExpreSelectedAttri::SendTextFinfished,this,&AttriPixmap::IsStateVar);

//    pVarlayout->addWidget(new QLabel(tr("控制量")),0,0);
//    pVarlayout->addWidget(m_pVarAttr,0,1);
//    pVarlayout->addWidget(m_pExpreAttri,1,0,1,2);
//    pVarlayout->setColumnStretch(0,1);
//    pVarlayout->setColumnStretch(1,4);

//    m_pPicGrp = new QGroupBox(tr("图片"));
//    m_pPicGrp->setEnabled(false);
//    QGridLayout *pPiclayout = new QGridLayout();
//    m_pPicGrp->setLayout(pPiclayout);
//    m_pZeroPicBtn = TBase::Button(tr("浏览"),tr("Zero"));
//    connect(m_pZeroPicBtn,&QPushButton::clicked,this,&AttriPixmap::SelectImg);
//    m_pOnePicBtn = TBase::Button(tr("浏览"),tr("One"));
//    connect(m_pOnePicBtn,&QPushButton::clicked,this,&AttriPixmap::SelectImg);
//    m_pZeroLineEdit = TBase::LineEdit("Zero");
//    m_pOneLineEdit = TBase::LineEdit("One");
//    pPiclayout->addWidget(new QLabel(tr("0")),0,0);
//    pPiclayout->addWidget(m_pZeroLineEdit,0,1);
//    pPiclayout->addWidget(m_pZeroPicBtn,0,2);
//    pPiclayout->addWidget(new QLabel(tr("1")),1,0);
//    pPiclayout->addWidget(m_pOneLineEdit,1,1);
//    pPiclayout->addWidget(m_pOnePicBtn,1,2);
//    pPiclayout->setColumnStretch(0,1);
//    pPiclayout->setColumnStretch(1,6);
//    pPiclayout->setColumnStretch(2,2);

//    QVBoxLayout *pStateAtrriLayout = new QVBoxLayout();
//    pStateAtrriLayout->addLayout(pVarlayout);
//    pStateAtrriLayout->addWidget(m_pPicGrp);
//    pStateAttriWidget->setLayout(pStateAtrriLayout);
    return pStateAttriWidget;
}

void AttriPixmap::SetItem(CoItemEx *itm)
{
    GraphicAttrWidget::SetItem(itm);
    TPixmap *pix = qobject_cast<TPixmap*>(itm);

    m_pLineEditImg->setText(pix->GetFileDir());
    m_pLineEditFreq->setText(QString::number(pix->GetAutoTime()));
    m_pTBAttri->SetZPlace(pix->GetZValue());
    if((!pix->GetStateVarName().isEmpty()) || (!pix->GetStateExpre().isEmpty()))
    {
        m_pPicGrp->setEnabled(true);
        m_pVarAttr->SetVarName(pix->GetStateVarName());
        m_pVarAttr->SetVarType(pix->GetStateVarType());
        m_pExpreAttri->SetExpre(pix->GetStateExpre());
        m_pZeroLineEdit->setText(pix->GetStateBGPix().at(0));
        m_pOneLineEdit->setText(pix->GetStateBGPix().at(1));
    }

    connect(m_pLineEditImg,&QLineEdit::editingFinished,this,&AttriPixmap::ImgChanged);
    connect(m_pLineEditFreq,&QLineEdit::editingFinished,this,&AttriPixmap::FreqChanged);
    connect(m_pTBAttri,&TopBottom::ZPlaceChanged,this,&AttriPixmap::ZPlaceChange);
    connect(m_pVarAttr,&VarSelectedAttri::SendTextFinfished,this,&AttriPixmap::StateVarChanged);
    connect(m_pExpreAttri,&ExpreSelectedAttri::SendTextFinfished,this,&AttriPixmap::StateExpreChanged);
    connect(m_pZeroLineEdit,&QLineEdit::editingFinished,this,&AttriPixmap::StatePicChanged);
    connect(m_pOneLineEdit,&QLineEdit::editingFinished,this,&AttriPixmap::StatePicChanged);

    connect(this,&AttriPixmap::SendTAttribute,pix,&TPixmap::SetAttributeEvent);
}

void AttriPixmap::SelectImg()
{
    QString strUser = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
    QString strPath = QFileDialog::getOpenFileName(NULL, tr("打开"),strUser,tr("Images (*.jpg *.png)"));
    if(strPath.isEmpty())
        return;

    if(!strPath.contains(strUser))
    {
        QMessageBox msg(QMessageBox::Warning,tr("提示"),tr("请选择“用户文件夹”下的图片"),
                        QMessageBox::StandardButton::Yes,this);
        msg.exec();
        return;
    }

    QString str = tr("用户文件夹");
    int index = strPath.indexOf(str,0,Qt::CaseInsensitive);
    QString strRelativePath = ":" + strPath.right(strPath.length() - index - str.length());

    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(btn->objectName() == "BackGround")
    {
        m_pLineEditImg->setText(strRelativePath);
        m_pLineEditImg->editingFinished();
    }
    else if(btn->objectName() == "Zero")
    {
        m_pZeroLineEdit->setText(strRelativePath);
        m_pZeroLineEdit->editingFinished();
    }
    else if(btn->objectName() == "One")
    {
        m_pOneLineEdit->setText(strRelativePath);
        m_pOneLineEdit->editingFinished();
    }
}

void AttriPixmap::ImgChanged()
{
    emit SendTAttribute(CoAttributeWidget::PixMap,m_pLineEditImg->text(),0);
}

void AttriPixmap::FreqChanged()
{
    emit SendTAttribute(CoAttributeWidget::RefreshFreq,m_pLineEditFreq->text(),0);
}

void AttriPixmap::ZPlaceChange(int value)
{
    emit SendTAttribute(CoAttributeWidget::ZValue,QString::number(value),0);
}

void AttriPixmap::IsStateVar(const QString &var)
{
    if(var.isEmpty())
        m_pPicGrp->setEnabled(false);
    else
        m_pPicGrp->setEnabled(true);
}

void AttriPixmap::StateVarChanged(const QString &var)
{
    emit SendTAttribute(CoAttributeWidget::StateVarName,var,0);
}

void AttriPixmap::StateExpreChanged(const QString &var)
{
    emit SendTAttribute(CoAttributeWidget::EnableExpre,var,0);
}

void AttriPixmap::StatePicChanged()
{
    QLineEdit *pEdit = qobject_cast<QLineEdit*>(sender());
    uchar uc = 0;
    if(pEdit->objectName().compare(tr("One"),Qt::CaseInsensitive) == 0)
    {
        uc = 1;
    }
    emit SendTAttribute(CoAttributeWidget::StateBGPix,pEdit->text(),uc);

}

