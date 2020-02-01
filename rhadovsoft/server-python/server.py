# ============================================================================
# Name:         server.py
# Part of:      RemoteCar project
# Description:  opens a socket and listends for commands
# Version:      1.0 (absolute hell)
# Author(s):    rhadov
# License:      No License
#============================================================================

#!/usr/bin/env python2.7 because wiringpi sucks
import socket
import re
import time
#import os # only for debugging
import wiringpi

while True:
    HOST = '192.168.1.102'
    PORT = 1337
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    wiringpi.wiringPiSetupGpio()

    #possible error exception
    try:
        s.bind((HOST, PORT))
    except socket.error:
        print 'bind failed!'
    s.listen(1)
    print 'socket is listening'
    (conn, addr) = s.accept()
    print 'connected!'
    break

while True:

    data = conn.recv(1024)
    if ' ' in data:
        values = re.findall(r"[\w']+", data)

        values = list(map(int, values))
        # values = list(map(int, data.split(','))) not functional
        # pins will use the physical addresses
        # forwards
        if values[0] == 1:
            wiringpi.digitalWrite(24, 0)
            wiringpi.digitalWrite(23, 1)
            wiringpi.pwmWrite(13, values[1])
        # reverse
        elif values[0] == 2:
            wiringpi.digitalWrite(24, 1)
            wiringpi.digitalWrite(23, 0)
            wiringpi.pwmWrite(13, values[1])
        else:
            wiringpi.digitalWrite(24, 0)
            wiringpi.digitalWrite(23, 0)

        # turn
        wiringpi.pwmWrite(18, values[2])
        time.sleep(0.02)