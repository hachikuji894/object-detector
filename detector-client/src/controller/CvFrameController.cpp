/**
 * Created by hachikuji on 2022/1/6.
 */

#include "service/CvBaseService.h"
#include "CvFrameController.h"

#include "service/DetectorService.h"

CvFrameController::CvFrameController(QObject *parent) :
        QObject(parent),
        cvFrameServiceList(new ObjectListModel()) {

    initializeService();

}

CvFrameController &CvFrameController::getInstance() {

    static CvFrameController cvFrameController;
    return cvFrameController;

}


void CvFrameController::initializeService() {
    /**
     * Adding service in this method
     * cvFrameServiceList->append(new ...)
     */
    cvFrameServiceList->append(new DetectorService());

}

QObject *CvFrameController::getServiceObjectByName(const QString &name) const {
    for (auto *serviceObj : cvFrameServiceList->getRawData()) {
        auto obj = qobject_cast<CvBaseService *>(serviceObj);
        if (obj->getName().toLower() == name.toLower())
            return obj;
    }
    return nullptr;
}

void CvFrameController::processImageByService(const cv::Mat &image) const {

    for (auto service : cvFrameServiceList->getRawData()) {
        auto obj = qobject_cast<CvBaseService *>(service);

        /**
         * CvMediaPlayer is always running. When isActive() is triggered,
         * service will process every frame from media.
         */

        if (obj->getIsActive()) {
            obj->processImage(image);
        }
    }

}
