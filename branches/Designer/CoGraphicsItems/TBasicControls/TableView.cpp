#include "TableView.h"
#include "CoGraphics/Base/Base.h"
#include "AttributeWidget.h"
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QDebug>
#include <QTimer>
#include <QDateTime>

TTableView::TTableView(double StartX, double StartY, double StopX, double StopY,
               const Qt::PenStyle &LineStyle, const int LineWidth,
               const QColor &LineColor, const QColor &BackColor) :
    CoWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::TableView);
    //SetDragAble(false);
    SetFontSize(10);
    m_CurrentIndex = -1;
    m_CurrentCol = -1;
    m_RowSum = 10;
    m_ColSum = 4;
    m_RowLength = 0;
    m_HeadHei = 32;
    SetSize(m_RowSum,m_ColSum);
    m_Head<< tr("ID") << tr("时间") << tr("等级") << tr("信息");

    m_ScrHeiMin = 40;
    m_Count = 0;
    m_Move = 0;
    m_StartData = 0;
    m_ScrWid = 15;
    m_ScrEndHei = 15;
    m_ScrbarHei = m_StopY - m_StartY - m_HeadHei - (m_ScrEndHei*2);

    ResizeData(m_RowSum);

//    SetDragAble(false);
//    SetSelectAble(false);
//    SetMoveAble(false);

//    QTimer *timer = new QTimer();
//    connect(timer,&QTimer::timeout,this,[=]{

//        QString time = QDateTime::currentDateTime().toString("YYYY-MM-DD hh:mm:ss.zzz");
//        QString msg = tr("[%1] [Error] {Test} Open file /root/用户文件夹/运行配置/主流程.txt failure").arg(time);
//        SendTo(2,msg);

//    });
//    timer->start(500);

}

void TTableView::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu *menu = new QMenu();
    QAction *removeAction = menu->addAction(tr("清空"));
    connect(removeAction,SIGNAL(triggered()),this,SLOT(Clear()));
    menu->exec(event->screenPos());
}

CoItem* TTableView::Copy()
{
    TTableView *Table = new TTableView();

    Table->Copy(this);

    return Table;
}

void TTableView::Copy(TTableView *TableFrom)
{
    if(NULL == TableFrom)
    {
        return;
    }

    CoWidgets::Copy(TableFrom);

    // 大小
    this->SetSize(TableFrom->GetRowSum(), TableFrom->GetColSum());

    // 行高
    this->SetRowLength(TableFrom->GetRowLength());

    // 列宽
    for(int i = 0; i < TableFrom->GetColSum(); i++)
    {
        this->SetColLength(i, TableFrom->GetColLength(i));
    }

    // 内容
    for(int i = 0; i < TableFrom->GetRowSum(); i++)
    {
        for(int j = 0; j < TableFrom->GetColSum(); j++)
        {
            this->SetCell(i, j, TableFrom->GetCell(i, j));
        }
    }
}

QRectF TTableView::boundingRect() const
{
    QRectF mRect(m_StartX, m_StartY, m_StopX-m_StartX, m_StopY-m_StartY);

    const int padding = (m_LineWidth + 1) / 2;
    mRect.adjust(-padding, -padding, 0, 0);

    return mRect;
}

void TTableView::SendTo(int Type, const QString msg, double, int , QString)
{
    QString msgTmp = msg.right(msg.length()-1);
    QString type = GetStringByType(Type);
    QString info = msgTmp.section("} " ,1,1);
    QString time = msgTmp.section("] [",0,0);
    time = time.mid(5,14);

    if(info.isEmpty())
        return;

    if(m_Data.size() > 199 || m_Count < (m_RowSum+1))
    {
        m_Data.removeLast();
    }

    QStringList infoList;
    infoList.clear();
    infoList << time << type << info;
    m_Data.insert(0,infoList);

    if(m_Count < (m_RowSum+1))
        ++m_Count;

    Update();
}

void TTableView::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    CoWidgets::SetAttributeEvent(type,value,uc);
    switch (type)
    {
    case CoAttributeWidget::RowSum:
        SetRowSum(value.toInt());
        break;
    default:
        break;
    }
}

