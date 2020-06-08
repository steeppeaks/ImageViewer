#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QFileInfo>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QTreeView>
#include <opencv2/opencv.hpp>

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();


private slots:
    void on_selectHomeDir_clicked();
    void on_selectImageFile_clicked();
    void on_readPrevImage_clicked();
    void on_readNextImage_clicked();
    void on_refreshImage_clicked();
    void on_rotateImage_clicked();
    void on_flipImage_clicked();
    void on_zoomInImage_clicked();
    void on_zoomOutImage_clicked();
    void on_extendImage_clicked();

private:
    //界面对象
    QLabel *imgArea;//图片显示区域
    QScrollArea *scrollArea;//图片展示窗口
    QLabel *fileIndexLabel;//文件索引Label
    QLabel *filePathLabel;//文件路径Label
    QLabel *fileSizeLabel;//文件大小Label
    QLabel *fileModDateLabel;//文件大小Label
    QLabel *imageSizeLabel;//图片尺寸Label
    QLabel *imageScaleLabel;//图片缩放比Label
    //公共参数，文件当前态信息
    QFileInfo fileInfo;//读取的文件信息
    QStringList files;//文件所在目录下文件列表
    int current_index = 0;//文件在目录下文件列表中的位置
    cv::Mat originImage;//读取的图片对象
    QSize imgSize;//图片尺寸
    double scaleVar;//缩放比
    //界面初始化
    void initUI();//UI创建及初始化
    void initToolBar();//初始化工具栏
    void initStatusBar();//初始化状态栏
    //图片处理公共流程
    void readFile(QString filePath);//读取文件
    void readDir(QString dirPath);//读取目录

    void loadFile();
    void loadImage(QString loadType);
    void computeScaleWithView();
    void displayImage(cv::Mat orgImage);
    //图片转换方法
    cv::Mat resizeImage(cv::Mat orgImage);
    cv::Mat rotateImage(cv::Mat orgImage);
    cv::Mat flipImage(cv::Mat orgImage);
    void zoomInImage();
    void zoomOutImage();
    void extendImage();



    void keyPressEvent(QKeyEvent *event);//按键事件
    void dragEnterEvent(QDragEnterEvent *event);//拖动进入事件
    void dropEvent(QDropEvent *event);
};
#endif // IMAGEVIEWER_H
