#include "coordinate.h"


Coordinate::Coordinate()
{

}
///
/// \brief Coordinate::XYtoJog
/// \param l1  第一臂长
/// \param l2  第二臂长
/// \param x   大地坐标x值
/// \param y   大地指标y的值
/// \param lr1 第一臂分辨率
/// \param lr2 第二臂分辨率
/// \return 关节坐标
///
QPair<double, double> Coordinate::XYtoJog(double l1, double l2, double x, double y, int lr1, int lr2)
{
    QPair<double,double> JogPoint;
    static double lx = lr1/360;
    static double ly = lr2/360;
    //右手
    double L = sqrt(pow(x,2) + pow(y,2)); //末端到原点的长度
    double cosa1 = (pow(l1,2) + pow(L,2) - pow(l2,2))/(2*L*l1); //末端到原点的角度cos值
    double cosa1_A = 180 * acos(cosa1) /3.1415926;  //末端到原点的角度
    double cosa2 = (pow(l1,2) + pow(l2,2) - pow(L,2))/(2 *l1*l2);//一臂和2臂夹角的cos值
    double cosa2_A = 180 * acos(cosa2)/3.1415926;//一臂和2臂夹角
    double sina12 = x/L;
    double sina12_A = 180 * asin(sina12)/3.1415926;//末端和原点直线 和 x轴的夹角
    double sina34 = y/L;
    double sina34_A = 180 * asin(sina34)/3.1415926;//34 象限时
    //第一象限
    if(x > 0 && y > 0)
    {
        JogPoint.first = (sina12_A + cosa1_A) * lx;
        JogPoint.second = -(180 - cosa2_A) * ly;
        return JogPoint;
    }
    else if (x < 0 && y > 0)//第二象限
    {
        JogPoint.first = -(sina12_A - cosa1_A) * lx;
        JogPoint.second = -(180 - cosa2_A) * ly;
    }
    else if(x < 0 && y < 0)//第三象限
    {
        JogPoint.first = -(sina34_A - cosa1_A + 90) * lx;
        JogPoint.second = -(180 - cosa2_A) * ly;
    }
    else if(x >0 && y <0)
    {
        JogPoint.first = (90 +sina34_A + cosa1_A) *lx;
        JogPoint.second = -(180 - cosa2_A) *ly;
    }
    else//在原点
    {
        JogPoint.first = 0;
        JogPoint.second = 0;
    }
    return JogPoint;
}
///
/// \brief Coordinate::GetXYpoint
/// \param l1    第一臂长
/// \param l2    第二臂长
/// \param J1    第一关节脉冲
/// \param J2    第二关节脉冲
/// \param lr1   第一关节分辨率
/// \param lr2   第二关节分辨率
/// \return
///
QPair<double, double> Coordinate::GetXYpoint(double l1, double l2, int J1, int J2, double lr1, double lr2)
{
    //一圈两千个脉冲 2000, l1: 15 ,l2: 15
    QPair<double,double> point;
    double angle_l12 = qAbs(J2)/lr2 * 360;//第二关节旋转角度
    //qDebug()<<"第二关节旋转角度"<<angle_l12;
    if(angle_l12 >= 180)
    {
        return point;
    }
    double angle_j = 180.0 - angle_l12;  //第一关节和第二关节夹角 /180*3.1415926
    //double line_l_2 =  pow(l1,2) + pow(l2,2) - (cos(angle_j) *2*l1*l2);
    double line_l_2 = 450 - (cos(angle_j/180*3.1415926) * 450);
    //qDebug()<< " cos(angle_j)" <<angle_j<<"°  ="<<line_l_2;
    line_l = sqrt(line_l_2);//对边长
    //qDebug()<<"对边长："<< line_l;
    double cosa = (pow(line_l,2) + pow(l1,2) - pow(l2,2))/(2*l2*line_l);
    //double a = cosa/3.1415926 * 180;
    double a = 180 * acos(cosa) /3.1415926;//一臂与工具坐标点之间的角度
    //qDebug()<<"虚拟线和一臂的夹角:"<<a;
    if(J1 ==0 && J2 == 0)
    {
        return point;
    }
    if(J1 > 0)//1轴右手
    {
        angle_l13 = (J1)/lr1 * 360;  //一轴旋转的角度
        if(J2 < 0)//2轴左手
        {
            angel = (angle_l13 - a)/180* 3.1415926;
            if(angle_l13 < 90)  //在第一或者第二象限
            {
                if((90 - angle_l13 + a) > 90)//第2象限
                {
                    point.first = qAbs(sin(angel) * line_l) * -1;
                    point.second = qAbs(cos(angel) * line_l);
                }
                else //第1象限
                {
                    point.first = qAbs(sin(angel) * line_l);
                    point.second = qAbs(cos(angel) * line_l);
                }
            }
            else  //在第一或者第四象限
            {
                if((180 - angle_l13 + a) >90) //第一象限
                {
                    point.first = qAbs(sin(angel) * line_l) * -1;
                    point.second = qAbs(cos(angel) * line_l);
                }
                else
                {
                    point.first = qAbs(sin(angel) * line_l) * -1;
                    point.second = qAbs(cos(angel) * line_l)* -1;
                }

            }

        }
        else//2轴右手
        {
            double  angel_c = (angle_l13 + a)/180* 3.1415926;
            angel = (angle_l13 + a);
            if(angel >= 0 && angel <= 90)//第一象限
            {
                point.first = qAbs(sin(angel_c) * line_l);
                point.second = qAbs(cos(angel_c) * line_l);
            }
            else if(angel > 90 && angel < 180)//第四象限
            {
                point.first = qAbs(sin(angel_c) * line_l);
                point.second = qAbs(cos(angel_c) * line_l) * -1;

            }

        }

    }
    else//1轴左手
    {
        angle_l13 = qAbs((J1)/lr1 * 360);  //1轴旋转的角度 0
        if(J2 > 0)//2轴右手
        {
            angel = (angle_l13 - a)/180* 3.1415926;//转化成弧度
            if((angle_l13 - a)<90)//第一象限
            {
                point.first = qAbs(sin(angel) * line_l);
                point.second = qAbs(cos(angel) * line_l);
            }
            else if((90 < (angle_l13 - a)) && ((angle_l13 - a) < 180))//第二象限
            {
                point.first = qAbs(sin(angel) * line_l)* -1;
                point.second = qAbs(cos(angel) * line_l);
            }
            else if((angle_l13 - a)> 180)//第三象限
            {
                point.first = qAbs(sin(angel) * line_l)* -1;
                point.second = qAbs(cos(angel) * line_l)* -1;
            }

        }
        else //2左手，角度小于0
        {
            angel = (angle_l13 + a)/180* 3.1415926;
            if((angle_l13 + a)<90)//第二象限
            {
                point.first = qAbs(sin(angel) * line_l) * -1;
                point.second = qAbs(cos(angel) * line_l);
            }
            else//第三象限
            {
                point.first = qAbs(sin(angel) * line_l)* -1;
                point.second = qAbs(cos(angel) * line_l)* -1;
            }

        }

    }
    return point;

}

