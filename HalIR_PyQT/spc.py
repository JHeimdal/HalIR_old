#!/usr/bin/python3
import struct
import array

ftflgs, fversn, fexper, fexp, fnpts, ffirst, flast,\
    fnsub, fxtype, fytype, fztype, fpost, fdate, fres,\
    fsource, fpeakpt, fspare, fcmnt, fcatxt, flogoff,\
    fmods, fprocs, flevel, fsampin, ffactor, fmethod,\
    fzinc, fwplanes, fwinc, fwtype, freserv = range(31)
subflgs, subexp, subindx, subtime, subnext,\
    subnois, subnpts, subscan, subwlevel, subresv = range(10)


class SPC:
    def __init__(self, filename):
        self.ydata = array.array('f')
        self.xdata = array.array('f')
        with open(filename, 'rb') as file1:
            self.spchdr = struct.unpack(
                '<BBBcLddLBBBBL9s9sH32s130s30sLLBBHf48sfLfB187s', file1.read(512))
            # print self.spchdr[ftflgs]
            if (self.spchdr[ftflgs] == 0):
                # print "Single ",self.spchdr[fnsub]
                self.subhdr = struct.unpack('<BcHfffLLf4s', file1.read(32))
                # print "subhdr: ",self.subhdr
                self.ydata.fromfile(file1, self.spchdr[fnpts])
                for i in range(len(self.ydata)):
                    self.xdata.append(
                        self.spchdr[ffirst] + i * (self.spchdr[flast] - self.spchdr[ffirst]) / (self.spchdr[fnpts] - 1))
            elif (self.spchdr[ftflgs] == 4):
                # print "Multifile ",self.spchdr[fnsub]
                self.subhdr = struct.unpack('<BcHfffLLf4s', file1.read(32))
                # print "subhdr: ",self.subhdr
                self.ydata.fromfile(file1, self.spchdr[fnpts])
                for i in range(len(self.ydata)):
                    self.xdata.append(
                        self.spchdr[ffirst] + i * (self.spchdr[flast] - self.spchdr[ffirst]) / (self.spchdr[fnpts] - 1))

#	def write(self,filename):
#		with open(filename,'wb') as out:
#			#pack?
#
