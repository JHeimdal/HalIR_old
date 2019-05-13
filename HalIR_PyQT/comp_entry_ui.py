# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'comp_entry.ui'
#
# Created by: PyQt5 UI code generator 5.11.3
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_CompEntry(object):
    def setupUi(self, CompEntry):
        CompEntry.setObjectName("CompEntry")
        CompEntry.resize(690, 60)
        self.horizontalLayout = QtWidgets.QHBoxLayout(CompEntry)
        self.horizontalLayout.setContentsMargins(-1, 0, -1, 0)
        self.horizontalLayout.setSpacing(6)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.molec_val = QtWidgets.QComboBox(CompEntry)
        self.molec_val.setObjectName("molec_val")
        self.molec_val.addItem("")
        self.molec_val.setItemText(0, "")
        self.horizontalLayout.addWidget(self.molec_val)
        self.iso_val = QtWidgets.QComboBox(CompEntry)
        self.iso_val.setObjectName("iso_val")
        self.horizontalLayout.addWidget(self.iso_val)
        self.amount_val = QtWidgets.QLineEdit(CompEntry)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.amount_val.sizePolicy().hasHeightForWidth())
        self.amount_val.setSizePolicy(sizePolicy)
        self.amount_val.setAlignment(QtCore.Qt.AlignCenter)
        self.amount_val.setObjectName("amount_val")
        self.horizontalLayout.addWidget(self.amount_val)
        self.amount_unit = QtWidgets.QComboBox(CompEntry)
        self.amount_unit.setObjectName("amount_unit")
        self.amount_unit.addItem("")
        self.amount_unit.addItem("")
        self.amount_unit.addItem("")
        self.amount_unit.addItem("")
        self.horizontalLayout.addWidget(self.amount_unit)
        self.fit_val = QtWidgets.QCheckBox(CompEntry)
        self.fit_val.setChecked(True)
        self.fit_val.setObjectName("fit_val")
        self.horizontalLayout.addWidget(self.fit_val)
        self.toolButton = QtWidgets.QToolButton(CompEntry)
        self.toolButton.setObjectName("toolButton")
        self.horizontalLayout.addWidget(self.toolButton)

        self.retranslateUi(CompEntry)
        QtCore.QMetaObject.connectSlotsByName(CompEntry)

    def retranslateUi(self, CompEntry):
        _translate = QtCore.QCoreApplication.translate
        CompEntry.setWindowTitle(_translate("CompEntry", "Form"))
        self.amount_val.setText(_translate("CompEntry", "0"))
        self.amount_unit.setItemText(0, _translate("CompEntry", "mb"))
        self.amount_unit.setItemText(1, _translate("CompEntry", "ub"))
        self.amount_unit.setItemText(2, _translate("CompEntry", "ppm"))
        self.amount_unit.setItemText(3, _translate("CompEntry", "ppb"))
        self.fit_val.setText(_translate("CompEntry", "   Fit"))
        self.toolButton.setText(_translate("CompEntry", "X"))

