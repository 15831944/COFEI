#include "myglwidget.h"
#include <QTimer>
#include <QMouseEvent>
#include <GL/glu.h>
const float piover180 = 0.0174532925f;
//const char rollFont[24] = {'h','t','t','p',':','/','/','w','w','w','.','c','o','w','a','i','n','.','c','o','m','.','c','n'};
GLWidget::GLWidget(int timerInterval, QWidget *parent)
    : QGLWidget(parent)
{
    if (timerInterval == 0) {
        m_timer = 0;
    } else {
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &GLWidget::timeOutSlot);
        m_timer->start(timerInterval);
    }
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        close();
    }
}

void GLWidget::timeOut()
{
}

void GLWidget::timeOutSlot()
{
    timeOut();
}

MyGLWidget::MyGLWidget(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    wiggleCount = 0;
    xrot = yrot = zrot = 0;
    scaling = 1.0;
    cnt1 = 0;
    cnt2 = 20;
    setWindowTitle(tr("版本信息"));

    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    //setFixedSize(pe->width()/2, pe->height()/2);
    //this->move(pe->width()/3,pe->height()/3);
    QIcon icon(":/images/skin/images/QFramer.png");
    setWindowIcon(icon);
}
void MyGLWidget::SetWidgetSize(int w, int h)
{
    setFixedSize(w/2,h/2);
    this->move(w/3,h/3);
}
void MyGLWidget::loadGLTextures()
{
    //QImage tex, buf(":/images/tim.bmp");
    QImage tex[2], buf(":/images/skin/images/QFramer.png"),buf1(":/images/skin/images/font.bmp");
    tex[0] = QGLWidget::convertToGLFormat(buf);
    tex[1] = QGLWidget::convertToGLFormat(buf1);
    glGenTextures(2, &texture[0]);

    for (int i = 0; i < 2; i++) {
        // 生成所有纹理
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, tex[i].width(), tex[i].height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, tex[i].bits());
    }
}

void MyGLWidget::initializeGL()
{
    loadGLTextures();
    buildFont();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glPolygonMode(GL_BACK, GL_FILL); // back face is filled in
    //glPolygonMode(GL_FRONT, GL_LINE); // front face is drawn with lines
    glPolygonMode(GL_FRONT, GL_FILL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (int x = 0; x < 45; x++) {
        for (int y = 0; y < 45; y++) {
            points[x][y][0] = float((x / 5.0f) - 4.5f);
            points[x][y][1] = float((y / 5.0f) - 4.5f);
            points[x][y][2] = float(sin((x/ 5.0f) * 40.0f * piover180));
        }
    }

    GLint lightAmbient[4] = {rand() % 256, rand() % 256, rand() % 256, 255};
    GLint lightDiffuse[4] = {rand() % 256, rand() % 256, rand() % 256, 255};
    GLint lightPosition[4] = {-1, -1, 0, 1};

    glLightiv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightiv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightiv(GL_LIGHT0,GL_POSITION, lightPosition);

    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHTING);
}
/* 创建字符显示列表 */
void MyGLWidget::buildFont()
{
    float cx, cy; // 字符坐标
    base = glGenLists(256); // 创建256个显示列表
    glBindTexture(GL_TEXTURE_2D, texture[1]); // 选择字符图像

    for (int i = 0; i < 256; i++) {
        cx = (i % 16) / 16.0;
        cy = (i / 16) / 16.0;

        glNewList(base + i, GL_COMPILE); {
            glBegin(GL_QUADS); {
                glTexCoord2f(cx, 1 - cy - 0.0625); glVertex2i(0, 0);
                glTexCoord2f(cx + 0.0625, 1 - cy - 0.0625); glVertex2i(16, 0);
                glTexCoord2f(cx + 0.0625, 1 - cy); glVertex2i(16, 16);
                glTexCoord2f(cx, 1 - cy); glVertex2i(0, 16);
            } glEnd();
            glTranslated(10, 0, 0);
        } glEndList();
    }
}
MyGLWidget::~MyGLWidget()
{
    killFont();                                         //删除显示列表
}

