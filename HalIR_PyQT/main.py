#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from PyQt5 import QtWidgets, QtCore
# from PyQt5.Qt import ItemIsUserCheckable
from halir_ui import Ui_MainWindow  # importing our generated file
from myproject_tab import MyProjectTab
from myfit_tab import MyFitTab

# from pyqtgraph import PlotWidget
import sys
import os


class MyWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(MyWindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.fileItems = None
        self.project_tab = MyProjectTab()
        self.ui.tabWidget.addTab(self.project_tab, "Project")
        # self.ui.filetree.setColumnCount(1)

        # Connections
        self.project_tab.filesUpdate.connect(self.updateFiles)
        self.project_tab.fitFiles.connect(self.onFitSpectra)

    def updateFiles(self, files):
        if not self.fileItems:
            self.fileItems = []
        for val in files:
            item = QtWidgets.QListWidgetItem(self.ui.filetree)
            item.setText(os.path.split(val)[-1])
            item.setFlags(item.flags() | QtCore.Qt.ItemIsUserCheckable |
                          QtCore.Qt.ItemIsEnabled)
            item.setCheckState(QtCore.Qt.Checked)
            self.fileItems.append(item)

    def onFitSpectra(self, projDict, sampleDict):
        # Open new tab and plot data when finised
        self.fitTab = MyFitTab()
        self.ui.tabWidget.addTab(self.fitTab,
                                 "Fit {} project".format(projDict['pname']))
        self.ui.tabWidget.setCurrentWidget(self.fitTab)

        # Start in new process
        self.fitTab.runFit(projDict, sampleDict)


if __name__ == '__main__':
    app = QtWidgets.QApplication([])
    application = MyWindow()
    application.show()
    sys.exit(app.exec())
