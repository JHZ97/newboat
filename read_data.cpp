#include "mainwindow.h"
#include "ui_mainwindow.h"

//static int remain=0;

int hex2int(char c1 ,char c2 ,char c3,char c4);
short hex2short(char c1,char c2);

bool check_frame(QByteArray frame);

//读取数据
void MainWindow::readData()
{
    QByteArray buf;
    buf = serial->readAll();
     qDebug() << "read: " << buf.toHex();
    //ui->status_r->append(buf.toHex()+"\n");
    frame_devide(buf);
   }


//检查数据头
bool check_header(QByteArray buf)
{
    if (buf[0]!=(char)0xA5||buf[1]!=(char)0x5A)  return false;
    return true;
}

/*
//帧拆分
void MainWindow::frame_devide(QByteArray buf)
{
    static QByteArray frame;
    if((buf.size()<=2)&&(buf[0]==(char)0xa5))
    {
        frame=buf;
        return;
    }
    if(!frame.isNull())
    {
        buf=frame.append(buf);
        frame.clear();
    }
    if (remain==0&&check_header(buf))
     {
         int len = buf.size();
         int size = buf[2]+2;
         if(len==size)
         {
             qDebug()<<"process:"<<buf.toHex();
             emit frame_ready(buf);
         }
         else if(len<size)
         {
             frame=buf;
             remain=size - len;
          }
         else
         {
             emit frame_ready(buf.left(size));
             qDebug()<<"process:"<<buf.left(size).toHex();
             frame_devide(buf.right(len-size));
         }
     }
    else if(remain!=0)
    {
        int len = buf.size();
        if(len==remain)
        {
            //frame.append(buf);
            qDebug()<<"process:"<<frame.toHex();
            emit frame_ready(buf);
            remain=0;
                    }
        else if (len>remain) {

            frame.append(buf.left(remain));
            qDebug()<<"process:"<<frame.toHex();
            emit frame_ready(frame);
            int dif = len-remain;
            remain=0;
            frame_devide(buf.right(dif));

            remain=0;
            frame_devide(buf);
        }
        else
            {
            //frame.append(buf);
            remain=remain-len;
        }
    }

}
*/

//帧拆分
void MainWindow::frame_devide(QByteArray buf)
{
    static QByteArray frame;
    if((buf.size()<=2)&&(buf[0]==(char)0xa5))
    {
        frame=buf;
        return;
    }
    if(!frame.isNull())
    {
        buf=frame.append(buf);
        frame.clear();
    }
    if (check_header(buf))
     {
         int len = buf.size();
         int size = buf[2]+2;
         if(len==size)
         {
             qDebug()<<"process:"<<buf.toHex();
             emit frame_ready(buf);
         }
         else if(len<size)
         {
             frame=buf;
          }
         else
         {
             emit frame_ready(buf.left(size));
             qDebug()<<"process:"<<buf.left(size).toHex();
             frame_devide(buf.right(len-size));
         }
     }


}


//帧检验
bool check_frame(QByteArray frame)
{
    if (frame[0]!=(char)0xa5||frame[1]!=(char)0x5a)
    {
        //ui->status_r->append("header error!!");
        return false;
    }
    if(!frame.endsWith((char)0xaa))
    {
        //ui->status_r->append("tailer error!!");
        return false;
    }
    char flag=0;
    int len = frame.size();
    for(int i=2;i<len-2;i++)
    {
        flag+=frame[i];
    }
    if(flag!=frame[len-2])
    {
        //ui->status_r->append("check error!!");
        return false;
    }
    return true;
}

//帧处理
void MainWindow::frame_process(QByteArray buf)
{
    if(check_frame(buf))
    {
        static QString hint;
        if(send_map.contains(buf))
        {
            auto values = send_map.values(buf);
            for(auto i:values)
            {
                hint = i->hint;
                i->sucess_send();
            }
            send_map.remove(buf);
            ui->status->append(hint+QStringLiteral("成功发送：")+buf.toHex());
        }
        else if (buf[3]==(char)0xa1)
            {
                //solve_sensor(buf);
            }
    }
    else {
        //remain=0;
    }

}



int hex2int(char c1 ,char c2 ,char c3,char c4)
{
    unsigned char a1,a2,a3,a4;
    a1 = c1;
    a2 = c2;
    a3=c3;
    a4=c4;
    unsigned int d=a1;
    d<<=8;
    d|=a2;
    d<<=8;
    d|=a3;
    d<<=8;
    d|=a4;
    int m=d;
    return m;
}

short hex2short(char c1,char c2)
{
    unsigned char a1;
    unsigned char a2;
    unsigned short dis;
    a1=c1;
    a2 =c2;
    dis = a1;
    dis<<=8;
    dis= dis|a2;
    short m = dis;
    return m;
}
