#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "eventmanager_global.h"
//sdfdsfds
class EVENTMANAGERSHARED_EXPORT MyObject
{
};
template<typename rtnTtpe,typename ArgType>
class EVENTMANAGERSHARED_EXPORT EventManager
{
    //使每个事件最多关联响应的函数个数
#define EVENT_LIST_MAX_NUM  (10)
    typedef rtnTtpe (MyObject::*pMemFunc)(ArgType id);

public:
    EventManager()
    {
        m_totalFunc = 0;
        m_obj = NULL;
        for (int i = 0; i < EVENT_LIST_MAX_NUM; i++)
        {
            m_func[i] = NULL;
        }
    }

    //关联回调成员函数
    template <class _func_type>
    void associate(MyObject *obj, _func_type func)
    {
        m_obj = obj;
        m_func[m_totalFunc] = static_cast<pMemFunc> (func);
        m_totalFunc++;
    }
    //删除事件关联回调成员函数
    template <class _func_type>
    void disAssociate(MyObject *obj, _func_type func)
    {
        if (obj != m_obj)
        {
            return;
        }
        int i ;
        //查找
        for (i = 0; i < m_totalFunc; i++)
        {
            if (m_func[i] == func)
            {
                break;
            }
        }

        //移动删除
        for (i ; i < m_totalFunc - 1; i++)
        {
            m_func[i] = m_func[i + 1];
        }

        m_func[i] = NULL;
        m_totalFunc --;
    }

    //执行关联的回调函数
    void sendEvent(ArgType id)
    {
        for (int i = 0; i < m_totalFunc; i++)
        {
            if (m_func[i] != NULL)
            {
                ((m_obj->*pMemFunc(m_func[i])))(id);
            }
        }
    }
private:
    MyObject* m_obj;
    pMemFunc m_func[EVENT_LIST_MAX_NUM];
    int m_totalFunc;
};


#endif // EVENTMANAGER_H
