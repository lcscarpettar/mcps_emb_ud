from ServoMotors2D import ServoMotors2D
from MLX90614Sensor import MLX90614Sensor
import numpy as np
import time


class TemperatureMeter:
    def __init__(self):
        self.motor = ServoMotors2D()
        self.sensor = MLX90614Sensor()
        self.diff_percentage = 0.1
        self.diff_angle = 8
        self._locateObject()

    def getAmbientTemperature(self):
        return self.sensor.readAmbTemperature()

    def getObjectTemperature(self):
        m_value = self.sensor.readObjectTemperature()
        if time.time() - self.last_epoch > 120.0 or abs(m_value - self.last_value) / m_value > self.diff_percentage:
            m_last_value = self.last_value
            self._locateObject(self.max_point[0] - self.diff_angle, self.max_point[0] + self.diff_angle,
                               self.max_point[1] - self.diff_angle, self.max_point[1] + self.diff_angle)
            if abs(m_last_value - self.last_value) / m_last_value > self.diff_percentage:
                self._locateObject()
        self.last_epoch = time.time()
        self.last_value = self.sensor.readObjectTemperature()
        return self.last_value

    def _locateObject(self, abs_hor_min = 45, abs_hor_max = 80, abs_vert_min = 30, abs_vert_max = 60):
        while True:
            angle_step = 1
            hor_direction = True
            hor_min = abs_hor_min
            hor_max = abs_hor_max
            vert_min = abs_vert_min
            vert_max = abs_vert_max
            max_value = -1.0
            max_point = [0, 0]
            for y in range(vert_min, vert_max + angle_step, angle_step):
                if hor_direction:
                    hor_begining = hor_min
                    hor_end = hor_max + angle_step
                    hor_step = angle_step
                else:
                    hor_begining = hor_max
                    hor_end = hor_min - angle_step
                    hor_step = -angle_step
                for x in range(hor_begining, hor_end, hor_step):
                    self.motor.placeServo(x, y)
                    m_value = self.sensor.readObjectTemperature()
                    if m_value > max_value:
                        max_value = m_value
                        max_point = [x, y]
                hor_direction = not hor_direction
            self.motor.placeServo(max_point[0], max_point[1])
            time.sleep(0.5)
            m_value = self.sensor.readObjectTemperature()
            if abs(m_value - max_value) / max_value < self.diff_percentage:
                break
        self.max_point = max_point
        self.last_value = m_value
        self.last_epoch = time.time()
            

