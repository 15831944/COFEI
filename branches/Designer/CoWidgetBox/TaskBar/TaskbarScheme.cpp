#include "TaskbarScheme.h"

CoTaskBarScheme* CoTaskBarScheme::myDefaultScheme = 0;

CoTaskBarScheme::CoTaskBarScheme(QObject *parent)
  : QObject(parent)
{
  QLinearGradient panelBackgroundGrd(0,0, 0,300);
  panelBackgroundGrd.setColorAt(0, 0x006181);
  panelBackgroundGrd.setColorAt(1, 0x00A99D);
  panelBackground = panelBackgroundGrd;

  headerSize = 30;
  headerAnimation = true;

  QLinearGradient headerBackgroundGrd(0,0, 0,30);
  headerBackgroundGrd.setColorAt(0, 0xFAFDFD);
  headerBackgroundGrd.setColorAt(0.3, 0xC8EAE9);
  headerBackgroundGrd.setColorAt(0.31, 0x79A8A6);
  headerBackgroundGrd.setColorAt(1, 0x85DEA9);
  headerBackground = headerBackgroundGrd;
  headerBackgroundOver = headerBackgroundGrd;

  headerBorder = QColor(0x79A8A6);
  headerBorderOver = QColor(0x79A8A6);

  headerLabelScheme.text = QColor(0xEEEEEE);
  headerLabelScheme.textOver = QColor(0xFFFFFF);
  headerLabelScheme.textOff = QColor(0x4F4F4F);
  headerLabelScheme.focusPen = QPen(QColor(0x006181), 1, Qt::DotLine);
  QFont f ;
  f.setWeight(QFont::Bold);
  f.setPointSize(12);
  headerLabelScheme.font = f;
  headerLabelScheme.iconSize = 24;
  headerLabelScheme.underlineOver = false;
  headerLabelScheme.cursorOver = true;

  headerButtonFold = QPixmap(":/images/headerButtonFold.png");
  headerButtonFoldOver = QPixmap(":/images/headerButtonFoldOver.png");
  headerButtonUnfold = QPixmap(":/images/headerButtonUnfold.png");
  headerButtonUnfoldOver = QPixmap(":/images/headerButtonUnfoldOver.png");
  headerButtonSize = QSize(18,18);


  QLinearGradient groupBackgroundGrd(0,0, 300,0);
  groupBackgroundGrd.setColorAt(1, 0xB8FFD9);
  groupBackgroundGrd.setColorAt(0, 0xFAFDFD);
  groupBackground = groupBackgroundGrd;
  groupBackgroundOver = groupBackground;

  groupBorder = QColor(0x79A8A6);
  groupBorderOver = groupBorder;

  groupFoldSteps = 20; groupFoldDelay = 15;
  groupFoldEffect = NoFolding;
  groupFoldThaw = true;

  taskLabelScheme.text = QColor(0x00736A);
  taskLabelScheme.textOver = QColor(0x044F49);
  taskLabelScheme.textOff = QColor(0xb0b0b0);
  taskLabelScheme.focusPen = QPen(QColor(0x006181), 1, Qt::DotLine);
  taskLabelScheme.iconSize = 24;
  taskLabelScheme.underlineOver = true;
  taskLabelScheme.cursorOver = true;
}

CoTaskBarScheme::~CoTaskBarScheme()
{

}

CoTaskBarScheme* CoTaskBarScheme::defaultScheme()
{
  if (!myDefaultScheme)
    myDefaultScheme = new CoTaskBarScheme();

  return myDefaultScheme;
}
