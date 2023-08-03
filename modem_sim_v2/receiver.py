from common import *
import numpy as np
from scipy import signal

class BPSKRecevier:
    def __init__(self, fs, f0, srrc_nsym, srrc_beta, srrc_osample, data_osample) -> None:
        self.fs = fs
        self.f0 = f0
        self.srrc_nsym = srrc_nsym
        self.srrc_beta = srrc_beta
        self.srrc_osample = srrc_osample
        self.data_osample = data_osample
        
        self.carrier_shiht = 1.001
        # self.f0 = self.f0 * self.carrier_shiht
        self.if_bandwith = 100e4

    def preprocess(self, input):
        f0 = self.f0 * 2.0
        fs = self.fs
        input = input * input
        h = signal.remez(21, [0, f0 - self.if_bandwith/2 - 200000 , f0 - self.if_bandwith/2, f0 + self.if_bandwith/2, f0 + self.if_bandwith/2 + 200000, 0.5 * fs], [0, 1, 0], fs=fs)
        return np.convolve(input, h, 'same')
        # PlotInFreq(h, self.fs, "")
        # PlotInFreq(input, self.fs, "")
    
    def pll(self, input):
        t1 = np.zeros(np.size(input))
        t2 = np.zeros(np.size(input))
        rc = np.zeros(np.size(input))
        t1[0] = 0
        t2[0] = 0
        mu1 = 0.03
        mu2 = 0.003
        for i in range(0, len(input) - 1):
            t1[i+1] = t1[i] - mu1 * input[i] * np.sin(4 * np.pi * self.f0 * (i) * 1/self.fs + 2*t1[i])
            t2[i+1] = t2[i] - mu2 * input[i] * np.sin(4 * np.pi * self.f0 * (i) * 1/self.fs + 2*t1[i] +2*t2[i])
            rc[i] = np.cos(2*np.pi*self.f0/self.fs*(i)+t1[i]+t2[i])
        figure, axis = plt.subplots(2, 1)
        axis[0].plot(t1)
        axis[1].plot(t2)
        axis[0].set_title("t1, t2")
        return rc

    def carrier_recover(self, input):
        rp = self.preprocess(input)
        carrier_rec = self.pll(rp)
        return carrier_rec
    
    def convert_to_baseband(self, input):
        carrier_rec = self.carrier_recover(input)
        f0 = self.f0
        fs = self.fs

        baseband = carrier_rec * input
        h = signal.remez(31, [0,  self.if_bandwith,  self.if_bandwith  + 100e3, 0.5 * fs], [1, 0], fs=fs)
        PlotInFreq(h, self.fs, "")
        baseband = np.convolve(h, baseband, "same")

        return baseband
    
    def Sampler(self):
        pass
        
    def rx(self, input):
        return self.convert_to_baseband(input)

