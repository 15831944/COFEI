#include "Base.h"

#include <QPicture>
#include <QFile>
#include <QDebug>
#include <QColorDialog>

TBase::TBase()
{
}

TBase::~TBase()
{
}

QLabel* TBase::Label(const QString &Text, const QString &ObjectName)
{
    QLabel *mLabel = new QLabel(Text);
    mLabel->setFont(QFont("黑体",10));
    mLabel->setObjectName(ObjectName);

    return mLabel;
}

QLabel* TBase::Label(const QString &Text, const QString &ObjectName, const int Weigh, const int Higth)
{
    QLabel *mLabel = new QLabel(Text);
    mLabel->setFont(QFont("黑体",10));
    mLabel->setObjectName(ObjectName);

    mLabel->setFixedSize(Weigh, Higth);

    return mLabel;
}

QPushButton* TBase::Button(const QString &Text, const QString &ObjecName)
{
    QPushButton * mButton = new QPushButton(Text);
    mButton->setFont(QFont("黑体",10));
    mButton->setObjectName(ObjecName);
    //mButton->setFixedSize(25 * Text.length(), 36);

    return mButton;
}

QPushButton* TBase::Button(const QString &Text, const QString &ObjecName, const int Width, const int Height)
{
    QPushButton * mButton = new QPushButton(Text);
    mButton->setFont(QFont("黑体",10));
    mButton->setObjectName(ObjecName);
    mButton->setFixedSize(Width, Height);

    return mButton;
}

void TBase::ColorLabel(QLabel *Label, const QColor &Color)
{
    if(Label)
    {
        QPixmap mPixmap(Label->width(), Label->height());
        mPixmap.fill(Color);
        Label->setPixmap(mPixmap);
    }
}

void TBase::ColorLabel(QLabel *Label, const QColor &Color, int Width, int Height)
{
    int w = Width;
    int h = Height;

    if(w > Label->width() || w < 0)
    {
        w = Label->width();
    }

    if(h > Label->height() || h < 0)
    {
        h = Label->height();
    }

    QPixmap mPixmap(w, h);
    mPixmap.fill(Color);
    Label->setPixmap(mPixmap);
}

void TBase::PictureLabel(QLabel *Label, const QString &Dir)
{
    QPicture *mPicture = new QPicture();
    mPicture->load(Dir);
    Label->setPicture(*mPicture);
}

QSpinBox* TBase::SpinBox(const QString &ObjectName, int Min, int Max)
{
    QSpinBox* mSpinBox = new QSpinBox();
    mSpinBox->setObjectName(ObjectName);
    mSpinBox->setRange(Min, Max);

    return mSpinBox;
}

QSpinBox* TBase::SpinBox(const QString &ObjectName, int Min, int Max, int Width, int Height)
{
    QSpinBox* mSpinBox = SpinBox(ObjectName, Min, Max);
    mSpinBox->setFixedSize(Width, Height);

    return mSpinBox;
}

QDoubleSpinBox* TBase::DoubleSpinBox(const QString &ObjectName)
{
    QDoubleSpinBox* mSpinBox = new QDoubleSpinBox();
    mSpinBox->setObjectName(ObjectName);

    return mSpinBox;
}

QDoubleSpinBox* TBase::DoubleSpinBox(const QString &ObjectName, const double Min, const double Max)
{
    //qDebug() << ObjectName << "const double Min, const double Max";
    QDoubleSpinBox* mSpinBox = DoubleSpinBox(ObjectName);
    mSpinBox->setRange(Min, Max);

    return mSpinBox;
}

QDoubleSpinBox* TBase::DoubleSpinBox(const QString &ObjectName, const double Min, const double Max,
                                        const int Width, const int Height)
{
    //qDebug() << ObjectName << "const double Min, const double Max, const int Width, const int Height";
    QDoubleSpinBox* mSpinBox = DoubleSpinBox(ObjectName, Min, Max);

    //qDebug() << "fDoubleSpinBox:" << Width << Height;
    mSpinBox->setFixedSize(Width, Height);

    return mSpinBox;
}

QComboBox* TBase::ComboBox(const QString &ObjectName)
{
    QComboBox *mBox = new QComboBox();
    mBox->setObjectName(ObjectName);

    return mBox;
}

QComboBox* TBase::ComboBox(const QString &ObjectName, const int Width, const int Height)
{
    QComboBox *mBox = new QComboBox();
    mBox->setObjectName(ObjectName);
    mBox->setFixedSize(Width, Height);

    return mBox;
}

