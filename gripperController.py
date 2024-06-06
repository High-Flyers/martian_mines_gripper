import Jetson.GPIO as GPIO

class GripperController:
        def __init__(self):
            self.cmdPin = 11
            self.ackPin = 15

            GPIO.setmode(GPIO.BOARD)
            GPIO.setup(self.cmdPin, GPIO.OUT)
            GPIO.setup(self.ackPin, GPIO.IN)

            GPIO.output(self.cmdPin, GPIO.LOW)

        def commandOpen(self):
            GPIO.output(self.cmdPin, GPIO.LOW)
                
        def commandClose(self):
            GPIO.output(self.cmdPin, GPIO.HIGH)
                
        def didOpen(self):
            return not GPIO.input(self.ackPin)
            

        def didClose(self):
            return GPIO.input(self.ackPin)

        def cleanup(self):
            GPIO.cleanup()