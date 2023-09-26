import serial
import sys
import time
import numpy as np
import argparse
import pandas as pd

class BTServer():
    def __init__(self, path, samples, port):
        self.buff = []
        self.samples = []
        self.connect_init(port)
        self.fill_buffer(samples)
        self.save_file(path)
        self.close()
    
    def connect_init(self, port):
        self.ser = serial.Serial(port)  # open serial port
        #print(f"Listening on serial port: {self.ser.name}") # check which port was really used
        self.ser.write(b'1')
        
    def get_sample(self, ser):
        sHi = ser.read()
        sLo = ser.read()
        comb = sHi + sLo
        i = int.from_bytes(comb, byteorder='big', signed = True)
        return i
    
    def fill_buffer(self, samples):
        start = time.monotonic()
        print("Reading...")
        while(True):
            ints = self.get_sample(self.ser)
            self.buff.append(ints)
            if len(self.buff) > samples :
                break
        self.ser.write(b'1')
        dur = time.monotonic() - start
        print(f"Transaction of {samples} samples took {dur:.3f} seconds with sample rate {(samples/dur):.1f}")
        
    def save_file(self, path):
        save = np.asarray(self.buff)*3.3/4096
        df = pd.DataFrame(save)
        df.to_csv(path, index = True)
        #np.savetxt(path, np.asarray(self.buff))
        print(f"File saved to {path}")
    
    def close(self):
        self.ser.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("path")
    parser.add_argument("samples")
    #parser.add_argument("port")
    args = parser.parse_args()
    BTServer(args.path, int(args.samples), '/dev/rfcomm0')