QComboBox* TBase::LineStyleComboBox()
{
    QComboBox* mComboBox = TBase::ComboBox(tr("mComboBoxLineStyle"));
    mComboBox->setIconSize(QSize(96, 22));
    Q_INIT_RESOURCE ( CoGraphics ) ;
    mComboBox->addItem(QIcon(tr(":/images/line/StyleNoPen.png")), tr("0"));
    mComboBox->addItem(QIcon(tr(":/images/line/StyleSolidLine.png")), tr("1"));
    mComboBox->addItem(QIcon(tr(":/images/line/StyleDashLine.png")), tr("2"));
    mComboBox->addItem(QIcon(tr(":/images/line/StyleDotLine.png")), tr("3"));
    mComboBox->addItem(QIcon(tr(":/images/line/StyleDashDotLine.png")), tr("4"));
    mComboBox->addItem(QIcon(tr(":/images/line/StyleDashDotDotLine.png")), tr("5"));
    mComboBox->addItem(QIcon(tr(":/images/line/StyleCustomDashLine.png")), tr("6"));

    mComboBox->setCurrentIndex(1);

    return mComboBox;
}

QTextEdit* TBase::TextEdit(const QString &ObjectName)
{
    QTextEdit *mTextEdit = new QTextEdit();
    mTextEdit->setFont(QFont("黑体",10));
    mTextEdit->setObjectName(ObjectName);

    return mTextEdit;
}

QTextEdit* TBase::TextEdit(const QString &ObjectName, const int Width, const int Height)
{
    QTextEdit *mTextEdit = new QTextEdit();
    mTextEdit->setFont(QFont("黑体",10));
    mTextEdit->setObjectName(ObjectName);
    mTextEdit->setFixedSize(Width, Height);

    return mTextEdit;
}

QLineEdit* TBase::LineEdit(const QString &ObjectName)
{
    QLineEdit *mLineEdit = new QLineEdit();
    mLineEdit->setFont(QFont("黑体",10));
    mLineEdit->setObjectName(ObjectName);

    return mLineEdit;
}

QLineEdit* TBase::LineEdit(const QString &ObjectName, const int Width, const int Height)
{
    QLineEdit *mLineEdit = new QLineEdit();
    mLineEdit->setFont(QFont("黑体",10));
    mLineEdit->setObjectName(ObjectName);
    mLineEdit->setFixedSize(Width, Height);

    return mLineEdit;
}

QCheckBox* TBase::CheckBox(const QString &Text, const QString &ObjectName)
{
    QCheckBox *mCheckBox = new QCheckBox();

    mCheckBox->setObjectName(ObjectName);
    mCheckBox->setText(Text);

    return mCheckBox;
}

bool TBase::ChooseColor(QColor &Color, QWidget *parent)
{
    QColor mNewColor = QColorDialog::getColor(Color, parent,QString(),QColorDialog::ShowAlphaChannel);

    if(mNewColor.isValid())
    {
        Color = mNewColor;
        return true;
    }

    return false;
}

int TBase::GetComboBoxCurrentData(QComboBox *ComboBox)
{
    if(ComboBox)
    {
        return ComboBox->itemData(ComboBox->currentIndex()).toInt();
    }

    return 0;
}

QDateTimeEdit* TBase::DateTimeEdit(const QString &ObjectName, int Month)
{
    QDateTimeEdit *DateTime = new QDateTimeEdit();
    DateTime->setFont(QFont("黑体",10));
    DateTime->setObjectName(ObjectName);
    DateTime->setDateTime(QDateTime::currentDateTime().addMonths(Month));
    DateTime->setDisplayFormat(tr("yyyy-MM-dd HH:mm:ss"));

    return DateTime;
}

QProgressBar* TBase::ProgressBar(const QString &ObjectName, int Min, int Max, QWidget *parent)
{
    QProgressBar *mBar = new QProgressBar(parent);

    mBar->setObjectName(ObjectName);
    mBar->setRange(Min, Max);

    return mBar;
}

QColor TBase::GetNewColor(const QColor &Source, int Value)
{
    int r = Source.red() + Value;
    int g = Source.green() + Value;
    int b = Source.blue() + Value;
    int a = Source.alpha();

    if(r > 255)
    {
        r = 255;
    }
    else if(r < 0)
    {
        r = 0;
    }

    if(g > 255)
    {
        g = 255;
    }
    else if(g < 0)
    {
        g = 0;
    }

    if(b > 255)
    {
        b = 255;
    }
    else if(b < 0)
    {
        b = 0;
    }


    return QColor(r, g, b, a);
}
