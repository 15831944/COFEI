#ifndef ATTRIBASIC_H
#define ATTRIBASIC_H

#include <QApplication>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFocusEvent>
#include "CoGraphics/Base/Base.h"
#include "AttriBase.h"

class TextAttri;
class FontAttri;
class BackgroundAttri;
class EnableAttri;
class VarSelectedAttri;
class ExpreSelectedAttri;
class JsTacticAttri;
class ColorSelect;
class TopBottom;
class AttriBase
{
private:
    AttriBase();
    ~AttriBase(){}

public:
    static TextAttri *textAttri(QWidget *parent = 0);
    static FontAttri *fontAttri(QWidget *parent = 0);
    static BackgroundAttri* backgroundAttri(QWidget *parent = 0);
    static EnableAttri *enableAttri(QWidget *parent = 0);
    static VarSelectedAttri *varSelectedAttri(QWidget *parent = 0);
    static ExpreSelectedAttri *expreSelectAttri(QWidget *parent = 0);
    static JsTacticAttri *jsTacticAttri(QWidget *parent = 0);
    static ColorSelect *colorSelect(QWidget *parent = 0);
    static TopBottom *topBottom(QWidget *parent = 0);

};


//---------------------文本属性-----------------------------------
class TextAttri : public QWidget
{
    Q_OBJECT
public:
    explicit TextAttri(QWidget *parent = 0);
    ~TextAttri() {}
signals:
    void SendTextFinfished(const QString&);

public:
    void SetText(const QString&);
    QString GetText() const;

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

public:
    QTextEdit   *m_pTextEditText;
private:
    bool        m_bFlag;
};
//----------------------end--------------------------------------


//---------------------字体属性-----------------------------------
class FontAttri : public QWidget
{
     Q_OBJECT
public:
    explicit FontAttri(QWidget *parent = 0);
    ~FontAttri() {}

signals:
    void FontColorChanged(QColor color);
    void FontSizeChanged(const QString&);
    void AlignVerChanged(int);
    void AlignHorChanged(int);

public:
    void SetFontSize(const QString&);
    QString GetFontSize() const;

    void SetFontColor(QColor color);
    QColor GetFontColor();

    void SetAlignVer(int);
    int GetAlignVer();

    void SetAligHor(int);
    int GetAlignHor();

private slots:
    void ColorChanged();

public:
    QColor           m_FontColor;

    QLabel          *m_plblFontColor;
    QComboBox       *m_pCmbFontSize;
    QButtonGroup    *m_pHorButtonGroup;
    QButtonGroup    *m_pVerButtonGroup;

private:
    //水平对齐
    QRadioButton    *m_pRdoHLeft;
    QRadioButton    *m_pRdoHCenter;
    QRadioButton    *m_pRdoHRight;

    //垂直对齐
    QRadioButton    *m_pRdoVTop;
    QRadioButton    *m_pRdoVCenter;
    QRadioButton    *m_pRdoVBottom;
};
//----------------------end--------------------------------------


//---------------------背景属性-----------------------------------
class BackgroundAttri : public QWidget
{
    Q_OBJECT
public:
    explicit BackgroundAttri(QWidget *parent = 0);
    ~BackgroundAttri(){}
signals:
    void ImageCheckStateChanged(bool);
    void ImageChanged(const QString &);
    void BackgroundColorChanged(QColor color);

public:
    void SetColor(QColor color);
    QColor GetColor();

    void SetImageCheckState(bool);
    bool GetImageCheckState();

    void SetImage(const QString &path);
    QString GetImage() const;

private slots:
    void IsUseImage(int state);
    void ColorChanged();
    void SelectImagePath();

private:
    QColor       m_BackgroundColor;

    QLabel      *m_pLblBackgroundColor;
    QCheckBox   *m_pChkIsImage;
    QLineEdit   *m_pLineEditImagePath;
    QPushButton *m_pBtnBackgroundColor;
    QPushButton *m_pBtnSelectImagePath;
};
//----------------------end--------------------------------------

//-----------------------可用属性---------------------------------
class EnableAttri : public QWidget
{
    Q_OBJECT
public:
    explicit EnableAttri(QWidget *parent = 0);
    ~EnableAttri() {}

signals:
    void SendTextFinfished(const QString&);
    void EnableStateChanged(int);

public:
    void SetButtonText(const QString& type);
    void SetSelectVarType(const QString& type);

