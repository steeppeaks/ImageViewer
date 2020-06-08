# ImageViewer
简单的基于qt+opencv的图片浏览器

![预览](doc/preview.png)

1. 项目构建基于Qt Creator，依赖OpenCV-Qt版
2. 支持拖入文件，支持图片通过此程序打开
3. 目前仅支持以下格式：```*.jpg;*.jpeg;*.bmp;*.png```
4. 由于使用qt(MinGW)+opencv构建，且对```cv::Mat```对象没有优化，所以占用内存较大
