import numpy as np
import cv2
import base64
from flask import Flask, request, jsonify
from flask_cors import cross_origin
from PIL import Image

ALLOWED_EXTENSIONS = {'png', 'jpg', 'JPG', 'PNG', 'gif', 'GIF'}


def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS


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
    print(img.shape)

    return jsonify({'msg': 'success'})


@app.route('/api/detect', methods=['POST'])
@cross_origin(supports_credentials=True)
def pic_predictor():
    file = request.files['image']
    if file is None:
        return jsonify({'msg': 'error'})
    img = Image.open(file.stream)
    if img is None:
        return jsonify({'msg': 'error'})

    print(img.size)

    return jsonify({'msg': 'success'})


if __name__ == "__main__":
    app.run()
