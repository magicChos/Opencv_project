from flask import Response , request , Flask
from algorithm.yolodetectPerson import yolodetectPerson
from algorithm.hogdetectPerson import hogdetectPerson
from utils.log import get_logger
from utils.http_tool import decode_image
import json
import traceback
import time

app = Flask(__name__)
logger = get_logger('server.log')

@app.route('/hello' , methods=['GET', 'POST'])
def hello():
    logger.info(f"hello test!")
    return Response(json.dumps({'msg': 'success', 'status': 0, 'data': {}}), mimetype='application/json', status=200)

@app.route('/person/detect/detectPerson' , methods = ['GET' , 'POST'])
def detect_Person():
    logger.info(f"/person/detect/detectPerson---get request")
    try:
        img = decode_image(request.data)
        start_time = time.time()
        results = yolodetectPerson(img , 0.3)
        # results = hogdetectPerson(img)
        print('forward cost time: {}'.format(time.time() - start_time))
        return Response(json.dumps({'msg' : 'success' , 'status' : 0 , 'data' : results}) , mimetype='application/json' , status=200)
    except BaseException as e:
        logger.error(f"/person/detect/detectPerson---occur exception: {e}")
        logger.error(traceback.format_exc())
        return Response(json.dumps({'msg': f'exception: {e}', 'status': -1024, 'data': None}), mimetype='application/json', status=500)

if __name__ == "__main__":
    app.run(host='0.0.0.0' , port = 8888 , threaded=True)