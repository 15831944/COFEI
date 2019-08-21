#ifndef ADDCOORDINATEUI_H
#define ADDCOORDINATEUI_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QContextMenuEvent>

class AddCoordinateUI : public QWidget
{
    Q_OBJECT
public:
    explicit AddCoordinateUI(QString ,int ,int ,QString ,QWidget *parent = 0);
    void contextMenuEvent(QContextMenuEvent *event);
    QMenu* CreateMenu();
    void DeleteMenu();
private:
    QGroupBox *gbox;

signals:
     void SendCoorTile(QString tile);
public slots:

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // ADDCOORDINATEUI_H