QMap<QString,double> ireturn;

QMap<QString, double> Coordinate::XYtoJ_W(QMap<QString, QMap<QString, double> > PointMap)
{
     ireturn.clear();
    if(PointMap.isEmpty())
        return ireturn;
    for(auto it = PointMap.begin();it != PointMap.end(); ++it)
    {
        QString Axisname = it.value().begin().key();
        double Axisvalue = it.value().begin().value();
        if(Axisname == "J1")
        {
            Axisvalue = Axisvalue/1080 * 0.01;
        }
        else if(Axisname == "J2")
        {
            Axisvalue = Axisvalue/1080 * 0.01;
        }
        else if(Axisname == "J3")
        {
            Axisvalue = Axisvalue/1080 * 0.01;
        }
        else if(Axisname == "J4")
        {
            Axisvalue = Axisvalue/1080 * 0.01;
        }
        else if(Axisname == "J5")
        {
            Axisvalue = Axisvalue/1080 * 0.01;
        }
        else if(Axisname == "J6")
        {
            Axisvalue = Axisvalue/1080 * 0.01;
        }
        ireturn.insert(Axisname,Axisvalue);
    }
    return ireturn;

}

void Coordinate::CovertTo_J(SWCONFIGXML_LCSP_STRU &p_pointStr)
{
//    if(p_pointStr == NULL)
//        return;
   for(auto it = p_pointStr.LCSPCPos.begin();it != p_pointStr.LCSPCPos.end();it++)//触发集合
   {
       for(auto im = it->LCSPTrigInfo.begin();im != it->LCSPTrigInfo.end();im++)//触发信息
       {
            for(auto in = im->LCSPTrigCPos.begin();in != im->LCSPTrigCPos.end();in++) //触发位置
            {
                PointStr trigstr;
                trigstr = Getgear(in->id);
                if(trigstr.elegeart != -1)
                {
                    in->id = trigstr.axisId;
                    in->pos = in->pos*trigstr.elegeart;
                }
            }

       }
       QMap<QString ,LCSP_CPOSINFO_STRU> myCSPosMap;
       for(auto ito = it->LCSCPosMap.begin();ito != it->LCSCPosMap.end();ito++)//点位信息
       {
           //it->LCSCPosMap.remove(ito.key());
           PointStr posMap;
           posMap = Getgear(ito.key());
           if(posMap .elegeart != -1)
           {
//               it->LCSCPosMap.erase(ito); //先删除再增加
               LCSP_CPOSINFO_STRU posvalue;
               posvalue.pos =  ito.value().pos*posMap.elegeart;
               //it->LCSCPosMap.insert(posMap.axisId,posvalue);
               myCSPosMap.insert(posMap.axisId,posvalue);
           }

       }
       it->LCSCPosMap = myCSPosMap;
   }

}
///
/// \brief Coordinate::CovertTo_X
/// \param P_pointStr
///
void Coordinate::CovertTo_X(SWCONFIGXML_LCSP_STRU &p_pointStr, QMap<QString, QVector<QString>>coordMap)
{
//    if(p_pointStr == NULL)
//        return;
    for(auto it = p_pointStr.LCSPCPos.begin();it != p_pointStr.LCSPCPos.end();it++)//触发集合
    {
        for(auto im = it->LCSPTrigInfo.begin();im != it->LCSPTrigInfo.end();im++)//触发信息
        {
            for(auto in = im->LCSPTrigCPos.begin();in != im->LCSPTrigCPos.end();in++) //触发位置
            {
                for(auto item = coordMap.begin();item != coordMap.end();item++)
                {
                    if(item.value().contains(in->id))
                    {
                        PointStr trigstr;
                        trigstr = Getgear(item.key());
                        if(trigstr.elegeart != -1)
                        {
                            in->id = item.key();
                            in->pos = in->pos/trigstr.elegeart ;
                        }
                    }
                }
            }
        }
        QMap<QString ,LCSP_CPOSINFO_STRU> myLCSCPosMap;
        for(auto ito = it->LCSCPosMap.begin();ito != it->LCSCPosMap.end();ito++)//点位信息
        {
            //it->LCSCPosMap.remove(ito.key());//不改变原有的 最后替换全部的map
            for(auto itea = coordMap.begin();itea != coordMap.end();itea++)
            {
                if(itea.value().contains(ito.key()))
                {
                    PointStr posMap;
                    posMap = Getgear(itea.key());
                    if(posMap .elegeart != -1)
                    {
                        //it->LCSCPosMap.erase(ito); //先删除再增加
                        LCSP_CPOSINFO_STRU posvalue;
                        posvalue.pos = ito.value().pos/posMap.elegeart;
                        if(!it->LCSCPosMap.contains(itea.key()))
                        {
                            //it->LCSCPosMap.insert(itea.key(),posvalue);
                            myLCSCPosMap.insert(itea.key(),posvalue);
                        }
                    }
                }

            }
        }
        it->LCSCPosMap = myLCSCPosMap;
    }
}
///
/// \brief Coordinate::Getgear
/// \param coordina 坐标名称
/// \return ID AxisName and elegearat
///
PointStr Coordinate::Getgear(QString coordina)
{
   QMutexLocker Locker(&m_mutex);
    PointStr postr;
    for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlCoordinateMap.begin(); \
        it!= ConfigData::GetInstance()->xmlMap.hwConfigXml\
        .hwConfigXmlCoordinateMap.end();it++)
    {
        if(coordina == it.key())
        {
          postr.elegeart = it.value().hwArmInfoMap.begin().value().elegearat;
          postr.axisId = it.value().hwArmInfoMap.begin().value().Id;
        }
    }
    return postr;
}


























