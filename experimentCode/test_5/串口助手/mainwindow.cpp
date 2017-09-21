#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <QTextBrowser>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#define NCC 50
/*********************************************************************************************************
** 函数名称: set_speed
** 功能描述: 设置串口波特率
** 输　入  : fd    串口操作句柄
** 输  入  : speed 串口设置的波特率
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
struct termio
{
     unsigned short	c_iflag;			/* 输入模式标志      	       */
     unsigned short	c_oflag;			/* 输出模式标志       	       */
     unsigned short	c_cflag;			/* 控制模式标志 	       */
     unsigned short	c_lflag;			/* local mode flags  	       */
     unsigned char	c_line;		    	/* line discipline      	       */
    unsigned char	c_cc[NCC];    		/* control characters	       */
};

int  set_speed (int  fd, int  speed)
{
    int            	ret;
    struct termios 	opt;

    ret = tcgetattr(fd, &opt);                         	            		/*获取串口的 termios 信息   */
    if (ret !=  0) {
         printf("Get Serial speed failed");
         return (10);
    }
   tcflush(fd, TCIOFLUSH);                             	               	/*  刷新输入输出数据           */

   cfsetispeed(&opt, speed);               	           	              	/*  设置输入波特率              */
   cfsetospeed(&opt, speed);               	            	            	/*  设置输出波特率              */
   /* 用于设置终端的相关参数, TCSANOW 不等数据传输完毕就立即改变属性	 */
   ret = tcsetattr(fd, TCSANOW, &opt);
   if (ret != 0) {
        printf("Setup Serial speed failed");
        return (11);
   }
   return (1);
}


/*********************************************************************************************************
** 函数名称: set_Parity
** 功能描述: 设置串口的通信参数
** 输　入  : fd        串口操作句柄
** 输  入  : databits  数据位
** 输  入  : stopbits  停止位
** 输  入  : parity    校验位
** 返回  值: 0 设置成功，-1 设置失败
** 全局变量:
** 调用模块:
*********************************************************************************************************/
int set_parity (int fd, int databits, int stopbits, int parity)
{
    int     			ret;
    struct termios 	options;

    ret = tcgetattr(fd, &options);
    if  (ret !=  0) {
        printf("Setup Serial failed");
        return(2);
    }

    options.c_cflag &= ~CSIZE;
    switch (databits) {                               		/*  设置数据位数           */
    case 5:
        options.c_cflag |= CS5;
        break;

    case 6:
        options.c_cflag |= CS6;
        break;

    case 7:
        options.c_cflag |= CS7;
        break;

    case 8:
        options.c_cflag |= CS8;
        break;

    default:
        printf("Unsupported data size\n");
        return (3);
    }

    switch (parity) {                                    	/*  设置校验方式           */
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB;
        options.c_iflag &= ~INPCK;
        break;

    case 'o':
    case 'O':
        options.c_cflag |= (PARODD | PARENB);
        options.c_iflag |= INPCK;
        break;

    case 'e':
    case 'E':
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;

    default:
        printf("Unsupported parity\n");
        return (4);
    }

    switch (stopbits) {	                                   	/* 设置停止位              */
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;

    case 2:
        options.c_cflag |= CSTOPB;
        break;

    default:
        printf("Unsupported stop bits\n");
        return (5);
    }
    tcflush(fd, TCIOFLUSH);                              	/* 清楚串口收发缓冲去      */
    /*
     * 用于设置终端的相关参数,TCSANOW：不等数据传输完毕就立即改变属性
     */
    ret = tcsetattr(fd, TCSANOW, &options);
    if (ret != 0) {
        printf("Setup Serial failed");
        return (2);
    }
    return (1);
}

// 打开设备并返回是否打开成功
int opendevice(const char* s_n, int b_d, int c_b, int s_b, int d_b, int *fd_tty, int *serial_status) {
    // 设备状态返回
    int       ret;
    *fd_tty = open(s_n, O_RDWR);                                     /*  打开串口                    */
    if (fd_tty < 0) {
        printf("open device %s faild\n", s_n);
        *serial_status = 0;
        return  9;
    }
    printf("ok?\n");
    ret = set_speed(*fd_tty, b_d);                                 /*  设置串口波特率              */
    printf("over close!%d", ret);
    if (ret != 1) {
        ::close(*fd_tty);
        *serial_status = 0;
        printf("gogoggo\nn");
        return  (ret);
    }
    ret = set_parity(*fd_tty, d_b, s_b, c_b);                             /*  数据位 8，停止位 1，无校验  */
    if (ret != 1) {
        ::close(*fd_tty);
        *serial_status = 0;
        return  (ret);
    }
    *serial_status = 1;

    return 1;
}



// 数据缓冲区最大缓存
#define MAX_LENGTH 1000
// 接口状态
int serial_status = 0;
// 接收数据个数
int send_count = 0;
// 发送数据的个数
int receiv_count = 0;
// 串口号
int s_n;
// 波特率
int b_r;
// 校验位
int c_b;
//停止位
int s_b;
// 发送缓冲区
int buff_send[MAX_LENGTH];
// 接收缓冲区
int buff_rcv[MAX_LENGTH];

// 读取信息的线程
MyThread *myth = NULL;

// 每次读取的个数
int       nread;
// 设备
int       fd_tty ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = 0;
    setWindowFlags(flags);
    setFixedWidth(this->width());
    setFixedHeight(this->height());
    serial_status = 0;
}

MainWindow::~MainWindow()
{
    // 关闭设备
    if (serial_status == 1) {
        serial_status = 0;
        if (myth != NULL) {

            myth->exit();
            delete myth;
            myth = NULL;
        }
        ::close(fd_tty);
    }
    delete ui;
}