TTableView::~TTableView()
{
    Destroy();
}

void TTableView::Destroy()
{
    m_ColLengthArray.clear();
    m_Data.clear();
    m_RowSum = 0;
    m_ColSum = 0;
}

void TTableView::UpdatePos()
{
    if(m_ColLengthArray.size() < m_ColSum)
    {
        return;
    }

//    int Height = 0;

//    for(int i = 0; i < m_ColSum; i++)
//    {
//        Height += m_ColLengthArray[i];
//    }
//    MoveTo(GetCurrentStartX(), GetCurrentStartY(),
//            GetCurrentStartX() + Height,
//            GetCurrentStartY() + m_RowLength * (m_RowSum+1));
}

bool TTableView::SetSize(int RowSum, int ColSum)
{
    m_ColLengthArray.clear();
    m_RowSum = RowSum;
    m_ColSum = ColSum;

    // 计算列宽
    //默认就是四列
    double total = GetCurrentStopX() - GetCurrentStartX();
    if(total > 225)
    {
        //列宽大于270时，前两列固定，最后一列拉伸
        m_ColLengthArray.append(20);
        m_ColLengthArray.append(95);
        m_ColLengthArray.append(55);
        m_ColLengthArray.append(total - 170);
        m_ScrWid = 15;
    }
    else
    {
        //列宽小于270时，整体按比例缩放，比例因子是0.46:0.27:0.27
        m_ColLengthArray.append(total*0.09);
        m_ColLengthArray.append(total*0.42);
        m_ColLengthArray.append(total*0.24);
        m_ColLengthArray.append(total*0.25);
        m_ScrWid = total*0.07;
    }

    // 行高
    m_RowLength = (GetCurrentStopY() - GetCurrentStartY() - m_HeadHei) / m_RowSum;

    if(m_RowLength > 15)
    {
        //上下端子高度
        m_ScrEndHei = 15;
        //滚动条高度
        m_ScrbarHei = m_StopY - m_StartY - m_HeadHei - (m_ScrEndHei*2);

        //数据量大于可显示的行数时，滚动条变短
        if(m_Data.size() > m_RowSum && m_ScrbarHei > m_ScrHeiMin)
        {
            int step = (200 - m_RowSum) / 5;
            float stepLen = (GetCurrentStopY() - GetCurrentStartY() - m_HeadHei - (m_ScrEndHei*2) - m_ScrHeiMin)/step;
            int factor = ((m_Data.size() - m_RowSum) / 5) + 1;

            m_ScrbarHei = m_ScrbarHei - (stepLen*factor);
            m_ScrbarHei = (m_ScrbarHei < m_ScrHeiMin) ? m_ScrHeiMin : m_ScrbarHei;
        }
    }
    else
    {
        m_ScrEndHei = (m_RowLength*m_RowSum + m_HeadHei)*0.09;
        m_ScrbarHei = (m_RowLength*m_RowSum + m_HeadHei)*0.24;
    }

    // 更新
    UpdatePos();
    return true;
}

void TTableView::SetRowSum(int sum)
{
    m_RowSum = sum;
    ResizeData(m_RowSum);
    Update();
}

int TTableView::GetRowSum()
{
    return m_RowSum;
}

int TTableView::GetColSum()
{
    return m_ColSum;
}

void TTableView::SetColLength(int Index, int Len)
{
    if(m_ColLengthArray.size() > Index && Index >= 0 && Index < m_ColSum)
    {
        m_ColLengthArray[Index] = Len;
        UpdatePos();
    }
}

int TTableView::GetColLength(int Index)
{
    if(m_ColLengthArray.size() > Index && Index >= 0 && Index < m_ColSum)
    {
        return m_ColLengthArray[Index];
    }
    return 0;
}

void TTableView::SetRowLength(int Len)
{
    m_RowLength = Len;
    UpdatePos();
}

int TTableView::GetRowLength()
{
    return m_RowLength;
}

int TTableView::GetCurrentCol()
{
    return m_CurrentCol;
}

int TTableView::GetCurrentIndex()
{
    return m_CurrentIndex;
}

