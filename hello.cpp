#include "hello.h"
#include "ui_hello.h"
#include "QFileDialog"
#include "QDebug"
#include "QDir"
#include "QTimer"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QImage>

using namespace cv;

//将opencv 的mat 转换为可以显示的qt图片
static QImage Mat2QImage(Mat& image)
{
    QImage img;

    if (image.channels()==3) {
        cvtColor(image, image, CV_BGR2RGB);
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_RGB888);
    } else if (image.channels()==1) {
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_ARGB32);
    } else {
        img = QImage((const unsigned char *)(image.data), image.cols, image.rows,
                     image.cols*image.channels(), QImage::Format_RGB888);
    }

    return img;
}

Hello::Hello(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hello),
    state("stop")
{
    ui->setupUi(this);

    pTimer = new QTimer(this);

    this->connect(pTimer,SIGNAL(timeout()),this,SLOT(showPic()));
    this->connect(ui->control,SIGNAL(clicked()),this, SLOT(onControl()));
    this->connect(ui->selectSrc,SIGNAL(clicked()), this, SLOT(onSelectSrc()));

    updateFileList("/Users/jinming/Downloads/image/IMAGE20160823110936/pic");

}

Hello::~Hello()
{
    delete ui;
}

void Hello::onSelectSrc()
{
    stop();
    QString dirName = QFileDialog::getExistingDirectory(this, tr("打开照片目录"), "/Users/jinming/Downloads/image");
    qDebug() <<"打开的目录是:"<< dirName;
    updateFileList(dirName+"/pic");

}

void Hello::onControl()
{
    //    qDebug()<<"the state is:"<<state;
    if(state.compare("stop") == 0) {
        play();
    } else {
        stop();
    }
}

void Hello::showPic()
{
    if(fileList.isEmpty()) {
        return;
    }
    std::string path = (dir+"/"+fileList.at(fileIndex)).toStdString();

    cv::Mat img;
    img = cv::imread(path.c_str());

    //    qDebug()<<img.size().height<<path.c_str();


    showPicInLabel(img, ui->originPic);

    showPicInLabel(img, ui->processedImg);

    fileIndex = fileIndex +1;
    if(fileIndex == fileList.length()) {
        fileIndex =0;
    }
}

void Hello::play()
{
    state = "inProgress";
    ui->control->setText(tr("停止播放"));
    pTimer->start(30);
}

void Hello::stop()
{
    if (state == "stop") {
        return;
    }
    state = "stop";
    ui->control->setText(tr("开始播放"));

    if(pTimer->isActive()) {
        pTimer->stop();
    }
}

void Hello::showPicInLabel(Mat &img, QLabel *frame)
{
    QImage imgShow =  Mat2QImage(img);
    frame->setPixmap(QPixmap::fromImage(imgShow));

}

void Hello::updateFileList(const QString &newDir)
{
    stop();
    dir = newDir;
    QDir defaultDir(dir);
    defaultDir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList filters;
    filters<<"*.jpg";
    defaultDir.setNameFilters(filters);
    defaultDir.setSorting(QDir::Name);
    fileList = defaultDir.entryList();
    fileIndex = 0;
}

