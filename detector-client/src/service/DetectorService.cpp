//
// Created by hachikuji on 2022/5/13.
//

#include "DetectorService.h"
#include "utils/CvImageUtil.h"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonObject>
#include <QBuffer>
#include <QUrlQuery>
#include <QEventLoop>
#include <QTimer>
#include <QJsonParseError>
#include <QJsonArray>
#include <QLabel>

#include <algorithm>


DetectorService::DetectorService() : CvBaseService("DetectorService") {

    this->setIsActive(true);

}

//图片转 base64 字符串
QByteArray DetectorService::getImageData2(const QImage &image) {
    QByteArray imageData;
    QBuffer buffer(&imageData);
    image.save(&buffer, "jpg");
    imageData = imageData.toBase64().replace("+", "%2B");
    return imageData;
}


void DetectorService::processImage(const cv::Mat &image) {

    qDebug() << "Request a python server for the process of object detecting!";

    QImage src = CvImageUtil::cvMat2QImage(image);
    QByteArray imageData = getImageData2(src);
//    qDebug() << imageData;

//    auto *networkManager = new QNetworkAccessManager(this);
    QNetworkAccessManager networkManager;
    QNetworkRequest requestInfo;

    QString url = "http://localhost:5000/api/detect/pil";
    requestInfo.setUrl(QUrl(url));
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");

    QUrlQuery postData;
    postData.addQueryItem("image", imageData);
    QNetworkReply *reply = networkManager.post(requestInfo, postData.toString(QUrl::FullyEncoded).toUtf8());

    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    QTimer::singleShot(40000, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QByteArray result;

    if (!reply->isFinished()) {
        disconnect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        reply->abort();
        qDebug() << "The request of object detection is timeout";
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Connection is refused";
        return;
    }

    result = reply->readAll();
    QJsonParseError jsonError{};
    QJsonDocument document = QJsonDocument::fromJson(result, &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << "Json is error";
        return;
    }

    QList<Data> dataList;

    if (document.isObject()) {
        QJsonObject resultObj = document.object();
        QJsonValue data;
        if (resultObj.contains("data")) {
            data = resultObj.take("data");
            if (data.isArray()) {
                QJsonArray dataArray = data.toArray();
                for (auto d:dataArray) {
                    if (d.isObject()) {
                        QJsonObject obj = d.toObject();
                        QJsonValue box;
                        QList<int> boxList;
                        QJsonValue label;
                        QJsonValue score;
                        if (obj.contains("box")) {
                            box = obj.take("box");
                            if (box.isArray()) {
                                QJsonArray boxArray = box.toArray();
                                for (auto b:boxArray)
                                    if (b.isString())
                                        boxList.append(b.toString().toInt());
                            }
                        }
                        if (obj.contains("label"))
                            label = obj.take("label");
                        if (obj.contains("score"))
                            score = obj.take("score");
                        if (label.isString() && score.isString())
                            dataList.push_back(Data(boxList, label.toString(), score.toString().toFloat()));
                    }
                }
            }
        }
    }
    reply->deleteLater();

    for (auto data:dataList) {
        qDebug() << data.label << ":" << data.score
                 << "(" << data.box[0] << "," << data.box[1] << "," << data.box[2] << "," << data.box[3] << ")";

        cv::rectangle(image, cv::Point(data.box[0], data.box[1]), cv::Point(data.box[2], data.box[3]),
                      computeColorsForLabels(data.label), std::max(src.width() / 300, 1), cv::LINE_8, 0);

        cv::putText(image, data.label.toStdString() + ":" + cv::format("%.4f", data.score),
                    cv::Point(data.box[0], data.box[1]),
                    cv::FONT_HERSHEY_SIMPLEX, std::max((double) src.width() / 1500, 0.1), cv::Scalar(255, 255, 255));
    }
}

cv::Scalar DetectorService::computeColorsForLabels(const QString &label) {

    int key = 0;
    for (auto s :label)
        key += s.toLatin1() - 'a';
    int r = (key * 23) % 256;
    if (r < 80) r += 80;
    int g = (key * 13) % 256;
    if (g < 80) g += 80;
    int b = (key * 31) % 256;
    if (b < 80) b += 80;
    return cv::Scalar(r, g, b);
}