QString TTableView::GetCurrentData(int Col)
{
    return GetCell(GetCurrentIndex(), Col);
}

void TTableView::SetCell(int Row, int Col, QString Text)
{
    if(Row >= 0 && Row < m_RowSum && Col >= 0 && Col < m_ColSum)
    {
//        m_Data[Row][Col] = Text;
        QStringList infoList = m_Data.at(Row);
        infoList.replace(Col,Text);
        m_Data.replace(Row,infoList);
    }
}

QString TTableView::GetCell(int Row, int Col)
{
    QString Text = tr("");
    if(Row >= 0 && Row < m_RowSum && Col >= 0 && Col < m_ColSum)
    {
//        Text = m_Data[Row][Col];
        Text = m_Data.at(Row).at(Col);
    }
    return Text;
}

void TTableView::Update()
{
    SetSize(m_RowSum,m_ColSum);
    update();
}

void TTableView::Clear()
{
    m_Count = 0;
    m_Move = 0;
    m_StartData = 0;
    m_ScrbarHei = m_StopY - m_StartY - m_HeadHei - (m_ScrEndHei*2);

    m_Data.clear();
    ResizeData(m_RowSum);
    Update();
}

void TTableView::SingleMove(int step)
{
    float max = m_StopY - m_StartY - (m_ScrEndHei*2) - m_ScrbarHei - m_HeadHei;
    int rem = m_Data.size() - m_RowSum;
    double move = (max / rem)*step;
    RefreshDataByMove(move);
    Update();
}

void TTableView::Draw(QPainter *painter)
{
    DrawHeader(painter);
    DrawData(painter);
    DrawScrRgn(painter);
}

void TTableView::DrawHeader(QPainter *painter)
{
    //绘制表头
    painter->setBrush(TBase::GetNewColor(m_BackGroundColor, 90));
    double Width = 0;
    for(int i = 0; i < m_ColSum; i++)
    {
        //绘制单元格
        painter->drawRect(m_StartX + Width,m_StartY,m_ColLengthArray[i],m_HeadHei);

        //绘制表头内容
        painter->setFont(m_font);
        painter->drawText(m_StartX + Width,m_StartY,m_ColLengthArray[i],m_HeadHei,Qt::AlignCenter,m_Head[i]);
        Width += m_ColLengthArray[i];
    }
}

void TTableView::DrawData(QPainter *painter)
{
    //绘制表格
    for(int i = 0; i < m_RowSum; i++)
    {
        double Width = 0;

        //绘制单元格及数据
        if(m_CurrentIndex == i)
        {
            //当前行高亮
            painter->setBrush(Qt::blue);
        }
        else
        {
            painter->setBrush(m_BackGroundColor);
        }

        for(int j = 0; j < m_ColSum; j++)
        {
            //框
            painter->drawRect(m_StartX + Width,m_StartY + m_HeadHei  + i*m_RowLength,
                              m_ColLengthArray[j],m_RowLength);
            //绘制表格内容
            if(j == 0)
            {
                //绘制表格索引，第一列
                painter->drawText(m_StartX + Width,m_StartY + m_HeadHei + i*m_RowLength ,
                                  m_ColLengthArray[j],m_RowLength,Qt::AlignCenter,QString::number(i+1+m_StartData));
            }
            else
            {
                //绘制表格数据
                if(m_Data.size() > 0)
                {
                    painter->drawText(m_StartX + Width,m_StartY+ m_HeadHei + i*m_RowLength,
                                      m_ColLengthArray[j],m_RowLength,Qt::AlignCenter,m_Data.at(i+m_StartData).at(j-1));
                }
            }
            Width += m_ColLengthArray[j];
        }
    }
}

