# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'fit_tab.ui'
#
# Created by: PyQt5 UI code generator 5.11.3
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MyFit_tab(object):
    def setupUi(self, MyFit_tab):
        MyFit_tab.setObjectName("MyFit_tab")
        MyFit_tab.resize(986, 780)
        self.verticalLayout = QtWidgets.QVBoxLayout(MyFit_tab)
        self.verticalLayout.setObjectName("verticalLayout")
        self.splitter = QtWidgets.QSplitter(MyFit_tab)
        self.splitter.setOrientation(QtCore.Qt.Vertical)
        self.splitter.setObjectName("splitter")
        self.mPlot = PlotWidget(self.splitter)
        self.mPlot.setObjectName("mPlot")
        self.rPlot = PlotWidget(self.splitter)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.MinimumExpanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.rPlot.sizePolicy().hasHeightForWidth())
        self.rPlot.setSizePolicy(sizePolicy)
        self.rPlot.setObjectName("rPlot")
        self.verticalLayout.addWidget(self.splitter)

        self.retranslateUi(MyFit_tab)
        QtCore.QMetaObject.connectSlotsByName(MyFit_tab)

    def retranslateUi(self, MyFit_tab):
        _translate = QtCore.QCoreApplication.translate
        MyFit_tab.setWindowTitle(_translate("MyFit_tab", "Form"))

from pyqtgraph import PlotWidget
