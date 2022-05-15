import torch
import time
import numpy as np
import cv2
import base64
from flask import Flask, request, jsonify
from flask_cors import cross_origin
from utils.cv_predictor import COCODemo
from maskrcnn_benchmark.config import cfg

ALLOWED_EXTENSIONS = {'png', 'jpg', 'JPG', 'PNG', 'gif', 'GIF'}


def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS


config_file = "./configs/conf_final.yaml"
cfg.merge_from_file(config_file)
# cfg.merge_from_list(["MODEL.DEVICE", "cpu"])

detector = COCODemo(
    cfg,
    min_image_size=800,
    confidence_threshold=0.7,
)

app = Flask(__name__)


# a web interface for using maskrcnn_benchmark to predictor images
@app.route('/api/detect/pil', methods=['POST'])
@cross_origin(supports_credentials=True)
def predictor():
    # file = request.files['image']
    # if file is None:
    #     return jsonify({'msg': 'error'})
    # img = Image.open(file.stream)
    # if img is None:
    #     return jsonify({'msg': 'error'})

    data = base64.b64decode(str(request.form['image']))
    img = np.fromstring(data, np.uint8)
    img = cv2.imdecode(img, cv2.IMREAD_COLOR)

    start = time.time()

    predictions = detector.compute_prediction(img)
    top_predictions = detector.select_top_predictions(predictions)

    # get labels, boxes and scores
    labels = top_predictions.get_field("labels")
    scores = top_predictions.get_field("scores")
    boxes = top_predictions.bbox
    result = list()

    for box, label, score in zip(boxes, labels, scores):
        box_list = box.to(torch.int64)[:].tolist()
        box_list_str = []
        for b in box_list:
            box_list_str.append(str(b))
        result.append({'label': detector.CATEGORIES[label.to(torch.int64)],
                       'score': str(score.to(torch.float32).item())[:6],
                       'box': box_list_str})
    end = time.time()
    print('run on an image totally cost', end - start)
    return jsonify({'msg': 'success', 'data': result})


if __name__ == "__main__":
    app.run()
