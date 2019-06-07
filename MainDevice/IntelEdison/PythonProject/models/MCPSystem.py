from TemperatureMeter import TemperatureMeter
from MeasurePublisher import MeasurePublisher
import time
import threading


class MCPSystem(threading.Thread):
    def __init__(self):
        super(MCPSystem, self).__init__()
        self.start()

    def run(self):
        self.publisher = MeasurePublisher()
        self.temp_meter = TemperatureMeter()
        self.bRunning = True
        while self.bRunning:
            self.publisher.addObjTemp(self.temp_meter.getObjectTemperature())
            self.publisher.addAmbTemp(self.temp_meter.getAmbientTemperature())
            self.publisher.publishMeasuares()
            time.sleep(30)

    def close(self):
        self.bRunning = False
        try:
            self.publisher.sensors_id_timestamp.close()
            self.publisher.sensors_id_timestamp.join(20)
        except Exception as ex:
            print(ex)

    def __del__(self):
        self.close()