void MainWindow::on_open_s_clicked()
{
    QString info;
    if (serial_status == 0) {
        // 获得设备
        const char *con_s_n_l = ui->serialnum->currentText().toAscii().constData();
        bool ok;
        int b_d_l = ui->boundrate->currentText().toInt(&ok, 10);
        int c_b_l = ui->chekbit->currentText().toLatin1().data()[0];
        int s_b_l = ui->stopbit->currentText().toInt(&ok, 10);
        printf("con_s_n_l:%s, b_d_l:%d, c_b_l:%c, s_b_l:%d\n", con_s_n_l, b_d_l, c_b_l, s_b_l);
        int ret = opendevice(con_s_n_l, b_d_l, c_b_l, s_b_l, 8, &fd_tty, &serial_status);

        if(ret == 1) {
            // 状态修改
            serial_status = 1;
            ui->s_s_t->setText("open");
            info = "Open Success";
            myth = new MyThread;
            connect(myth,SIGNAL(send(int, QString)),this,SLOT(change(int, QString)));
            myth->start();
            printf("start!\n");
        } else {
            printf("hihhiijiret_info:%d\n", ret);
            info = retReturn(ret);
            printf("do u know?\n");
            printf("%s\n", info.toAscii().constData());
        }
    } else {
        info = "is already open!";
    }

    QMessageBox::information(NULL, "Open Serial", info,
                             QMessageBox::Yes );
}


// 发送数据
void MainWindow::on_send_data_clicked()
{
    QString content = ui->data_send->toPlainText();
    const char *buff = content.toAscii().constData();
    if(serial_status == 1 && content != NULL && content.length() != 0) {
        //ui->data_recv->setText(content);
        write(fd_tty, buff, strlen(buff));
        send_count += strlen(buff);
        QString str = QString(QLatin1String(buff));
        SetCounttingBit(ui->s_b_t, send_count);
    }

}


// 修改计数器
void MainWindow::SetCounttingBit(QTextBrowser *tb, int count) {
    QString s = QString::number(count, 10)+ " B";
    tb->setText(s);
}

// 清空发送计数
void MainWindow::on_clear_sc_clicked()
{
    send_count = 0;
    SetCounttingBit(ui->s_b_t, 0);
}

// 清空接收计数
void MainWindow::on_clear_rc_clicked()
{
    receiv_count = 0;
    SetCounttingBit(ui->r_b_t, receiv_count);
}

// 停止显示，并关闭串口
void MainWindow::on_stop_display_clicked()
{
    if(serial_status == 1) {
        //::close(fd_tty);
        ui->s_s_t->setText("close");
        serial_status = 0;
        ::close(fd_tty);
        if (myth != NULL) {
            usleep(50*1000);
            myth->quit();

            delete myth;
            myth = NULL;
        }
    } else {
        printf("alreay close!");
    }

}
// 保存文件
void MainWindow::on_m_save_clicked()
{/*
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Open Document",
       QDir::currentPath(),
       "Document files (*.txt *.text);;All files(*.*)");*/
    QMessageBox message(QMessageBox::Warning,"Information",
                        "Really to Save Info?",
                        QMessageBox::Yes|QMessageBox::No,NULL);
        if (message.exec()==QMessageBox::Yes)
        {
            saveText2Local(NULL, "info.txt", ui->data_recv->toPlainText());
        }
        else
        {
            printf("nothing to save");
        }
}

// 清空接收显示
void MainWindow::on_clear_data_clicked()
{
    ui->data_recv->setText("");

}

//根据设备返回的错误码，返回错误信息
QString MainWindow::retReturn(int err_code) {
    QString res;
    switch (err_code) {
    case 2:
        res = "Setup Serial failed";
        break;
    case 3:
        "Unsupported data size";
        break;
    case 4:
        "Unsupported parity";
        break;
    case 5:
        res = "Unsupported stop bits";
        break;
    case 10:
        res = "Get Serial speed failed";
        break;
    case 11:
        res = "Set Serial speed failed";
        break;
    case 9:
        res = "open device faild";
        break;
    default:
        res = "uknow error";
        break;
    }
    return res;
}

// 修改接收窗口的内容
void MainWindow::change(int nread, QString content) {
    printf("%s", content.toAscii().constData());
    //ui->data_recv->setText(content);
    ui->data_recv->insertPlainText(content);
    ui->data_recv->moveCursor(QTextCursor::End);
    receiv_count += content.length();
    SetCounttingBit(ui->r_b_t, send_count);
}


MyThread::MyThread()
{

}
void MyThread::run() {
    fd_set  			fdset;
    printf("thread run?\n");
    printf("thread:serial_status:%d\n", serial_status);
    while (serial_status) {
        printf("is in?");
        //int ret = select(fd_tty + 1, &fdset, NULL, NULL, NULL);
        if (1 == 1) {
            char buff[255];
            nread = read(fd_tty, buff, 255);
            QString str = QString(QLatin1String(buff));
             /*
              * 向主线程发送线程信号，发送后延时0.5s，做消抖处理
              */
            emit send(nread, str);
            send_count += nread;
            usleep(200 * 1000);
        } else {
            break;
        }
    }
    if (serial_status == 1) {
        ::close(fd_tty);
        serial_status = 0;
    }
    exec();
}

void MainWindow::saveText2Local(QString path, QString fileName, QString content){
    QString full_path = NULL;
    if (path == NULL) {
        full_path = fileName;
    } else {
        full_path = path + "/" + fileName;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append)){
          QMessageBox::warning(this,"sdf","can't open",QMessageBox::Yes);
    }
    file.resize(0);
    QTextStream in(&file);

    in<<content;
    file.close();

}
