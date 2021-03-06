#include "torquechart.h"

TorqueChart *TorqueChart::m_pInstance = NULL;
TorqueChart::TorqueChart()
{
  //曲线
  m_plinseries = new QSplineSeries();
  //图表
  m_pchart = new QChart();
  m_pchart->setTheme(QChart::ChartThemeDark);
  //chart容器
  m_pchatrview = new QChartView(m_pchart);
  //操作图表的轴
  m_paxis = new QValueAxis;
  m_pyaxis = new QValueAxis;
  m_pchart->legend()->hide();
  //增加线条
  m_pchart->addSeries(m_plinseries);
  //niaoyonmgmeiyou
  m_pchart->createDefaultAxes();
  m_pchart->setTitle("<h>实时扭矩图</h>");
  m_pchart->axisX()->setRange(0,6);
  m_pchart->axisY()->setRange(0,3);
  m_pchart->setAxisY(m_pyaxis,m_plinseries);
  m_pchart->axisY()->setTitleText("Torque(N/M)");
  m_pyaxis->setTickCount(10);
  m_pchart->setAxisX(m_paxis,m_plinseries);
  m_pchart->axisX()->setTitleText("Time(sec)");
  m_paxis->setTickCount(15);
  m_pHbox = new QHBoxLayout(this);
  m_pHbox->addWidget(m_pchatrview);

  this->setLayout(m_pHbox);
  this->setGeometry(50,50,500,500);
  //this->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:15px;"));

}

TorqueChart *TorqueChart::GetInstance()
{
    if(m_pInstance == NULL )
    {
        m_pInstance = new TorqueChart;
    }
    return m_pInstance;
}
///
/// \brief TorqueChart::AddChartPoint
/// \param x
/// \param y
///判断x和y的值 超值就重新设置范围
void TorqueChart::AddChartPoint(qreal x, qreal y)
{
    if(x >= m_paxis->max() *0.9)
    {
        m_pchart->axisX()->setRange(m_paxis->min(),x * 1.1);
    }
    if(x < m_paxis->min() )
    {
        m_pchart->axisX()->setRange(x * 1.1,m_paxis->max());
    }
    if(y >= m_pyaxis->max() *0.9)
    {
        m_pchart->axisY()->setRange(m_pyaxis->min(),y *1.1);
    }
    if(y < m_pyaxis->min())
    {
        m_pchart->axisY()->setRange(y * 1.1,m_pyaxis->max());
    }
    m_plinseries->append(x,y);

}
///
/// \brief TorqueChart::ClearChart
///清除曲线并重新设置 X,Y 的值
void TorqueChart::ClearChart()
{
    m_plinseries->clear();
    //m_pchart->axisX()->setRange(0,3.5);
    //m_pchart->axisY()->setRange(0,3.5);
}

TorqueChart::~TorqueChart()
{
    delete m_pchart;
    delete m_pHbox;
    delete m_plinseries;
    delete m_pchatrview;
    delete m_paxis;
    delete m_pyaxis;
    //delete this;
}
