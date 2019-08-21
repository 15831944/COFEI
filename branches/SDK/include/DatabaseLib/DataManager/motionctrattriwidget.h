#ifndef MOTIONCTRATTRIWIDGET_H
#define MOTIONCTRATTRIWIDGET_H

#include <QObject>
#include <QWidget>
#include <DatabaseLib/variable.h>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMutex>
#include <QTableWidget>
#include <QItemDelegate>
///
/// \brief The MotionCtrAttriWidget class
///运动控制类pp csp
class MotionCtrAttriWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MotionCtrAttriWidget(QWidget *parent = 0);
    ~MotionCtrAttriWidget();
    void SetData(VARDB_PPMOTCTR_STRU stru);
private:
    QLineEdit *m_delayline;
    QLineEdit *m_timoutline;
    QLineEdit *m_accline;
    QLineEdit *m_deccline;
    QCheckBox *m_savetoDb;
    QLabel *m_namelabel;
    QTableWidget *m_tableWg;
private:
    void createUI();
    QString GetAxisInfoString();
    void GetAxisandRegNumNames(QStringList &axisNames,QStringList &regnumNames);
    void SetAxisInfoTable(QString axisInfo);
signals:
    void signalAttriDataChanged(QString type, BASEDB_STRU *base);
private slots:
    void slotMotionCtrAttriChanged();
    void slotCheckAxisInfo(QTableWidgetItem *current);
private:
    QStringList m_baselist;

public slots:
};

class  SpeedRateDelegate :  public  QItemDelegate
{
    Q_OBJECT
public :
    SpeedRateDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent,  const QStyleOptionViewItem &option,
                          const  QModelIndex &index)  const
    {
        Q_UNUSED(option);
        Q_UNUSED(index);
        QLineEdit *editor =  new  QLineEdit(parent);
        // QRegExp regExp("100|([0-9]{0,2}[\.][0,9]{1,3})$");
        QDoubleValidator *pDoubleValidator = new QDoubleValidator();
        editor->setValidator(pDoubleValidator);
        return  editor;
    }
    void  setEditorData(QWidget *editor,  const  QModelIndex &index)  const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit =  static_cast <QLineEdit*>(editor);
        if((text.toDouble() > 0)&&(text.toDouble() <= 100))
        {
            lineEdit->setText(text);
        }
        else{
            lineEdit->setText("100");
        }
    }
    void  setModelData(QWidget *editor, QAbstractItemModel *model,
                       const  QModelIndex &index)  const
    {
        QLineEdit *lineEdit =  static_cast <QLineEdit*>(editor);
        QString text = lineEdit->text();
        if((text.toDouble() > 0)&&(text.toDouble() <= 100))
        {
              model->setData(index, text, Qt::EditRole);
        }
    }
    void  updateEditorGeometry(QWidget *editor,
                               const  QStyleOptionViewItem &option,  const  QModelIndex &index) const
    {
          Q_UNUSED(index);
        editor->setGeometry(option.rect);
    }
};

#endif // MOTIONCTRATTRIWIDGET_H
