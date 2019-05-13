from PyQt5 import QtWidgets
from PyQt5.QtCore import QThread, pyqtSignal
from fit_tab_ui import Ui_MyFit_tab
import hapi
import os


class _runHAPI_Fit(QThread):
    done = pyqtSignal()

    def __init__(self, projDict, sampleDict):
        super(QThread, self).__init__()
        self.projDict = projDict
        self.sampleDict = sampleDict
        self.cwd = self.projDict['pdir']
        self.isoTxtToNum =\
            {v[1]: int(str(k[0])+str(k[1])) for k, v in hapi.ISO.items()}
        self.molecToIsoNum = self._getMolecToIsoNum()
        try:
            os.mkdir(self.cwd)
        except FileExistsError:
            pass
        os.chdir(self.cwd)
        self.components = sampleDict['comp']

    def run(self):
        hapi.db_begin(self.projDict['pname'])
        for comp in self.components:
            if comp['isotop'] == 'Natural':
                molec = comp['molec']
                hapi.fetch_by_ids(molec,
                                  self.molecToIsoNum[molec],
                                  *self.sampleDict['ROI'])
            else:
                molec = comp['molec']
                hapi.fetch_by_ids(molec,
                                  self.isoTxtToNum[comp['isotop']],
                                  *self.sampleDict['ROI'])

        self.done.emit()

    def _getMolecToIsoNum(self):
        ans = {}
        molec = self._getMolecs()
        for m in molec:
            ans[m] =\
                [int(str(k[0])+str(k[1])) for k, v in hapi.ISO.items() if v[4] == m]
        return ans

    def _getMolecs(self):
        ans = []
        for k, v in hapi.ISO_ID.items():
            if v[-1] not in ans:
                ans.append(v[-1])
        return ans


class MyFitTab(QtWidgets.QWidget):
    # filesUpdate = QtCore.pyqtSignal(list)

    def __init__(self):
        super(MyFitTab, self).__init__()
        self.ui = Ui_MyFit_tab()
        self.ui.setupUi(self)
        # self.isoTxtToNum =\
        #    {v[1]: int(str(k[0])+str(k[1])) for k, v in hapi.ISO.items()}
        # self.molecToIsoNum = self._getMolecToIsoNum()
        # self.runHAPI_Fit.done.connect(self.fitDone)

    def runFit(self, projDict, sampleDict):
        self.runHAPI_Fit = _runHAPI_Fit(projDict, sampleDict)
        self.runHAPI_Fit.done.connect(self.fitDone)
        self.runHAPI_Fit.start()

    def fitDone(self):
        # upate plot
        print('Hej')
