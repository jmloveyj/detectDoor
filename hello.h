#ifndef HELLO_H
#define HELLO_H

#include <QMainWindow>
#include <opencv2/core.hpp>

namespace Ui {
class Hello;
}

class QTimer;

class QLabel;


class Hello : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hello(QWidget *parent = 0);
    ~Hello();

public slots:
    void onSelectSrc();
    void onControl();
private slots:
    void showPic();


private:
    Ui::Hello *ui;
    QString state;
    QString dir;
    QStringList fileList;
    int fileIndex;
    QTimer * pTimer;


    void play();

    void stop();

    void undistortImage(const cv::Mat &img, cv::Mat &undistortImg);

    void showPicInLabel(cv::Mat &img, QLabel *frame);

    void updateFileList(const QString &newDir);
    void getDoorFrame(cv::Mat &biImg, std::vector<cv::Vec4i> &lines);
};

#endif // HELLO_H
