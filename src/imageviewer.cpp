#include "imageviewer.h"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QDateTime>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QScrollArea>
#include <QMimeData>
//debug
//#include <QDebug>

ImageViewer::ImageViewer(QWidget *parent)
    : QMainWindow(parent)
{
    initUI();
    if(!qApp->arguments().value(1).isEmpty()){
        readFile(qApp->arguments().value(1));
    }
}


ImageViewer::~ImageViewer()
{
}

void ImageViewer::initUI(){
    initToolBar();
    initStatusBar();

    imgArea = new QLabel();
    imgArea->setAlignment(Qt::AlignCenter);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imgArea);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->addWidget(scrollArea);
    contentLayout->setMargin(0);

    QWidget *window = new QWidget;
    window->setLayout(contentLayout);

    setAcceptDrops(true);//允许窗口外部拖入
    setCentralWidget(window);
    setContextMenuPolicy(Qt::NoContextMenu);//禁止工具栏右键菜单
    setWindowTitle(tr("图片浏览器"));
    resize(1200, 800);
    show();
}

void ImageViewer::initToolBar(){
    QToolBar *fileToolBar = addToolBar(tr("工具栏"));
    fileToolBar->setMovable(false);//禁止工具栏拖动

    const QIcon openImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/OpenImage.png"));
    QAction *openImageAct = new QAction(openImageIcon, tr("&选择图片"), this);
    openImageAct->setShortcuts(QKeySequence::New);
    connect(openImageAct, &QAction::triggered, this, &ImageViewer::on_selectImageFile_clicked);
    fileToolBar->addAction(openImageAct);

    const QIcon openFolderIcon = QIcon::fromTheme("document-new", QIcon(":/images/OpenFolder.png"));
    QAction *openFolderAct = new QAction(openFolderIcon, tr("&选择目录"), this);
    openFolderAct->setShortcuts(QKeySequence::New);
    connect(openFolderAct, &QAction::triggered, this, &ImageViewer::on_selectHomeDir_clicked);
    fileToolBar->addAction(openFolderAct);

    fileToolBar->addSeparator();

    const QIcon prevImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Back.png"));
    QAction *prevImageAct = new QAction(prevImageIcon, tr("&上一张"), this);
    prevImageAct->setShortcuts(QKeySequence::New);
    connect(prevImageAct, &QAction::triggered, this, &ImageViewer::on_readPrevImage_clicked);
    fileToolBar->addAction(prevImageAct);


    const QIcon nextImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Next.png"));
    QAction *nextImageAct = new QAction(nextImageIcon, tr("&下一张"), this);
    nextImageAct->setShortcuts(QKeySequence::New);
    connect(nextImageAct, &QAction::triggered, this, &ImageViewer::on_readNextImage_clicked);
    fileToolBar->addAction(nextImageAct);

    fileToolBar->addSeparator();

    const QIcon refreshImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Refresh.png"));
    QAction * refreshImageAct = new QAction(refreshImageIcon, tr("&刷新"), this);
    refreshImageAct->setShortcuts(QKeySequence::New);
    connect(refreshImageAct, &QAction::triggered, this, &ImageViewer::on_refreshImage_clicked);
    fileToolBar->addAction(refreshImageAct);

    fileToolBar->addSeparator();

    const QIcon rotateImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Rotate.png"));
    QAction * rotateImageAct = new QAction(rotateImageIcon, tr("&旋转"), this);
    rotateImageAct->setShortcuts(QKeySequence::New);
    connect(rotateImageAct, &QAction::triggered, this, &ImageViewer::on_rotateImage_clicked);
    fileToolBar->addAction(rotateImageAct);

    const QIcon flipImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Flip.png"));
    QAction * flipImageAct = new QAction(flipImageIcon, tr("&镜像"), this);
    flipImageAct->setShortcuts(QKeySequence::New);
    connect(flipImageAct, &QAction::triggered, this, &ImageViewer::on_flipImage_clicked);
    fileToolBar->addAction(flipImageAct);

    fileToolBar->addSeparator();

    const QIcon extendImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Extend.png"));
    QAction * extendImageAct = new QAction(extendImageIcon, tr("&原始尺寸"), this);
    extendImageAct->setShortcuts(QKeySequence::New);
    connect(extendImageAct, &QAction::triggered, this, &ImageViewer::on_extendImage_clicked);
    fileToolBar->addAction(extendImageAct);

    const QIcon zoomInImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/ZoomIn.png"));
    QAction * zoomInImageAct = new QAction(zoomInImageIcon, tr("&放大"), this);
    zoomInImageAct->setShortcuts(QKeySequence::New);
    connect(zoomInImageAct, &QAction::triggered, this, &ImageViewer::on_zoomInImage_clicked);
    fileToolBar->addAction(zoomInImageAct);

    const QIcon zoomOutImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/ZoomOut.png"));
    QAction * zoomOutImageAct = new QAction(zoomOutImageIcon, tr("&缩小"), this);
    zoomOutImageAct->setShortcuts(QKeySequence::New);
    connect(zoomOutImageAct, &QAction::triggered, this, &ImageViewer::on_zoomOutImage_clicked);
    fileToolBar->addAction(zoomOutImageAct);
}

