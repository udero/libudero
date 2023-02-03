#!/usr/bin/env python

import sys, socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("192.168.1.11", 43000))
sf = s.makefile()

s.send('lock\n')
response = sf.readline().strip()
tokens = response.split(' ')
if (not tokens[0] == 'lock') or (not tokens[-1] == 'OK'):
    print('lock Command Error.')
    sys.exit(0)

s.send('get_angles\n')
response = sf.readline().strip()
tokens = response.split(' ')
if (not tokens[0] == 'get_angles') or (not tokens[-1] == 'OK'):
    print('get_angles Command Error.')
    sys.exit(0)
    
angles = [float(token) for token in tokens[1:-2]]
print('Angles are %s' % (angles))

angles[0] = 1.0
s.send("set_angles {0} {1} {2} {3} {4} {5}\n".format(*angles))
response = sf.readline().strip()
if (not tokens[0] == 'set_angles') or (not tokens[-1] == 'OK'):
    print('set_angles Command Error.')


s.send('disconnect\n')
