#ifndef TTABLE_H
#define TTABLE_H

#include <CoGraphics/Graphics.h>

///////////////////////  控件基类 /////////////////////////////////////
class TTableView : public CoWidgets
{
    Q_OBJECT
public:
    TTableView(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TTableView();

public slots:
    CoItem *Copy();
    void    Copy(TTableView *TableFrom);
    virtual QRectF  boundingRect() const;
    virtual void SendTo(int Type , const QString msg,double ValueFloat = 0,
                         int ValueInt = 0, QString Text = tr(""));
    void    SetAttributeEvent(const int , const QString , uchar);

    bool    SetSize(int RowSum, int ColSum);
    void    SetRowSum(int sum);
    int     GetRowSum();
    int     GetColSum();

    void    SetColLength(int Index, int Len);
    int     GetColLength(int Index);
    void    SetRowLength(int Len);
    int     GetRowLength();

    int     GetCurrentCol();               // 获取当前列
    int     GetCurrentIndex();             // 获取当前行
    QString GetCurrentData(int Col);       // 获取当前行 某列的数据
    void    SetCell(int Row, int Col, QString Text);
    QString GetCell(int Row, int Col);
    void    Update();
    void    Clear();
    void    SingleMove(int step);
private:
    int     m_CurrentCol;     //当前列
    int     m_CurrentIndex;   //当前行
    int     m_RowSum;         //行总数
    int     m_ColSum;         //列总数

    int     m_RowLength;      //行高
    QVector<int>    m_ColLengthArray;  //列宽
    QList<QString>  m_Head;   //表头
    QList<QStringList> m_Data;//表格数据

    int     m_HeadHei;         //
    int     m_ScrWid;          //滚动条区域宽度
    int     m_ScrEndHei;       //滚动条上下端子高度
    int     m_ScrbarHei;       //滚动条高度
    QRectF  m_ScrbarRgn;       //滚动条区域
    QRectF  m_ScrRgn;          //滚动区域
    QRectF  m_ScrBarUpRgn;     //向上端子
    QRectF  m_ScrBarDownRgn;   //向下端子

    float   m_Move;            //滚动条相对偏移
    int     m_StartData;       //表格数据起始索引
    int     m_Count;           //
    int     m_ScrHeiMin;       //滚动条最小高度

    void    Destroy();
    void    UpdatePos();
    void    Draw(QPainter *painter);
    void    DrawHeader(QPainter *painter);
    void    DrawData(QPainter *painter);
    void    DrawScrRgn(QPainter *painter);

    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void    contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QString GetStringByType(int type);
    void    RefreshDataByMove(float move);
    void    ResizeData(int rowsum);
};

#endif // TTABLE_H
