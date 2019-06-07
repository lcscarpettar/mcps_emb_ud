import mraa
import time


class ServoMotors2D:
    def __init__(self):
        self.horizontal_pwm = mraa.Pwm(5)
        self.horizontal_pwm.period_ms(20)
        self.horizontal_pwm.enable(True)

        self.vertical_pwm = mraa.Pwm(6)
        self.vertical_pwm.period_ms(20)
        self.vertical_pwm.enable(True)
        
        self.last_x = 90
        self.last_y = 45
        self.placeServo(90, 45)

    def placeServo(self, x, y):
        if x > 180:
            x = 180
        if x < 0:
            x = 0
        if y > 70:
            y = 70
        if y < 0:
            y = 0
        firstServoDuty = float(x) / 180.0 * 0.2
        secondServoDuty = float(y) / 180.0 * 0.2
        self.horizontal_pwm.write(firstServoDuty)
        self.vertical_pwm.write(secondServoDuty)
        time.sleep(0.005 * max(abs(x - self.last_x), abs(y - self.last_y)))
        self.last_x = x
        self.last_y = y

