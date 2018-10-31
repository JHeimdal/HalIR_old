
# import os
import matplotlib.pyplot as plt
from subprocess import check_output as call
from spc import SPC
molecs = [{'name': 'co_1',  'comp': 5, 'intv': [2000., 2250.],'c': 1.3,'res':0.1,'T':303.,'P':1000.,'l':0.025},
          {'name': 'co_2',  'comp': 5, 'intv': [2040., 2045.],'c': 3.3,'res':0.1,'T':303.,'P':1000.,'l':0.025},
          {'name': 'co_c1', 'comp': 5, 'intv': [2000., 2245.],'c': 0.02,'res':0.1,'T':303.,'P':1000.,'l':3.00},
          {'name': 'co_c2', 'comp': 5, 'intv': [2000., 2245.],'c': 0.04,'res':0.1,'T':303.,'P':1000.,'l':3.00},
          {'name': 'co_c3', 'comp': 5, 'intv': [2000., 2245.],'c': 0.06,'res':0.1,'T':303.,'P':1000.,'l':3.00},
          {'name': 'co_c4', 'comp': 5, 'intv': [2000., 2245.],'c': 0.08,'res':0.1,'T':303.,'P':1000.,'l':3.00},
          {'name': 'co_c5', 'comp': 5, 'intv': [2000., 2245.],'c': 0.10,'res':0.1,'T':303.,'P':1000.,'l':3.00},
          {'name': 'co_c6', 'comp': 5, 'intv': [2000., 2245.],'c': 0.15,'res':0.1,'T':303.,'P':1000.,'l':3.00},
          {'name': 'hi_1', 'comp': 1701, 'intv': [2292., 2308.],'c': 2.0,'res':0.02,'T':303.,'P':1000.,'l':3.00},
          {'name': 'hi_2', 'comp': 1701, 'intv': [2292., 2308.],'c': 2.0,'res':0.02,'T':303.,'P':750.,'l':3.00},
          {'name': 'hi_3', 'comp': 1701, 'intv': [2292., 2308.],'c': 2.0,'res':0.02,'T':303.,'P':500.,'l':3.00},
          {'name': 'hi_4', 'comp': 1701, 'intv': [2292., 2308.],'c': 2.0,'res':0.02,'T':303.,'P':250.,'l':3.00},
          {'name': 'hi_5', 'comp': 1701, 'intv': [2292., 2308.],'c': 2.0,'res':0.005,'T':303.,'P':100.,'l':3.00},
          {'name': 'hi_6', 'comp': 1701, 'intv': [2292., 2308.],'c': 2.0,'res':0.005,'T':303.,'P':50.,'l':3.00},
          {'name': 'hi_7', 'comp': 1701, 'intv': [2292., 2308.],'c': 2.0,'res':0.005,'T':303.,'P':15.,'l':3.00},
          {'name': 'hi_8', 'comp': 1701, 'intv': [2292., 2308.],'c': 2.0,'res':0.001,'T':303.,'P':2.,'l':3.00},
          {'name': 'hcl_1', 'comp': 15, 'intv': [2650., 3150.], 'c':0.01,'res':0.02,'T':303.,'P':1000.,'l':3.00},
          {'name': 'hcl_2', 'comp': 15, 'intv': [2650., 3150.], 'c':0.01,'res':0.04,'T':303.,'P':1000.,'l':3.00},
          {'name': 'hcl_3', 'comp': 15, 'intv': [2650., 3150.], 'c':0.01,'res':0.06,'T':303.,'P':1000.,'l':3.00},
          {'name': 'hcl_4', 'comp': 15, 'intv': [2650., 3150.], 'c':0.01,'res':0.08,'T':303.,'P':1000.,'l':3.00},
          {'name': 'hcl_5', 'comp': 15, 'intv': [2650., 3150.], 'c':0.01,'res':0.1,'T':303.,'P':1000.,'l':3.00},
          {'name': 'co2_1', 'comp': 2,  'intv': [ 600.,  700.], 'c':1.1,'res':0.02,'T':303.,'P':1000.,'l':0.025},
          {'name': 'co2_2', 'comp': 2,  'intv': [1300., 1400.], 'c':2.1,'res':0.02,'T':303.,'P':1000.,'l':3.25},
          {'name': 'co2_3', 'comp': 2,  'intv': [2300., 2500.], 'c':1.1,'res':0.02,'T':303.,'P':1000.,'l':0.025}]


runfile = "!Demo MALT5 input file\n\
!=====================\n\
!All lines beginning with ! are comments and ignored\n\
!Comment\n\
    'Clean air, 1000mb 30C, 26m'\n\
!Fitfile, reffile\n\
    ''\n\
!Outfiles\n\
    ''\n\
!Abscof file\n\
    ''\n\
!Linelist, type\n\
    'c:\HITRAN\HITRAN08.par'   'c:\HITRAN\hitran.dat'\n\
!Scratch directory path\n\
    ''\n\
!\n\
!   #iter\n\
    0\n\
!\n\
!   low     high    ext     outspacing  SNR\n\
    {intv[0]}    {intv[1]}    1.     0.0         0\n\
!\n\
!   Ytype   #layers\n\
    1       1\n\
!Layer 1\n\
!-------\n\
!   type	Press   unit    fit   Temp    unit    fit   Path    unit    broad   isoscale\n\
    1		{P}     mb      f     {T}     K       f     {l}     m       0       0\n\
!   #malt components    # library spectra\n\
    1                   0\n\
!\n\
!Loop n_comp times\n\
!   comp   amount  unit     linefile    type    fit     display\n\
    {comp} {c}     mb       ''          ''       t	      t\n\
!\n\
 \n\
!Instrument/ILS\n\
!==============\n\
!   polynomial (# terms, initial values)\n\
    2       1.0     0.0     0.0     0.0\n\
!   shift   resoln  apod\n\
    -0.0    {res}   1\n\
    t       f\n\
!   fov     fap     phase   sym1    sym2    malign  PBadj   zoffset\n\
    3.0    0.0     0.0     0.0     0.0     0.0     0.0     0.0\n\
    t       f       t       f       f       f       f       f\n\
!   Channel spectra\n\
!   period  phase   amplitude\n\
	0.0		0.0		0.0\n\
      f           f           f\n\
	0.0		0.0		0.0\n\
      f           f           f\n"


if __name__ == '__main__':
    for m in molecs:
        # print "Calculating ",m['name']
        outf = open(m['name']+'.prm', 'w')
        outf.write(runfile.format(**m))
        outf.close()
        output = call(['malt5', m['name']+'.prm'])
        outf = open(m['name']+'.out', 'w')
        outf.write(output)
        outf.close()

    for m in molecs:
        if m['name'] not in ['hi_1', 'hi_2', 'hi_3', 'hi_4', 'hi_5', 'hi_6', 'hi_7', 'hi_8']:
            continue
        tn = m['name']+'.spc'
        td = SPC(tn)
        plt.plot(td.xdata, td.ydata, label=tn + '_' + str(m['P']))
    plt.legend()
    plt.show()
