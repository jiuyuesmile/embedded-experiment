#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTextBrowser>
#include <QThread>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    void SetCounttingBit(QTextBrowser *tb, int count);
    QString retReturn(int err_code);
    void saveText2Local(QString path, QString fileName, QString content);
    ~MainWindow();



private slots:
    void on_open_s_clicked();

    void on_send_data_clicked();

    void on_clear_sc_clicked();

    void on_clear_rc_clicked();

    void on_stop_display_clicked();

    void on_m_save_clicked();

    void on_clear_data_clicked();

    // 用来触发 读入的数据输入接收窗口
    void change(int nread, QString content);

private:
    Ui::MainWindow *ui;
};
class MyThread: public QThread
{
    Q_OBJECT
public:
    MyThread();
    void run();
signals:
    void send(int nread, QString content);
};
#endif // MAINWINDOW_H