void TTableView::DrawScrRgn(QPainter *painter)
{
    //绘制滚动条
    //滚动区
    m_ScrRgn = QRect(m_StopX - m_ScrWid,m_StartY + m_HeadHei,m_ScrWid,m_RowLength*m_RowSum);
    painter->fillRect(m_ScrRgn,QBrush(QColor(255,255,255,80)));
    //上下端子
    m_ScrBarUpRgn = QRect(m_StopX - m_ScrWid,m_StartY + m_HeadHei,m_ScrWid,m_ScrEndHei);
    m_ScrBarDownRgn = QRect(m_StopX - m_ScrWid,(m_StartY + m_HeadHei + m_RowLength*m_RowSum)- m_ScrEndHei,m_ScrWid,m_ScrEndHei);
    painter->fillRect(m_ScrBarUpRgn,QBrush(QColor(255,255,255,200)));
    painter->drawPixmap(m_StopX - m_ScrWid,m_StartY + m_HeadHei,m_ScrWid,m_ScrEndHei,QPixmap(":Images/Up.png"));
    painter->fillRect(m_ScrBarDownRgn,QBrush(QColor(255,255,255,200)));
    painter->drawPixmap(m_StopX - m_ScrWid,(m_StartY + m_HeadHei + m_RowLength*m_RowSum)- m_ScrEndHei,m_ScrWid,m_ScrEndHei,QPixmap(":Images/Down.png"));
    //滚动条
    m_ScrbarRgn = QRect(m_StopX - m_ScrWid,m_Move + m_StartY + m_ScrEndHei + m_HeadHei,m_ScrWid,m_ScrbarHei - 7);
    painter->fillRect(m_ScrbarRgn,QBrush(Qt::darkGray));
}

void TTableView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
        if(m_ScrBarUpRgn.contains(QPointF(event->scenePos().x(),event->scenePos().y())))
            SingleMove(-1);
        else if(m_ScrBarDownRgn.contains(QPointF(event->scenePos().x(),event->scenePos().y())))
            SingleMove(1);


        if(!m_ScrRgn.contains(QPointF(event->scenePos().x(),event->scenePos().y())))
        {
            // 判断在第几行
            if(m_RowLength > 0 && m_RowSum > 0)
            {
                m_CurrentIndex = (event->pos().y() - m_StartY - m_HeadHei) / m_RowLength;
            }
        }
//        // 判断在第几列
//        if(m_ColLengthArray.size() > GetColSum())
//        {
//            int Height = event->pos().x() - m_StartX;

//            for(int i = 0; i < GetColSum(); i++)
//            {
//                if(Height <= m_ColLengthArray[i])
//                {
//                    m_CurrentCol = i;
//                    break;
//                }
//                else
//                {
//                    Height -= m_ColLengthArray[i];
//                }
//            }
//        }
    }
    CoItem::mousePressEvent(event);
}

void TTableView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!GetMoveAble() && !GetDragAble() && !GetSelectAble())
    {
        if(event->scenePos().y() < (m_ScrRgn.y() + m_ScrRgn.height()))
        {
            //鼠标在y方向的位移量
            int move = event->pos().y() - event->lastPos().y();
            RefreshDataByMove(move);
        }
        Update();
    }
    else
    {
        CoItem::mouseMoveEvent(event);
    }
}

void TTableView::RefreshDataByMove(float move)
{
    float max = m_StopY - m_StartY - (m_ScrEndHei*2) - m_ScrbarHei - m_HeadHei;
    if(max > 0)
    {
        float tmp = abs(move);
        tmp = (tmp > max) ? max : tmp;
        tmp = (move > 0) ? tmp :(tmp*(-1));

        //计算滚动条的相对偏移量
        m_Move = m_Move + tmp;
        m_Move = (m_Move > 0) ? m_Move:0;
        m_Move = (m_Move < max) ? m_Move : max;

        int end = m_Data.size() - m_RowSum;
        m_StartData = (m_Move / max )*m_Data.size();
        m_StartData = (m_StartData > end) ? end : m_StartData;
    }
}

void TTableView::ResizeData(int rowsum)
{
    m_Data.reserve(rowsum);
    for(int i = 0; i < rowsum;++i)
    {
        QStringList list;
        list.reserve(3);
        for(int j = 0; j < 3; ++j)
            list.append("");
        m_Data.append(list);
    }
}

QString TTableView::GetStringByType(int type)
{
    QString str = "";
    switch (type) {
    case 1: str = tr("Error"); break;
    case 2: str = tr("Warning"); break;
    case 3: str = tr("Info"); break;
    default:
        break;
    }
    return str;
}

