from PyQt5 import QtWidgets
from PyQt5.QtCore import QThread, pyqtSignal
from fit_tab_ui import Ui_MyFit_tab
from halir_pylib import HalIR
from utilities import hitranDownload
from contextlib import contextmanager
from spc import SPC
import os
import json
import time


@contextmanager
def timeit(name):
    startTime = time.time()
    yield
    elapsedTime = time.time() - startTime
    print('[{}] finished in {} ms'.format(name, int(elapsedTime * 1000)))


class _runHAPI_Fit(QThread):
    done = pyqtSignal()

    def __init__(self, projDict, sampleDict):
        super(QThread, self).__init__()
        self.projDict = projDict
        self.sampleDict = sampleDict
        self.cwd = self.projDict['pdir']

        try:
            os.mkdir(self.cwd)
        except FileExistsError:
            pass

        os.chdir(self.cwd)
        self.components = sampleDict['comp']

    def run(self):
        # Download and create HITRAN data for HalIR
        with timeit('HalIR'):
            hitranDownload(self.projDict, self.sampleDict)
        # Start a HalIR session
            input = {'projectDict': self.projDict, 'sampleDict': self.sampleDict}
            self.halir = HalIR(json.dumps(input))
            self.halir.calcSpectra()
            self.calcSpec = self.halir.getSpectra()
            for s in self.calcSpec:
                print(s['molec'])
        self.done.emit()


class MyFitTab(QtWidgets.QWidget):
    # filesUpdate = QtCore.pyqtSignal(list)

    def __init__(self, projDict, sampleDict):
        super(MyFitTab, self).__init__()
        self.ui = Ui_MyFit_tab()
        self.ui.setupUi(self)
        self.sampleDict = sampleDict
        self.projDict = projDict
        # self.isoTxtToNum =\
        #    {v[1]: int(str(k[0])+str(k[1])) for k, v in hp.ISO.items()}
        # self.molecToIsoNum = self._getMolecToIsoNum()
        # self.runhp_Fit.done.connect(self.fitDone)
        # Do some setup for view window
        self.ui.rPlot.setXLink(self.ui.mPlot)
        self.specFiles = projDict['pfiles']
        self._plot()

    def _plot(self):
        """ plot files """
        for i, f in enumerate(self.specFiles):
            td = SPC(f)
            self.ui.mPlot.plot(td.xdata, td.ydata, pen=(i, 3))

    def runFit(self):
        self.runHAPI_Fit = _runHAPI_Fit(self.projDict, self.sampleDict)
        self.runHAPI_Fit.done.connect(self.fitDone)
        self.runHAPI_Fit.start()

    def fitDone(self):
        # upate plot
        td = SPC('CO.spc')
        self.ui.rPlot.plot(td.xdata, td.ydata)
