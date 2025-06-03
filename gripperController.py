import Jetson.GPIO as GPIO

class GripperController:
        def __init__(self):
            self.txPin = 6
            self.rxPin = 5

            GPIO.setmode(GPIO.BOARD)
            GPIO.setup(self.txPin, GPIO.OUT)
            GPIO.setup(self.rxPin, GPIO.IN)

            GPIO.output(self.txPin, GPIO.LOW)

        def commandOpen(self):
            GPIO.output(self.txPin, GPIO.LOW)
                
        def commandClose(self):
            GPIO.output(self.txPin, GPIO.HIGH)
                
        def didOpen(self):
            return not GPIO.input(self.rxPin)
            

        def didClose(self):
            return GPIO.input(self.rxPin)

        def cleanup(self):
            GPIO.cleanup()