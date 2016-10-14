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
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

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
                     image.cols*image.channels(), QImage::Format_Grayscale8);
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



void Hello::getDoorFrame(cv::Mat &biImg, std::vector<cv::Vec4i> &lines)
{
    cv::HoughLinesP(biImg,lines,1, CV_PI/180, 30, 30, 15);
    vector<cv::Vec4i>::iterator it = lines.begin();

    while(it!=lines.end()) {
        float distance = sqrt(((*it)[1] - (*it)[3])*((*it)[1] - (*it)[3]) +
                ((*it)[0] - (*it)[2])*((*it)[0] - (*it)[2]));
        if ((*it)[0] == (*it)[2]) {
            it++;
            continue;

        }

        float angle = atanf(fabs(((*it)[1] - (*it)[3])/(float)((*it)[0] - (*it)[2])))*2/CV_PI;

        if((angle >0.02)&&(angle<0.8)) {
            lines.erase(it);
            continue;
        }

        if(angle>0.79) {
            if(distance<300) {
                lines.erase(it);
                continue;
            }
        }
        if(angle<0.16) {
            if((*it)[1]>150) {
                lines.erase(it);
                continue;
            }

        }
        it++;


    }
}


void Hello::showPic()
{
    if(fileList.isEmpty()) {
        return;
    }
    std::string path = (dir+"/"+fileList.at(fileIndex)).toStdString();

    cv::Mat img,newImg,biImg;
    img = cv::imread(path.c_str());

    undistortImage(img,newImg);

    cv::cvtColor(newImg,biImg,COLOR_BGR2GRAY);
    blur( biImg, biImg, Size(3,3) );
    cv::Canny(biImg,biImg,100,200);


    std::vector<cv::Vec4i> lines;

    getDoorFrame(biImg, lines);
    qDebug()<<lines.size();


    for( size_t i = 0; i < lines.size(); i++ )
    {
        line(newImg, Point(lines[i][0], lines[i][1]),
                Point(lines[i][2], lines[i][3]), Scalar(0,255,0), 2);
    }


    showPicInLabel(newImg, ui->originPic);
    showPicInLabel(biImg, ui->processedImg);

    fileIndex = fileIndex +1;
    if(fileIndex == fileList.length()) {
        fileIndex =0;
    }
}

void Hello::play()
{
    state = "inProgress";
    ui->control->setText(tr("停止播放"));
    pTimer->start(10);
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

void Hello::undistortImage(const Mat &img, Mat &undistortImg)
{
    float intrinsic[3][3] = {{431.6688,0,275.6883},{0,432.6680,236.1946},{0,0,1}};
    float distortion[1][5] = {{-0.3873, 0.2025, 0,0,-0.0655}};
    Mat intrinsic_matrix = Mat(3,3,CV_32FC1,intrinsic);
    Mat distortion_coeffs = Mat(1,5,CV_32FC1,distortion);

    cv::undistort(img,undistortImg,intrinsic_matrix,distortion_coeffs);

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

