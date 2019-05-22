from PyQt5 import QtWidgets
from PyQt5.QtCore import QThread, pyqtSignal
from fit_tab_ui import Ui_MyFit_tab
from utilities import hitranDownload
import os


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
        hitranDownload(self.projDict, self.sampleDict)
        # Start a HalIR session
        self.halir = HalIR()
        self.done.emit()


class MyFitTab(QtWidgets.QWidget):
    # filesUpdate = QtCore.pyqtSignal(list)

    def __init__(self):
        super(MyFitTab, self).__init__()
        self.ui = Ui_MyFit_tab()
        self.ui.setupUi(self)
        # self.isoTxtToNum =\
        #    {v[1]: int(str(k[0])+str(k[1])) for k, v in hp.ISO.items()}
        # self.molecToIsoNum = self._getMolecToIsoNum()
        # self.runhp_Fit.done.connect(self.fitDone)

    def runFit(self, projDict, sampleDict):
        self.runHAPI_Fit = _runHAPI_Fit(projDict, sampleDict)
        self.runHAPI_Fit.done.connect(self.fitDone)
        self.runHAPI_Fit.start()

    def fitDone(self):
        # upate plot
        print(os.getcwd())
        print('Hej')
