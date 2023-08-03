import numpy as np
import matplotlib.pyplot as plt
def s_rrc(syms, beta, P, t_off):
    half = int(syms*P/2)
    k = np.arange(-half+1e-8+t_off, half+1e-8+t_off+1)
    s=4*beta/np.sqrt(P)*(np.cos((1+beta)*np.pi*k/P)+np.sin((1-beta)*np.pi*k/P)/(4*beta*k/P))/(np.pi*(1-16*(beta*k/P) * (beta*k/P)))
    return s

def PlotInFreq(s, fs, name):
    S = np.fft.fftshift(np.fft.fft(s))
    f = np.arange(-fs/2, fs/2, fs/len(s))
    figure, axis = plt.subplots(3, 1)
    axis[0].plot(f, np.abs(S))
    axis[1].plot(f, np.angle(S))
    axis[0].set_title(name + " " + "mag")
    axis[1].set_title(name + " " + "phase")
    axis[2].plot(s)
    axis[2].set_title(name + " " + "time")