void MyGLWidget::killFont()
{
    glDeleteLists(base, 256);
}
void MyGLWidget::resizeGL(int width, int height)
{
    height = height ? height : 1;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, GLdouble(width) / GLdouble(height), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MyGLWidget::paintGL()
{
    float float_x, float_y, float_xb, float_yb; // used to break the flag into tiny quads

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen and depth buffer
    glLoadIdentity(); // reset the current matrix

    srand(time(NULL));
    glColor4ub(rand() % 256, rand() % 256, rand() % 256, 200);

    glTranslatef(0.0, 0.0, -12.0); // translate 12 units into the screen

    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, texture[0]); // select our texture

    glBegin(GL_QUADS);
    for (int x = 0; x < 44; x++) {
        for (int y = 0; y < 44; y++) {
            float_x = float(x) / 44.0;
            float_y = float(y) / 44.0;
            float_xb = float(x + 1) / 44.0;
            float_yb = float(y + 1) / 44.0;

            glTexCoord2f(float_x, float_y); // first texture coordinate(bottom left)
            glVertex3f(points[x][y][0], points[x][y][1], points[x][y][2]);
            glTexCoord2f(float_x, float_yb); // top left
            glVertex3f(points[x][y + 1][0], points[x][y + 1][1], points[x][y + 1][2]);
            glTexCoord2f(float_xb, float_yb); // top right
            glVertex3f(points[x + 1][y + 1][0], points[x + 1][y + 1][1], points[x + 1][y + 1][2]);
            glTexCoord2f(float_xb, float_y); // bottom right
            glVertex3f(points[x + 1][y][0], points[x + 1][y][1], points[x + 1][y][2]);
        }
    }
    glEnd();
    glLoadIdentity();

    glColor3f(0.96, 1.0, 1.0);
    glPrint(450, 105, "Version:V2.3.8", 0);

    glColor3f(0.96, 1.0, 1.0);
    glPrint(450, 85, "author:sunny", 1);

    glColor3f(0.96, 1.0, 1.0);
    glPrint(500, 65, "kolle", 1);
    glColor3f(0.96, 1.0, 1.0);
    glPrint(500, 45, "ying", 1);

    glColor3f(0.96, 1.0, 1.0);
    glPrint(cnt1, 25, "http://www.cowain.com.cn", 0);
    if(cnt1 -388 > 0)
    {
        if(cnt1 -388 -240 < 0)
        {
            glColor3f(0.96, 1.0, 1.0);
            glPrint(cnt1 -388-240 , 25, "http://www.cowain.com.cn", 0);
        }
        else
        {
            cnt1 = 0;
        }
    }
}
void MyGLWidget::glPrint(GLint x, GLint y, const char *string, int set)
{
    if (set >= 1)
        set = 1;
    else
        set = 0;

    glBindTexture(GL_TEXTURE_2D, texture[1]); // 绑定为字体纹理
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    //glOrtho(0, 1024, 0, 768, -1, 1); // 设置正投影的可视区域
    glOrtho(0, 640, 0, 480, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(x, y, 1); // 把字符原点移动到(x, y)位置
    glListBase(base - 32 + (128 * set)); // 选择字符集
    glCallLists(strlen(string), GL_BYTE, string); // 把字符串写到屏幕

    glMatrixMode(GL_PROJECTION); // 选择投影矩阵
    glPopMatrix(); // 设置为保存的矩阵
    glMatrixMode(GL_MODELVIEW); // 选择模型矩阵
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}
void MyGLWidget::timeOut()
{
    GLfloat hold; // temporarily holds a floating point value
    //if (wiggleCount == 2) { // used to slow down the wave(every 2nd frame only)
        for (int y = 0; y < 45; y++) {
            hold = points[0][y][2]; // store current value one left side of wave
            for (int x = 0; x < 44; x++) {
                points[x][y][2] = points[x + 1][y][2]; // current wave value equals value to the right
            }
            points[44][y][2] = hold; // last value becomes the far left stored value
        }
//        wiggleCount = 0;
  //  }
//    wiggleCount++;

        cnt1 += 1.0;
//        cnt2 += 0.0081;
    updateGL();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_F11:
        if (isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
}

void MyGLWidget::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    GLfloat dx = GLfloat(e->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(e->y() - lastPos.y()) / height();
    if (e->buttons() & Qt::LeftButton) {
        xrot -= 180 * dy;
        yrot -= 180 * dx;
        updateGL();
    } else if (e->buttons() & Qt::RightButton) {
        xrot += 180 * dy;
        zrot += 180 * dx;
        updateGL();
    }
    lastPos = e->pos();
}

void MyGLWidget::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
