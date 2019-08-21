#ifndef BASE_H
#define BASE_H

#include <QDialog>
#include <QApplication>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QCheckBox>


#include <QProgressBar>

#include <QLayout>
#include <QLineEdit>
#include <QDateTimeEdit>

class TBase
{
    Q_DECLARE_TR_FUNCTIONS(TBase)

private:
    TBase();
    ~TBase();

public:
    static QLabel* Label(const QString &Text, const QString &ObjectName);
    static QLabel* Label(const QString &Text, const QString &ObjectName, const int Width, const int Height);
    static QPushButton* Button(const QString &Text, const QString &ObjecName);
    static QPushButton* Button(const QString &Text, const QString &ObjecName, const int Width, const int Height);

    static void ColorLabel(QLabel *Label, const QColor &Color);
    static void ColorLabel(QLabel *Label, const QColor &Color, int Width, int Height);
    static void PictureLabel(QLabel *Label, const QString &Dir);
    static QSpinBox* SpinBox(const QString &ObjectName, int Min, int Max);
    static QSpinBox* SpinBox(const QString &ObjectName, int Min, int Max, int Width, int Height);

    static QDoubleSpinBox* DoubleSpinBox(const QString &ObjectName);
    // 调用该接口，必须传入浮点数，假如为常数，也要包含小数点，如5.0
    static QDoubleSpinBox* DoubleSpinBox(const QString &ObjectName, const double Min, const double Max);
    static QDoubleSpinBox* DoubleSpinBox(const QString &ObjectName, const double Min, const double Max,
                                          const int Width, const int Height);
    static QComboBox* ComboBox(const QString &ObjectName);
    static QComboBox* ComboBox(const QString &ObjectName, const int Width, const int Height);
    static QComboBox* LineStyleComboBox();
    static QTextEdit* TextEdit(const QString &ObjectName);
    static QTextEdit* TextEdit(const QString &ObjectName, const int Width, const int Height);
    static QLineEdit* LineEdit(const QString &ObjectName);
    static QLineEdit* LineEdit(const QString &ObjectName, const int Width, const int Height);
    static QCheckBox* CheckBox(const QString &Text, const QString &ObjectName);

    static bool ChooseColor(QColor &Color, QWidget *parent = 0);
    static int GetComboBoxCurrentData(QComboBox *ComboBox);

    // month 距离当前时间的月
    static QDateTimeEdit* DateTimeEdit(const QString &ObjectName, int Month = 0);

    static QProgressBar* ProgressBar(const QString &ObjectName, int Min, int Max, QWidget *parent = 0);

    static QColor GetNewColor(const QColor &Source, int Value);
};

#endif // BASE_H
