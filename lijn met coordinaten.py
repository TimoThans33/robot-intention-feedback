# -*- coding: utf-8 -*-
"""
Created on Tue Oct 13 14:15:20 2020

@author: Gebruiker
"""

# -*- coding: utf-8 -*-
"""
Created on Mon Oct 12 11:54:16 2020

@author: Gebruiker
"""


from PyQt5.QtWidgets import QApplication, QMainWindow

from PyQt5.QtGui import QPainter, QPen


from PyQt5.QtCore import Qt
import sys

class Window(QMainWindow):

    def __init__(self):

        super().__init__()

        self.title = "Test pijl coordinaten"

        self.top= 0

        self.left= 30

        self.width = 950

        self.height = 300

        self.InitWindow()

    def InitWindow(self):

        self.setWindowTitle(self.title)

        self.setGeometry(self.top, self.left, self.width, self.height)

        self.show()

    def paintEvent(self, event):

        painter = QPainter()

        painter.begin(self)

        painter.setRenderHint(QPainter.Antialiasing)

        painter.setPen(QPen(Qt.red,  8, Qt.SolidLine))

        painter.setBrush(Qt.white)
        
        painter.drawLine(150, 50, 150, 150)

        painter.drawLine(800, 100, 100, 100)

        painter.drawLine(150, 150, 100, 100)

        painter.drawLine(150, 50, 100, 100)

App = QApplication(sys.argv)

window = Window()

sys.exit(App.exec())
