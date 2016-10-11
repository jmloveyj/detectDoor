#ifndef HELLO_H
#define HELLO_H

#include <QMainWindow>

namespace Ui {
class Hello;
}

class QTimer;

class QLabel;

namespace cv {
class Mat;
}


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

    void showPicInLabel(cv::Mat &img, QLabel *frame);

    void updateFileList(const QString &newDir);
};

#endif // HELLO_H
