#include "showphotoform.h"
#include <DycLibBase/TorqueChart/torquechart.h>
ShowPhotoForm::ShowPhotoForm(QVector<UIXML_STRU> vec,QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hBox = new QHBoxLayout(this);
    this->setLayout(hBox);
    hBox->addWidget(TorqueChart::GetInstance());
    this->setGeometry(5,5,parent->width()-5,parent->height()-5);
}
