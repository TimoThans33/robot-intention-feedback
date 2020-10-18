import sys
from PyQt5.QtWidgets import QWidget, QPushButton, QApplication, QLabel
from PyQt5.QtGui import QPixmap, QTransform
import numpy as np 

THETA =30

class Example(QWidget):
    def __init__(self):
        super().__init__()

        height = 360
        width = 640

        self.setStyleSheet("background-color: white;")
        self.setFixedHeight(height)
        self.setFixedWidth(width)

        self.initUI()
    def initUI(self):
        qbtn = QPushButton('Quit', self)
        qbtn.clicked.connect(QApplication.instance().quit)
        qbtn.resize(qbtn.sizeHint())
        qbtn.move(50, 50)
        
        label = QLabel(self)
        pixmap = QPixmap('../Figures/pv-header-logo.png')
        # Transfromation matrix respectively x, y, z -axis
        AffineTransformMatrix = np.array([[np.cos(THETA/180*np.pi) , 0, np.sin(THETA/180*np.pi)], [0, 1, 0], [-np.sin(THETA/180*np.pi), 0, np.cos(THETA/180*np.pi)]])
        #AffineTransformMatrix = np.array([[np.cos(THETA/180*np.pi) , -np.sin(THETA/180*np.pi), 0], [np.sin(THETA/180*np.pi), np.cos(THETA/180*np.pi), 0], [0, 0, 1]])
        #AffineTransformMatrix = np.array([[1 , 0, 0], [0, np.cos(THETA/180*np.pi), -np.sin(THETA/180*np.pi)], [0, np.sin(THETA/180*np.pi), np.cos(THETA/180*np.pi)]])
        print(pixmap)
        #scaled_pixmap = pixmap.scaledToHeight(160)
        scaled_pixmap = pixmap.scaledToWidth(440)
        label.setPixmap(scaled_pixmap)
        label.move(150, 100)

        # self.setWindowTitle('Quit button')
        self.showFullScreen()

def main():
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
