import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *

class App(QWidget):

    def __init__(self):
        super().__init__()
        self.title = 'Pijl door afbeelding te projecteren'
        self.left = 0
        self.top = 0
        self.width = 640
        self.height = 480
        self.initUI()
    
    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
    
        # Create widget
        label = QLabel(self)
        pixmap = QPixmap('pijl1.png')
        label.setPixmap(pixmap)
        self.resize(pixmap.width(),pixmap.height())
        
        self.show()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = App()
    sys.exit(app.exec_())