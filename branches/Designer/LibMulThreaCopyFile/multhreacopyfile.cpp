#include "multhreacopyfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#define THREADS_COUNT 4
#define THREADS_BUFF_SIZE (1*1024)
pthread_mutex_t pmxt;
MulThreaCopyFile::MulThreaCopyFile()
{
}

int MulThreaCopyFile::ExcCopy(QString srcfile, QString dfile)
{
    //打开文件
    char a[50] = {0};
    char b[50] = {0};
    strcpy(a,dfile.toStdString().c_str());
    strcpy(b,srcfile.toStdString().c_str());
    char *sfile  = NULL;
    char *tarfile = NULL;
    tarfile = a;
    sfile = b;
    int infd = open(sfile,O_RDONLY);
    int outfd = open(tarfile,O_CREAT|O_WRONLY,0644);
    if(infd == -1|| -1 ==outfd)
    {
        printf("error while open file \n");
        return -1;
    }
    size_t file_size = get_filesize(infd);//得到文件大小
    size_t thread_size = THREADS_COUNT;
    ///动态申请，注意释放
    struct thread_block *blocks = (struct thread_block *)
            malloc(sizeof(struct thread_block )* thread_size);
    size_t percent = file_size / thread_size;
  //  printf("filesize = %d\t percent_blocks = %d\n",file_size,percent);
    //init-thread-block
    for(uint i = 0; i < thread_size;++i)
    {
        blocks[i].infd = infd;
        blocks[i].outfd = outfd;
        blocks[i].start = i * percent;
        blocks[i].end = blocks[i].start + percent;
    }
    //the last thread
    //  blocks[i].end = file_size;
    pthread_t ptid[thread_size];
    ///创建线程  此部分要注意！！！！只要一创建就直接加入主线程释放资源；不能把所有都创建好了在释放！！！！
    for(uint i = 0 ; i < thread_size; ++i)
    {
        int ret = pthread_create(&ptid[i],NULL,thread_copy_fn,&blocks[i]);
        if(ret != 0)
        {
          //  printf("thread_create error ret=%d\n",ret);
        }
        usleep(1000);
        pthread_join(ptid[i],NULL);
    }
    ///线程Join！！！！此步骤放在每一个线程创建的时候使用
    ///释放资源
    free(blocks);
    close(infd);
    close(outfd);
    printf("Copy File Successfully \n");
    return 0;
}

size_t MulThreaCopyFile::get_filesize(int fd)
{
    struct stat st;
    fstat(fd,&st);
    return st.st_size;
}

void *MulThreaCopyFile::thread_copy_fn(void *arg)
{
    //pthread_cond_t cond;
    struct thread_block *block = (struct thread_block *)arg;
    char buf[THREADS_BUFF_SIZE];
    size_t count = block->start;
    ///lseek到同样的位置
    lseek(block->infd,block->start,SEEK_SET);
    lseek(block->outfd,block->start,SEEK_SET);
    int bytes_read;
    int bytes_write = -1;
    while(true)
    {
        if(count >= block->end)
        {
            break;
        }
        pthread_mutex_lock(&pmxt);
        bytes_read = read(block->infd,buf,sizeof(buf));//为什么读出来是字节数为0????解决问题
        // read函数是负责从fd中读取内容.成功时,read返回实际所读的字节数,如果返回的值是0,表示已经读到文件的结束了.
        pthread_mutex_unlock(&pmxt);
        if(bytes_read >0)
        {
            count += bytes_read;
            //error while read
            if((bytes_read < 0)&&(errno !=EINTR))
                break;
            char *ptr_write = buf;
            //悲剧的少了个括号，于是bytes_write == 1
            while(true)
            {
                bytes_write = write(block->outfd,ptr_write,bytes_read);
                if(bytes_write == 0)
                {
                    break;
                }
                if((bytes_write < 0)&&(errno!=EINTR))
                {
                    break;
                }
                if(bytes_write == bytes_read)
                {
                    break;
                }
                else if(bytes_write > 0)
                {
                    ptr_write += bytes_write;
                    bytes_read -= bytes_write;
                }

                // printf("thread = %ld\t write = %ld\t read %d\n",pthread_self(),bytes_write,bytes_read);
                //qDebug()<<"write"<< pthread_self() << bytes_write<<bytes_read;
            }//end-write;
            ///error while write
            if(bytes_write < 0)
            {
                break;
            }

        }//end-if
        else  if(bytes_read == 0){
            break;
        }
    }//end-read
    // pthread_detach(pthread_self());
    printf("#####Thread exit %ld#####\n",pthread_self());
    pthread_exit(NULL);
}
