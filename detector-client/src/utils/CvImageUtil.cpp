/**
 * Created by hachikuji on 2021/1/7.
 */
#include "CvImageUtil.h"

#include <QDebug>


cv::Mat CvImageUtil::loadCvImage(QString path) {

    path.replace("file:///", "");

    QImage image(path);
    if (image.isNull()) {
        qDebug() << "Failed to open picture";
        return cv::Mat(100, 100, CV_8UC1, cv::Scalar::all(0));
    }

    return qImage2CvMat(image);
}

cv::Mat CvImageUtil::qImage2CvMat(const QImage &image) {
    switch (image.format()) {
        // 8-bit, 4 channel
        case QImage::Format_ARGB32:
            break;
        case QImage::Format_ARGB32_Premultiplied: {
            cv::Mat mat(image.height(), image.width(),
                        CV_8UC4,
                        (void *) image.constBits(),
                        image.bytesPerLine());
            return mat.clone();
        }

            // 8-bit, 3 channel
        case QImage::Format_RGB32: {
            cv::Mat mat(image.height(), image.width(),
                        CV_8UC4,
                        (void *) image.constBits(),
                        image.bytesPerLine());

            // drop the all-white alpha channel
            cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
            return mat.clone();
        }
        case QImage::Format_RGB888: {
            QImage swapped = image.rgbSwapped();
            cv::Mat mat(swapped.height(), swapped.width(),
                        CV_8UC3,
                        (void *) image.constBits(),
                        image.bytesPerLine());
            return mat.clone();
        }

            // 8-bit, 1 channel
        case QImage::Format_Indexed8: {
            cv::Mat mat(image.height(), image.width(),
                        CV_8UC1,
                        (void *) image.constBits(),
                        image.bytesPerLine());
            return mat.clone();
        }

            // wrong
        default:
            qDebug() << "ERROR: QImage could not be converted to Mat.";
            break;
    }
    return cv::Mat();
}

QImage CvImageUtil::cvMat2QImage(const cv::Mat &src) {

    cv::Mat mat = src;
    switch (mat.type()) {
        // 8位4通道
        case CV_8UC4: {
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
            return image;
        }

            // 8位3通道
        case CV_8UC3: {
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return image.rgbSwapped();
        }

            // 8位单通道
        case CV_8UC1: {
            static QVector<QRgb> sColorTable;
            // only create our color table once
            if (sColorTable.isEmpty()) {
                for (int i = 0; i < 256; ++i)
                    sColorTable.push_back(qRgb(i, i, i));
            }
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
            image.setColorTable(sColorTable);
            return image;
        }

        default:
            qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
            break;
    }
    return QImage();
}