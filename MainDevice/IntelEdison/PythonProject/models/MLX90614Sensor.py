import mraa
import time


class MLX90614Sensor:
    def __init__(self):
        self.i2c6_file = "/sys/devices/pci0000:00/0000:00:09.1/i2c_dw_sysnode/mode"
        self._setI2CSpeedStandard()
        self.amb_add = 0x06
        self.obj1_add = 0x07
        self.obj2_add = 0x08
        self.i2c = mraa.I2c(6)
        self.i2c.address(0x5A)

    def _setI2CSpeedStandard(self):
        with open(self.i2c6_file, 'w') as f:
            f.write("std")

    def readObjectTemperature(self):
        for i in range(25):
            time.sleep(0.005)
            init_temp = self._readObjectTemperature()
            time.sleep(0.005)
            second_temp = self._readObjectTemperature()
            if abs(init_temp - second_temp) / second_temp < 0.015:
                return (init_temp + second_temp) / 2.0
        return (init_temp + second_temp) / 2.0

    def _readObjectTemperature(self):
        obj1 = self._readTemp(self.obj1_add)
        obj2 = self._readTemp(self.obj1_add)
        return (obj1 + obj2) / 2.0

    def readAmbTemperature(self):
        return self._readTemp(self.amb_add)

    def _convertTemperature(self, measure):
        return (float(measure) * 0.02) - 273.15

    def _readTemp(self, address):
        try:
            return self._convertTemperature(self.i2c.readWordReg(address))
        except Exception as ex:
            return -1.0
        

    
