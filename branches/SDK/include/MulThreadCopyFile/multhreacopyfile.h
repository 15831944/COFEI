#ifndef MULTHREACOPYFILE_H
#define MULTHREACOPYFILE_H
#include <QString>
#include <iostream>
#include "multhreacopyfile_global.h"
using namespace std;
class MULTHREACOPYFILESHARED_EXPORT MulThreaCopyFile
{

public:
    MulThreaCopyFile();
    int  ExcCopy(QString srcfile,QString dfile);
private:
    struct thread_block
    {
        int infd; ///<*文件句柄
        int outfd;
        size_t start;///<*文件的写入起始位置
        size_t end; ///<* 文件写入的终止位置[first ,last)开区间
    };
private:
    size_t get_filesize(int fd);
    static  void *thread_copy_fn(void *arg);
};

#endif // MULTHREACOPYFILE_H
