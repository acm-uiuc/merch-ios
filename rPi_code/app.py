from flask import Flask, request, abort, jsonify
import RPi.GPIO as GPIO
import time
import sys

CLOCK = 21
ROW = [26, 16, 20]
COL = [19, 13]

GPIO.setmode(GPIO.BCM)

app = Flask(__name__)

@app.route('/vend', methods=['POST'])
def hello_world():
    if 'item' not in request.args:
        abort(400)
    item = request.args['item']
    vend(item[0], item[1])
    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}

def setup():
    GPIO.setup(CLOCK, GPIO.OUT, initial=GPIO.LOW)
    for row in ROW:
        GPIO.setup(row, GPIO.OUT, initial=GPIO.LOW)
    for col in COL:
        GPIO.setup(col, GPIO.OUT, initial=GPIO.LOW)

def LOW():
    GPIO.output(CLOCK, GPIO.LOW)

    for row in ROW:
        GPIO.output(row, GPIO.LOW)
    for col in COL:
        GPIO.output(col, GPIO.LOW)

def commit():
    GPIO.output(CLOCK, GPIO.HIGH)
    time.sleep(0.5)
    GPIO.output(CLOCK, GPIO.LOW)
    LOW()

def vend(letter, number):
    # Set the letter
    binary = ord(letter) - ord('A')
    if(binary & 0b100):
        GPIO.output(ROW[0], GPIO.HIGH)
    if(binary & 0b010):
        GPIO.output(ROW[1], GPIO.HIGH)
    if(binary & 0b001):
        GPIO.output(ROW[2], GPIO.HIGH)

    GPIO.output(COL[0], GPIO.HIGH)
    GPIO.output(COL[1], GPIO.HIGH)

    commit()


    # Set the number
    row = (number-1) / 2
    col = (number+1) % 2+1

    if(col & 0b01):
        GPIO.output(COL[1], GPIO.HIGH)
    if(col & 0b10):
        GPIO.output(COL[0], GPIO.HIGH)

    if(row & 0x100):
        GPIO.output(ROW[0], GPIO.HIGH)
    if(row & 0b010):
        GPIO.output(ROW[1], GPIO.HIGH)
    if(row & 0b001):
        GPIO.output(ROW[2], GPIO.HIGH)

    commit()
    commit()

def reject(bad_position):
    print("%s is not a valid character" % bad_position)

if __name__ == '__main__':
    setup()
    LOW()
    commit()
    app.run(debug=True)