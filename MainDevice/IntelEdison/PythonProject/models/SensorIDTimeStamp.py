from uuid import getnode
from datetime import datetime
import requests
import json
import time
import threading


class SensorIDTimeStamp(threading.Thread):    
    def __init__(self):
        super(SensorIDTimeStamp, self).__init__()
        self.server_id_url = "https://mcpsapi.azurewebsites.net/mcpsapi/Parametro"
        self.obj_temp_name = "obj_temp_" + str(getnode())
        self.amb_temp_name = "amb_temp_" + str(getnode())
        self.obj_temp_id = -1
        self.amb_temp_id = -1
        self.server_timestamp = time.time()
        self.last_time = time.time()
        self._getServerID()
        self.bRunning = True
        self.start()

    def calculateServerTimeStamp(self):
        diff_time = time.time() - self.last_time
        return self.server_timestamp + diff_time

    def close(self):
        self.bRunning = False

    def _getServerID(self):
        r = self._getServerTimeStamp(self.obj_temp_name)
        if r and 'SensorID' in r:
            self.obj_temp_id = int(r['SensorID'])
        r = self._getServerTimeStamp(self.amb_temp_name)
        if r and 'SensorID' in r:
            self.amb_temp_id = int(r['SensorID'])

    def _getServerTimeStamp(self, sensor_name):
        try:
            response = requests.get(self.server_id_url, params={'codigoSensor': sensor_name})
            r = json.loads(response.content)
            if 'TimeStamp' in r:
                self.server_timestamp = float(r['TimeStamp'])
                self.last_time = time.time()
            return r
        except Exception as ex:
            print(ex)
            return dict()

    def run(self):
        while self.bRunning:
            self._getServerTimeStamp(self.obj_temp_name)
            for i in range(60):
                if self.obj_temp_id < 0:
                    self._getServerID()
                time.sleep(10)
                if not self.bRunning:
                    break

