//
// Created by hachikuji on 2022/5/13.
//

#ifndef DETECTOR_CLIENT_DETECTORSERVICE_H
#define DETECTOR_CLIENT_DETECTORSERVICE_H


#include <utility>

#include "CvBaseService.h"

class DetectorService : public CvBaseService {
Q_OBJECT
private:
    static QByteArray getImageData2(const QImage &image);

    struct Data {
        QList<int> box;
        QString label;
        float score;

        Data(QList<int> box, QString label, float score) {
            this->box = std::move(box);
            this->label = std::move(label);
            this->score = score;
        }
    };

public:
    explicit DetectorService();

    ~DetectorService() override = default;

    void processImage(const cv::Mat &image) override;

    static cv::Scalar computeColorsForLabels(const QString& label);

};

#endif //DETECTOR_CLIENT_DETECTORSERVICE_H
