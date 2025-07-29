from Hub import main_hub
import sys
import os
from PyQt5.QtWidgets import QApplication

# Add the Hub folder to Python path dynamically
sys.path.append(os.path.join(os.path.dirname(__file__), "Hub"))


def activate_program():
    app = QApplication(sys.argv)
    window = main_hub.WindowedFullScreen()
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    activate_program()