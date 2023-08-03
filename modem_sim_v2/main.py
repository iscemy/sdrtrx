from common import *
import matplotlib.pyplot as plt
from transmitter import *
from receiver import *

a = [0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1]
fs = 8000e3
tx = BPSKTransmitter(fs, 1500e3+1e4, 4, 0.15, 6, 6)
rx = BPSKRecevier(fs, 1500e3, 4, 0.15, 6, 6)

r = tx.tx(a)
baseband = rx.rx(r)



PlotInFreq(baseband, fs, "")
plt.show()