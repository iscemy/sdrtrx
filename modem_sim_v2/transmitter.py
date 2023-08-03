from common import *
import numpy as np

class BPSKTransmitter:
    def __init__(self, fs, f0, srrc_nsym, srrc_beta, srrc_osample, data_osample) -> None:
        self.fs = fs
        self.f0 = f0
        self.srrc_nsym = srrc_nsym
        self.srrc_beta = srrc_beta
        self.srrc_osample = srrc_osample
        self.data_osample = data_osample
        

    def oversample(self, bits, oversample_per_bit, oversample_val = 0):
        data_oversampled = np.array([])
        for bit in bits:
            pulse = np.zeros(oversample_per_bit)
            pulse[0] = bit*2-1 
            data_oversampled = np.concatenate((data_oversampled, pulse))    
        return data_oversampled

    def upconvert(self, input, phase = 0):
        t = np.arange(len(input)) / self.fs
        carrier = np.cos(2 * np.pi * self.f0 * t + np.pi/180 * phase)
        
        return carrier * input

    def tx(self, bits):
        oversampled_bits = self.oversample(bits, self.data_osample, 0)
        shaper = s_rrc(self.srrc_nsym, self.srrc_beta, self.srrc_osample, 0)
        shaped = np.convolve(shaper, oversampled_bits)
        PlotInFreq(shaped, self.fs , "")
        # return shaped
        return self.upconvert(shaped)