void ImageViewer::initStatusBar(){
    fileIndexLabel = new QLabel();
    filePathLabel = new QLabel();
    fileSizeLabel = new QLabel();
    imageScaleLabel = new QLabel();
    imageSizeLabel = new QLabel();
    fileModDateLabel = new QLabel();

    statusBar()->addWidget(fileIndexLabel,0);
    statusBar()->addWidget(filePathLabel,1);
    statusBar()->addWidget(fileSizeLabel,0);
    statusBar()->addWidget(imageSizeLabel,0);
    statusBar()->addWidget(imageSizeLabel,0);
    statusBar()->addWidget(fileModDateLabel,0);
}


void ImageViewer::readFile(QString filePath){
    if(filePath != ""){
        fileInfo = QFileInfo(filePath);
        loadFile();
        loadImage("normal");
    }
}

void ImageViewer::readDir(QString dirPath){
    if(dirPath != ""){
        QDir dir(dirPath);
        if(!dir.exists())
            return;

        //查看路径中后缀为.cfg格式的文件
        QStringList nameFilters;
        nameFilters<< "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";

        files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
        fileInfo = QFileInfo(dirPath+"/"+files.at(0));
        loadFile();
        loadImage("normal");
    }
}

void ImageViewer::loadFile(){
    current_index = 0;

    QStringList nameFilters;
    nameFilters<< "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";
    files = fileInfo.absoluteDir().entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

    for(int i = 0; i< files.size();++i) {
        QString tmp = files.at(i);
        if(tmp == fileInfo.fileName()){
            current_index = i;
        }
    }

    fileIndexLabel->setText(QString::number(current_index + 1)+"/"+QString::number(files.size()));
    filePathLabel->setText(fileInfo.absoluteFilePath());
    fileModDateLabel->setText(fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    double fileSize = fileInfo.size();
    QString fileSizeStr;
    if(fileSize / 1024 < 1){
        fileSizeStr = QString::number(fileSize)+" Btyes";
    } else if(fileSize / (1024*1024) < 1){
        fileSize = ( (float)( (int)( (fileSize/1024+0.005)*100 ) ) )/100;
        fileSizeStr = QString::number(fileSize)+" KB";
    } else if(fileSize / (1024*1024*1024) < 1){
        fileSize = ( (float)( (int)( (fileSize/1024/1024+0.005)*100 ) ) )/100;
        fileSizeStr = QString::number(fileSize)+" MB";
    } else {
        fileSizeStr = QString::number(fileSize)+" Btyes";
    }
    fileSizeLabel->setText(fileSizeStr);

    originImage = cv::imread(fileInfo.absoluteFilePath().toLocal8Bit().toStdString()
                             ,cv::IMREAD_UNCHANGED);
}

void ImageViewer::loadImage(QString loadType){
    if(!originImage.empty()){
        imgSize = QSize(originImage.cols, originImage.rows);
        imageSizeLabel->setText(QString::number(originImage.cols)
                                +"x"
                                +QString::number(originImage.rows));
        if(loadType == "normal"){
            computeScaleWithView();
            displayImage(resizeImage(originImage));
        } else if(loadType == "flip"){
            flipImage(originImage).copyTo(originImage);
            computeScaleWithView();
            displayImage(resizeImage(originImage));
        } else if(loadType == "rotate"){
            rotateImage(originImage).copyTo(originImage);
            computeScaleWithView();
            displayImage(resizeImage(originImage));
        } else if(loadType == "zoomIn"){
            zoomInImage();
            displayImage(resizeImage(originImage));
        } else if(loadType == "zoomOut"){
            zoomOutImage();
            displayImage(resizeImage(originImage));
        } else if(loadType == "extend"){
            extendImage();
            displayImage(resizeImage(originImage));
        }

        imageSizeLabel->setText(QString::number(( (float)( (int)( (scaleVar+0.005)*100 ) )))+" %");
    }
}
void ImageViewer::computeScaleWithView(){
    double labelW = scrollArea->size().width()-2;
    double labelH = scrollArea->size().height()-2;
    if(originImage.cols > labelW
            || originImage.rows > labelH){
        double imgW = originImage.cols;
        double imgH = originImage.rows;

        if(imgW > labelW && imgH < labelH){
            scaleVar = imgW/labelW;
        }else if(imgW < labelW && imgH > labelH){
            scaleVar = imgH/labelH;
        }else {
            double scaleVarW = imgW/labelW;
            double scaleVarH = imgH/labelH;
            if(scaleVarW > scaleVarH){
                scaleVar = scaleVarW;
            }else{
                scaleVar = scaleVarH;
            }
        }
        scaleVar = 1 / scaleVar;
    } else {
        scaleVar = 1;
    }
}

void ImageViewer::displayImage(cv::Mat image){
    cv::Mat Rgb;
    QImage Img;
    //    qDebug()<<"image.channels() ="<< image.channels();
    if (image.channels() == 3) {//RGB Img
        cv::cvtColor(image, Rgb, cv::COLOR_BGR2RGB);//颜色空间转换
        Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
        //        qDebug()<<"Img ="<< Img;
    } else if(image.channels() == 4){
        cv::cvtColor(image, Rgb, cv::COLOR_BGRA2RGBA);//颜色空间转换
        Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGBA8888);
    } else {//Gray Img
        Img = QImage((const uchar*)(image.data), image.cols, image.rows, image.cols*image.channels(), QImage::Format_Indexed8);
    }
    imgArea->setPixmap(QPixmap::fromImage(Img));
    imgArea->adjustSize();
}
/**************************************
 * 图片处理函数
 *************************************/
