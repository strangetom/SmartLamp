#!/home/tom/anaconda3/bin/python
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'lamp.ui'
#
# Created by: PyQt5 UI code generator 5.7
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
import json

#json_file = '/mnt/nas/www/light.json'
json_file = '/mnt/rpi/pi/ledLamp/light.json'

class Ui_lampGUI(object):
    def setupUi(self, lampGUI):
        lampGUI.setObjectName("lampGUI")
        lampGUI.resize(240, 300)
        self.centralwidget = QtWidgets.QWidget(lampGUI)
        self.centralwidget.setObjectName("centralwidget")
        

        self.btnRainbowMode = QtWidgets.QPushButton(self.centralwidget)
        self.btnRainbowMode.setGeometry(QtCore.QRect(20, 10, 200, 30))
        self.btnRainbowMode.setFlat(False)
        self.btnRainbowMode.setObjectName("btnRainbowMode")
        self.btnRainbowMode.clicked.connect(self.rainbowMode)

        self.btnStaticMode = QtWidgets.QPushButton(self.centralwidget)
        self.btnStaticMode.setGeometry(QtCore.QRect(20, 50, 200, 30))
        self.btnStaticMode.setFlat(False)
        self.btnStaticMode.setObjectName("btnStaticMode")
        self.btnStaticMode.clicked.connect(self.staticMode)

        self.btnStaticColour = QtWidgets.QPushButton(self.centralwidget)
        self.btnStaticColour.setGeometry(QtCore.QRect(190, 50, 30, 30))
        self.btnStaticColour.setFlat(False)
        self.btnStaticColour.setObjectName("btnStaticColour")
        self.btnStaticColour.clicked.connect(self.staticMode)
        try:
            red, green, blue = self.getColour()
            self.btnStaticColour.setStyleSheet("background-color: rgb({},{},{})".format(red, green, blue))
        except:
            pass

        self.btnNotificationMode = QtWidgets.QPushButton(self.centralwidget)
        self.btnNotificationMode.setGeometry(QtCore.QRect(20, 90, 200, 30))
        self.btnNotificationMode.setFlat(False)
        self.btnNotificationMode.setObjectName("btnNotification")
        self.btnNotificationMode.clicked.connect(self.notificationMode)

        self.btnStaticColour2 = QtWidgets.QPushButton(self.centralwidget)
        self.btnStaticColour2.setGeometry(QtCore.QRect(190, 90, 30, 30))
        self.btnStaticColour2.setFlat(False)
        self.btnStaticColour2.setObjectName("btnStaticColour")
        self.btnStaticColour2.clicked.connect(self.notificationMode)
        try:
            red, green, blue = self.getColour()
            self.btnStaticColour2.setStyleSheet("background-color: rgb({},{},{})".format(red, green, blue))
        except:
            pass            

        self.btnWarmMode = QtWidgets.QPushButton(self.centralwidget)
        self.btnWarmMode.setGeometry(QtCore.QRect(20, 130, 200, 30))
        self.btnWarmMode.setFlat(False)
        self.btnWarmMode.setObjectName("btnWarmMode")
        self.btnWarmMode.clicked.connect(self.warmMode)

        self.btnColdMode = QtWidgets.QPushButton(self.centralwidget)
        self.btnColdMode.setGeometry(QtCore.QRect(20, 170, 200, 30))
        self.btnColdMode.setFlat(False)
        self.btnColdMode.setObjectName("btnColdMode")
        self.btnColdMode.clicked.connect(self.coldMode)

        self.btnOffMode = QtWidgets.QPushButton(self.centralwidget)
        self.btnOffMode.setGeometry(QtCore.QRect(20, 210, 200, 30))
        self.btnOffMode.setFlat(False)
        self.btnOffMode.setObjectName("btnOffMode")
        self.btnOffMode.clicked.connect(self.offMode)

        self.lblStatus = QtWidgets.QLabel(self.centralwidget)
        self.lblStatus.setGeometry(QtCore.QRect(20, 250, 200, 20))
        self.lblStatus.setAlignment(QtCore.Qt.AlignCenter)
        self.lblStatus.setObjectName("lblStatus")
        self.lblStatus.setText("Lamp colour is " + self.getState())


        lampGUI.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(lampGUI)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 240, 23))
        self.menubar.setObjectName("menubar")
        lampGUI.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(lampGUI)
        self.statusbar.setObjectName("statusbar")
        lampGUI.setStatusBar(self.statusbar)

        self.retranslateUi(lampGUI)
        QtCore.QMetaObject.connectSlotsByName(lampGUI)

    def retranslateUi(self, lampGUI):
        _translate = QtCore.QCoreApplication.translate
        lampGUI.setWindowTitle(_translate("lampGUI", "Lamp Control"))
        self.btnRainbowMode.setText(_translate("lampGUI", "Rainbow Mode"))
        self.btnStaticMode.setText(_translate("lampGUI", "Static Mode"))
        self.btnNotificationMode.setText(_translate("lampGUI", "Notification Mode"))
        self.btnWarmMode.setText(_translate("lampGUI", "Warm Mode"))
        self.btnColdMode.setText(_translate("lampGUI", "Cold Mode"))
        self.btnOffMode.setText(_translate("lampGUI", "Turn Off"))
        self.btnStaticColour.setText(_translate("lampGUI", ""))
        

    def getState(self):
        with open(json_file, 'r') as f:
            data = json.load(f)
            return data['mode']

    def getColour(self):
        with open(json_file, 'r') as f:
            data = json.load(f)
            red = data['red']
            green = data['green']
            blue = data['blue']
            return red, green, blue


    def rainbowMode(self):
        with open(json_file, 'r+') as f:
            data = json.load(f)
            if data['mode'] != 'rainbow':
                data['mode'] = 'rainbow'
            f.seek(0)
            json.dump(data, f)
            f.truncate()
        self.lblStatus.setText("Lamp colour is " + self.getState())
        

    def staticMode(self):
        colour = QtWidgets.QColorDialog.getColor().getRgb()
        red, green, blue, _ = colour

        if colour != (0, 0, 0, 255):
            with open(json_file, 'r+') as f:
                data = json.load(f)
                if data['mode'] != 'static':
                    data['mode'] = 'static'            
                data['red'] = int(red)
                data['green'] = int(green)
                data['blue'] = int(blue)
                f.seek(0)
                json.dump(data, f)
                f.truncate()
            self.btnStaticColour.setStyleSheet("background-color: rgb({},{},{})".format(red, green, blue))
            self.lblStatus.setText("Lamp colour is " + self.getState())

    def notificationMode(self):
        colour = QtWidgets.QColorDialog.getColor().getRgb()
        red, green, blue, _ = colour

        if colour != (0, 0, 0, 255):
            with open(json_file, 'r+') as f:
                data = json.load(f)
                if data['mode'] != 'notification':
                    data['mode'] = 'notification'            
                data['red'] = int(red)
                data['green'] = int(green)
                data['blue'] = int(blue)
                data['app'] = 'none'
                f.seek(0)
                json.dump(data, f)
                f.truncate()
            self.btnStaticColour2.setStyleSheet("background-color: rgb({},{},{})".format(red, green, blue))
            self.lblStatus.setText("Lamp colour is " + self.getState())


    def warmMode(self):
        with open(json_file, 'r+') as f:
            data = json.load(f)
            if data['mode'] != 'warm':
                data['mode'] = 'warm'
            f.seek(0)
            json.dump(data, f)
            f.truncate()
        self.lblStatus.setText("Lamp colour is " + self.getState())

    def coldMode(self):
        with open(json_file, 'r+') as f:
            data = json.load(f)
            if data['mode'] != 'cold':
                data['mode'] = 'cold'
            f.seek(0)
            json.dump(data, f)
            f.truncate()
        self.lblStatus.setText("Lamp colour is " + self.getState())


    def offMode(self):
        with open(json_file, 'r+') as f:
            data = json.load(f)
            if data['mode'] != 'off':
                data['mode'] = 'off'
            f.seek(0)
            json.dump(data, f)
            f.truncate()
        self.lblStatus.setText("Lamp colour is " + self.getState())


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    lampGUI = QtWidgets.QMainWindow()
    ui = Ui_lampGUI()
    ui.setupUi(lampGUI)
    lampGUI.show()
    sys.exit(app.exec_())

