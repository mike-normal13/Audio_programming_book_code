#!/usr/bin/python
# Magnitude plot
# works in conjuction with C++ program dft
# (c) Victor Lazzarini, 2007

from Tkinter import *
import os
import sys
import time

class Application(Frame):
  
    def createCanvas(self):
        self.size = 600
        self.canvas = Canvas(self,height=self.size,width=self.size,bg="white")
        self.canvas.pack()

    def draw(self):
        space = self.space
        for i in self.spec:
         val = (float(i)/self.max)*(self.size-20.0)/self.size
         self.canvas.create_line(space,self.size-20,space,self.size-val*self.size-20)  
         space += self.space
        for i in range(0, self.size, self.size/10):
         fr = "%d" % int(i*(self.sr/self.size)/2)
         self.canvas.create_text(i+6,self.size-5,text=fr)

    def message(self):
        self.canvas.create_text(self.size/2,10, text="usage: python spec.py filename")
        self.canvas.create_text(self.size/2,25, text=" to display the soundfile spectrum")
                
    def quit(self):
        self.master.destroy()

    def __init__(self,master=None):
        Frame.__init__(self,master)
        self.pack()
        self.createCanvas()
        self.master = master
        self.spec = []
        if(len(sys.argv) > 1 and os.spawnv(os.P_WAIT, "dft", ['dft',sys.argv[1]])==0):
         master.title("Magnitude Spectrum of %s" % sys.argv[1])   
         f = open("spec.txt")
         maxi = 0.1
         self.sr = float(f.readline());
         for line in f:
           self.spec.append(float(line))
           if float(line) > maxi: maxi = float(line)
         self.max = maxi
         self.space = self.size/(len(self.spec)+1)
         self.draw()
        else: self.message()
        self.master.protocol("WM_DELETE_WINDOW", self.quit)

app = Application(Tk())
app.mainloop()
