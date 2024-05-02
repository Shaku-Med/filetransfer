from flask import Flask, request, send_file
from flask_cors import CORS
import os
import uuid

app = Flask(__name__)
CORS(app)

UPLOAD_FOLDER = 'Files/'

def allowed_file(filename):
    return True

@app.route('/')
def index():
    return send_file('index.html')

@app.route('/style.css')
def style():
    return send_file('style.css')

@app.route('/api', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        return "No file part", 400
    file = request.files['file']
    if file.filename == '':
        return "No selected file", 400
    if file and allowed_file(file.filename):
        filename = str(uuid.uuid4()) + "_" + file.filename
        file.save(os.path.join(UPLOAD_FOLDER, filename))
        return {"success": True}
    else:
        return "File type not allowed", 400

if __name__ == '__main__':
    app.run(port=3001)
