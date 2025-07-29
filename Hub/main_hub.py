
import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton, QWidget
from PyQt5.QtGui import QIcon, QPixmap
from PyQt5.QtCore import Qt
import webbrowser

class WindowedFullScreen(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Ludas Hub")
        
        self.setWindowIcon(QIcon("images\Taskbar_image.ico")) # Sets the window icon
        self.setStyleSheet("background-color: #545454;") # Background color

        #adds left BG
        LBG = QLabel(self)
        LBG.setGeometry(0,100,600,800)
        LBG.setStyleSheet("background-color: #5b6770")

        #Engine Version
        Version = QLabel(self)
        Version.setText("Version: 1.0")
        Version.setGeometry(0,0,600,100)
        Version.setStyleSheet("color: #facd72; font-size: 80px;")
        Version.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)

        # Makes the image Label
        self.image_label = QLabel(self)

        # Adds Logo Image
        pixmap = QPixmap("images\logo and text seperated.png")
        self.image_label.setPixmap(pixmap)
        self.image_label.setScaledContents(True)
        self.image_label.setGeometry(1400,10, 500, 175)

        #adds GitHub link
        GH = QPushButton("Ludas GitHub", self)
        GH.setGeometry(1500,900, 400,100)
        GH.clicked.connect(self.linkGH)
        GH.setStyleSheet("background-color: #facd72; color: black; font-size: 35px")

        #Adds GitHub photo
        self.GHphoto = QLabel(self)
        GHpixmap = QPixmap("images\GitHub.png")
        self.GHphoto.setPixmap(GHpixmap)
        self.GHphoto.setScaledContents(True)
        self.GHphoto.setGeometry(1400,900,100,100)

        # Get screen size
        screen = QApplication.primaryScreen()
        rect = screen.availableGeometry()

        # Resize to fill the screen (but not cover taskbar if it's visible)
        self.setGeometry(rect)

        self.show()
    def linkGH(self):
        webbrowser.open("https://github.com/XTCooper11/Ludas")
        print("opening GitHub")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = WindowedFullScreen()
    sys.exit(app.exec_())
