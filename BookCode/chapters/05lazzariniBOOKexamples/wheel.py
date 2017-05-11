#!/usr/bin/python
# Aliasing demonstration
# plays an animation of a spinning wheel
# at different frequencies demonstrating
# aliasiang beyond sr/2 as in figs. 1 and 2
# from the Introduction to Digital Signals
# chapter
# (c) Victor Lazzarini, 2007

from Tkinter import *
from math import *

class Application(Frame):
  
    def createCanvas(self):
        self.size = 600
        self.canvas = Canvas(self,height=self.size,width=self.size,bg="violet")
        self.scale = Scale(self,orient="horizontal",showvalue=False, label = "frequency (0 - SR)", length = 300)
        self.canvas.pack()
        self.scale.pack()

    def rotate(self):
        midx = self.size/2
        midy = self.size/2+50
        rad = self.size/3
        self.wheel = self.canvas.create_oval(midx-rad,midy-rad,midx+rad,midy+rad,fill="white")
        self.line  = self.canvas.create_line(midx,midy,midx+cos(self.ang)*rad,midy-sin(self.ang)*rad) 
   
    def draw(self):
        # freq goes from 0 to the sampling-rate
        freq = self.scale.get()/100.0
        self.ang += 2*pi*freq
        self.canvas.delete(self.wheel)
        self.canvas.delete(self.line)
        self.rotate()
        self.master.after(1000/self.srate,self.draw)
        
                
    def quit(self):
        self.master.destroy()

    def __init__(self,master=None):
        Frame.__init__(self,master)
        master.title("Spinning Wheel and Aliasing")
        self.pack()
        self.createCanvas()
        # sampling-rate: 24 frames/sec
        self.srate = 24
        # start position: 0 degrees
        self.ang = 0.0
        self.rotate()  
        self.master = master
        self.draw()
        self.master.protocol("WM_DELETE_WINDOW", self.quit)
        self.canvas.create_text(self.size/2,10, text="Move the slider and see the wheel spin at different frequencies.")
        self.canvas.create_text(self.size/2,30, text="The slider range goes from 0 to the sampling rate (24 frames/sec)")
        self.canvas.create_text(self.size/2,50, text="You will see that the wheel spins counterclockwise, if the slider")
        self.canvas.create_text(self.size/2,70, text="is to the left. Then it reverses the spin if the slider goes beyond")
        self.canvas.create_text(self.size/2,90, text="the half-way point (freq=SR/2, the Nyquist), when aliasing occurs.")
        self.canvas.create_text(self.size/2,110, text="You can only properly sample the spinning wheel at freqs <= SR/2 ")

app = Application(Tk())
app.mainloop()
