#ifndef COTASKPANELSCHEME_H
#define COTASKPANELSCHEME_H

#include <QtGui>

#include "Rect.h"

enum CoTaskBarFoldEffect
{
  NoFolding,
  ShrunkFolding,
  SlideFolding
};

struct  CoIconLabelScheme
{
  QColor text, textOver, textOff;
  QPen focusPen;
  QFont font;
  int iconSize;
  bool underlineOver, cursorOver;
};

class  CoTaskBarScheme : public QObject
{
public:
  CoTaskBarScheme(QObject *parent = 0);
  ~CoTaskBarScheme();

  static CoTaskBarScheme* defaultScheme();

  QBrush panelBackground;

  QBrush headerBackground, headerBackgroundOver;
  CoIconLabelScheme headerLabelScheme;
  QPen headerBorder, headerBorderOver;

  int headerSize;
  bool headerAnimation;
  CoRect headerCorners;

  QIcon headerButtonFold, headerButtonFoldOver, headerButtonUnfold, headerButtonUnfoldOver;
  QSize headerButtonSize;

  QBrush groupBackground, groupBackgroundOver;
  QPen groupBorder, groupBorderOver;
  int groupFoldSteps, groupFoldDelay;
  CoTaskBarFoldEffect groupFoldEffect;
  bool groupFoldThaw;
  CoIconLabelScheme taskLabelScheme;

protected:
  static CoTaskBarScheme *myDefaultScheme;
};

#endif // COTASKPANELSCHEME_H
