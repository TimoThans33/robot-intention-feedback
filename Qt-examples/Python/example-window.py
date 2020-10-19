import sys
from PyQt5.QtWidgets import QWidget, QPushButton, QApplication

class Example(QWidget):
    def __init__(self):
        super().__init__()

        height = 360
        width = 640

        self.setFixedHeight(height)
        self.setFixedWidth(width)

        self.initUI()
    def initUI(self):
        qbtn = QPushButton('Quit', self)
        qbtn.clicked.connect(QApplication.instance().quit)
        qbtn.resize(qbtn.sizeHint())
        qbtn.move(50, 50)

        # self.setGeometry(300, 300, 350, 250)
        # self.setWindowTitle('Quit button')
        self.showFullScreen()
    
def main():
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