cv::Mat ImageViewer::resizeImage(cv::Mat orgImage){
    cv::Mat image;
    int width = floor(orgImage.cols * scaleVar);
    int height = floor(orgImage.rows * scaleVar);
    cv::resize(orgImage,image,
               cv::Size(width, height),
               cv::INTER_CUBIC);
    return image;
}

cv::Mat ImageViewer::rotateImage(cv::Mat orgImage){
    cv::Mat dstImage;
    //转置阵图片
    cv::transpose(orgImage,dstImage);
    cv::flip(dstImage,dstImage,1);
    return dstImage;
}

cv::Mat ImageViewer::flipImage(cv::Mat orgImage){
    cv::Mat dstImage;
    cv::flip(orgImage,dstImage,1);
    return dstImage;
}

void ImageViewer::zoomInImage(){
    if(scaleVar <= 1.75){
        scaleVar = scaleVar+0.25;
    }else{
        scaleVar = 2;
    }
}

void ImageViewer::zoomOutImage(){
    if(scaleVar >= 0.35){
        scaleVar = scaleVar-0.25;
    }else{
        scaleVar = 0.1;
    }
}
void ImageViewer::extendImage(){
    scaleVar = 1;
}
/**************************************
 * 按钮事件
**************************************/
void ImageViewer::keyPressEvent(QKeyEvent *event){
    if(event->modifiers() == Qt::NoModifier){
        if(event->key() == Qt::Key_Left){
            on_readPrevImage_clicked();
        }else if(event->key() == Qt::Key_Right){
            on_readNextImage_clicked();
        }
    }
}

void ImageViewer::dragEnterEvent(QDragEnterEvent *event){
    qDebug()<<"dragEnterEvent = "<<event->mimeData()->urls()[0].toLocalFile();
    if(!event->mimeData()->urls()[0].toLocalFile().right(3).compare("jpg",Qt::CaseInsensitive)
            ||!event->mimeData()->urls()[0].toLocalFile().right(3).compare("png",Qt::CaseInsensitive)
            ||!event->mimeData()->urls()[0].toLocalFile().right(3).compare("bmp",Qt::CaseInsensitive)
            ||!event->mimeData()->urls()[0].toLocalFile().right(4).compare("jpeg",Qt::CaseInsensitive)){
        event->acceptProposedAction();//接受鼠标拖入事件
    } else {
        event->ignore();//否则不接受鼠标事件
    }
}

void ImageViewer::dropEvent(QDropEvent *event){
    readFile(event->mimeData()->urls()[0].toLocalFile());
}

void ImageViewer::on_selectHomeDir_clicked(){
    QString selectDir;
    selectDir = QFileDialog::getExistingDirectory(this,
                                                  "选择图片目录",
                                                  "",
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if( selectDir != "" ) {
        readDir(selectDir);
    }
}

void ImageViewer::on_selectImageFile_clicked(){
    QString selectFile;
    QFileInfo fi;
    selectFile = QFileDialog::getOpenFileName(this,
                                              "选择图片",
                                              "*.jpg;*.jpeg;*.bmp;*.png");
    if(selectFile != ""){
        readFile(selectFile);
    }
}

void ImageViewer::on_readPrevImage_clicked(){
    if(current_index > 0){
        current_index -= 1;
    }else if(current_index == 0){
        current_index = 0;
    }else{
        return;
    }
    readFile(fileInfo.absolutePath()+"/"+files.at(current_index));
}

void ImageViewer::on_readNextImage_clicked(){
    if(current_index < files.size() - 1){
        current_index += 1;
    }else if(current_index == files.size() - 1){
        current_index = files.size() - 1;
    }else{
        return;
    }
    readFile(fileInfo.absolutePath()+"/"+files.at(current_index));
}

void ImageViewer::on_refreshImage_clicked(){
    readFile(fileInfo.absoluteFilePath());
}
void ImageViewer::on_rotateImage_clicked(){
    loadImage("rotate");
}
void ImageViewer::on_flipImage_clicked(){
    loadImage("flip");
}
void ImageViewer::on_zoomInImage_clicked(){
    if(scaleVar < 2){
        loadImage("zoomIn");
    }
}
void ImageViewer::on_zoomOutImage_clicked(){
    if(scaleVar > 0.1){
        loadImage("zoomOut");
    }
}
void ImageViewer::on_extendImage_clicked(){
    if(scaleVar != 1){
        loadImage("extend");
    }
}
