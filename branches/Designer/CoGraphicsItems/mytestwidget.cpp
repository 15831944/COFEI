#include "mytestwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QGraphicsGridLayout>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <CoGraphics/Scene/Scene.h>
#include <QDebug>
MyTestWidget::MyTestWidget(double StartX, double StartY, double StopX, double StopY,
                           const Qt::PenStyle &LineStyle, const int LineWidth,
                           const QColor &LineColor, const QColor &BackColor,CoScene* Scene) :
              CoProxyWidget(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::B_3);
    QLabel *pPixmapLabel = new QLabel();
    QLineEdit *pAccountLineEdit = new QLineEdit();
    QLineEdit *pPasswdLineEdit = new QLineEdit();
    QCheckBox *pRememberCheckBox = new QCheckBox();
    QObject::connect(pRememberCheckBox,&QCheckBox::clicked,[=]()
    {
        qDebug() << "QCheckBox clicked";
    });
    QCheckBox *pAutoLoginCheckBox = new QCheckBox();
    QPushButton *pLoginButton = new QPushButton();
    QObject::connect(pLoginButton,&QPushButton::clicked,[=]()
    {
        qDebug() << "QPushButton Login clicked";
    });
    QPushButton *pRegisterButton = new QPushButton();
    QObject::connect(pRegisterButton,&QPushButton::clicked,[=]()
    {
        qDebug() << "QPushButton Register clicked";
    });
    QPushButton *pForgotButton = new QPushButton();
    QObject::connect(pForgotButton,&QPushButton::clicked,[=]()
    {
        qDebug() << "QPushButton Forgot clicked";
    });

    pPixmapLabel->setStyleSheet("border-image: url(:/images/male.png); min-width:90px; min-height:90px; border-radius:45px; background:transparent;");
    pAccountLineEdit->setPlaceholderText(QStringLiteral("QQ号码/手机/邮箱"));
    pPasswdLineEdit->setPlaceholderText(QStringLiteral("密码"));
    pPasswdLineEdit->setEchoMode(QLineEdit::Password);
    pRememberCheckBox->setText(QStringLiteral("记住密码"));
    pAutoLoginCheckBox->setText(QStringLiteral("自动登录"));
    pLoginButton->setText(QStringLiteral("登录"));
    pRegisterButton->setText(QStringLiteral("注册账号"));
    pForgotButton->setText(QStringLiteral("找回密码"));

    pLoginButton->setFixedHeight(30);
    pAccountLineEdit->setFixedWidth(180);

    // 添加 widget
    QGraphicsProxyWidget *pPixmapWidget = Scene->addWidget(pPixmapLabel);
    QGraphicsProxyWidget *pAccountWidget = Scene->addWidget(pAccountLineEdit);
    QGraphicsProxyWidget *pPasswdWidget = Scene->addWidget(pPasswdLineEdit);
    QGraphicsProxyWidget *pRememberWidget = Scene->addWidget(pRememberCheckBox);
    QGraphicsProxyWidget *pAutoLoginWidget = Scene->addWidget(pAutoLoginCheckBox);
    QGraphicsProxyWidget *pLoginWidget = Scene->addWidget(pLoginButton);
    QGraphicsProxyWidget *pRegisterWidget = Scene->addWidget(pRegisterButton);
    QGraphicsProxyWidget *pForgotWidget = Scene->addWidget(pForgotButton);

    // 添加至网格布局中
    QGraphicsGridLayout *pLayout = new QGraphicsGridLayout();
    pLayout->addItem(pPixmapWidget, 0, 0, 3, 1);
    pLayout->addItem(pAccountWidget, 0, 1, 1, 2);
    pLayout->addItem(pRegisterWidget, 0, 4);
    pLayout->addItem(pPasswdWidget, 1, 1, 1, 2);
    pLayout->addItem(pForgotWidget, 1, 4);
    pLayout->addItem(pRememberWidget, 2, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->addItem(pAutoLoginWidget, 2, 2, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
    pLayout->addItem(pLoginWidget, 3, 1, 1, 2);
    pLayout->setHorizontalSpacing(10);
    pLayout->setVerticalSpacing(10);
    pLayout->setContentsMargins(10, 10, 10, 10);

    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->setLayout(pLayout);
}

CoProxyWidget *MyTestWidget::Copy()
{
    MyTestWidget *Button = new MyTestWidget();

    Button->Copy(this);

    return Button;
}

void MyTestWidget::Copy(MyTestWidget *ButtonFrom)
{
    CoProxyWidget::Copy(ButtonFrom);
}