    void SetEnableExpre(const QString &);
    QString GetEnableExpre() const;

    void SetEnableState(int);
    int GetEnableState();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
    void SelectVarExt();

private:
    QString         m_strSelectVarType;
    bool            m_bFlag;
    QTextEdit       *m_pTextEditExpre;
    QPushButton     *m_pBtnSelectVarExt;
    QButtonGroup    *m_pBtnGroup;
    QRadioButton    *m_pRdoEnable;
    QRadioButton    *m_pRdoNotEnable;

};
//----------------------end--------------------------------------


//----------------------变量选择----------------------------------
class VarSelectedAttri : public QWidget
{
    Q_OBJECT
public:
    explicit VarSelectedAttri(QWidget *parent = 0,bool isappend = false);
    ~VarSelectedAttri(){}

signals:
    //varInfo:"<varname>,<vartype>"
    void SendTextFinfished(const QString &varInfo);

public:
    void SetSelectVarType(const QString& type);
    void SetIsRealtimeVar(bool visible);
    void IsHWSwitch(bool b);

    void SetVarName(const QString &name);
    void SetVarType(char type);
    QString GetVarName() const;
    char GetVarType() const;

private slots:
    void SelectVar();
    void VarChangedExt();

private:
    QLineEdit *m_pLineEditVar;
    QPushButton *m_pBtnSelectVar;

    char m_VarType;
    QString m_strVarName;
    QString m_strSelectVarType;
    bool m_bIsRealtimeVar;
    bool m_bIsHWSwitch;
    bool m_bAppend;
};
//----------------------end--------------------------------------

//----------------------表达式选择---------------------------------
class ExpreSelectedAttri : public QWidget
{
    Q_OBJECT
public:
    explicit ExpreSelectedAttri(QWidget *parent = 0);
    ~ExpreSelectedAttri(){}

signals:
    void SendTextFinfished(const QString &);

public:
    void SetSelectVarType(const QString& type);
    void SetExpre(const QString &);
    QString GetExpre() const;

private slots:
    void SelectVarExt();

private:
    bool eventFilter(QObject *obj, QEvent *ev);
    QString         m_strSelectVarType;
    bool            m_bFlag;
    QTextEdit       *m_pTextEditExpre;
    QPushButton     *m_pBtnSelectVarExt;
};

//----------------------end--------------------------------------


//-------------------------JS--------------------------------
class JsTacticAttri : public QWidget
{
    Q_OBJECT
public:
    explicit JsTacticAttri(QWidget *parent = 0);
    ~JsTacticAttri(){}
signals:
    void JsTacticChanged(const QString& filename);

public:
    void SetJsTactic(const QString& filename);
    QString GetJsTactic() const;

private slots:
    void NewJsTactic();
    void CheckStateChanged(int state);
private:
    QStringList GetCurJSFile();
private:
    QPushButton     *m_pBtnNewJsTactic;
    QCheckBox       *m_pChkIsUseJsTactic;
    QComboBox       *m_pCmbJsTactic;
};
//----------------------------end------------------------------


//--------------------------颜色选择-----------------------------
class ColorSelect : public QWidget
{
    Q_OBJECT
public:
    explicit ColorSelect(QWidget *parent = 0);
    ~ColorSelect(){}
signals:
    void ColorChanged(QColor color);

public:
    void SetColor(QColor color);
    QColor GetColor();

    QLabel      *m_pLblText;

private:
    Q_SLOT void SelectColor();

    QColor       m_Color;
    QLabel      *m_pLblColor;
    QPushButton *m_pBtnSelectColor;

};
//----------------------------end------------------------------


//----------------------------置顶置底---------------------------
class TopBottom : public QWidget
{
    Q_OBJECT
public:
    explicit TopBottom(QWidget *parent = 0);
    ~TopBottom(){}
signals:
    void ZPlaceChanged(int value);

public:
    void SetZPlace(int value);
    int GetZPlace();

private:
    Q_SLOT void PlaceChange(int state);

    QCheckBox      *m_pChkTop;
    QCheckBox      *m_pChkBottom;
    int m_ZPlace;
};
//----------------------------end------------------------------

#endif // ATTRIBASIC_H
