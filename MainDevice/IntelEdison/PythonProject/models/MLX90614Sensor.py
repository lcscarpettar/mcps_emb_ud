import mraa


class MLX90614Sensor:
    def __init__(self):
        self.amb_add = 0x06
        self.obj1_add = 0x07
        self.obj2_add = 0x08
        self.i2c = mraa.I2c(0)
        self.i2c.address(0x5A)

    def readObjectTemperature(self):
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
        

    
