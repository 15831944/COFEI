#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(int timeInterval = 0, QWidget *parent = 0);

protected:
    virtual void initializeGL() = 0;
    virtual void resizeGL(int width, int height) = 0;
    virtual void paintGL() = 0;
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void timeOut();
protected slots:
    virtual void timeOutSlot();

private:
    QTimer *m_timer;
};
class MyGLWidget : public GLWidget
{
public:
    MyGLWidget(int timeInterval = 0, QWidget *parent = 0);
    ~MyGLWidget() ;
public:
    void SetWidgetSize(int ,int);
protected:
    void loadGLTextures();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void glPrint(GLint x, GLint y, const char *string, int set);
    void buildFont();
    void killFont();
private:
    float points[45][45][3]; // the array for the points on the grid of our "wave"
    int wiggleCount; // counte used to control how fast flag waves
    GLfloat xrot, yrot, zrot;
     GLfloat cnt1, cnt2; // 字体移动计数器
    GLuint texture[2];
    QPoint lastPos;
    GLfloat scaling;
    GLuint base; // 绘制字体的显示列表的开始位置
};

#endif // MYGLWIDGET_H
