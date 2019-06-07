from ServoMotors2D import ServoMotors2D
from MLX90614Sensor import MLX90614Sensor
import numpy as np
import time


class TemperatureMeter:
    def __init__(self):
        self.motor = ServoMotors2D()
        self.sensor = MLX90614Sensor()
        self._locateObject()
        for i in range(20):
            time.sleep(1)
            print(self.sensor.readObjectTemperature())

    def _locateObject(self):
        while True:
            angle_step = 1
            abs_hor_min = 30
            abs_hor_max = 90
            abs_vert_min = 20
            abs_vert_max = 60
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
            print(max_point, max_value)
            time.sleep(1)
            m_value = self.sensor.readObjectTemperature()
            if abs(m_value - max_value) / max_value < 0.05:
                break
            

