import sys
import GUI

def main():
    app = GUI.QApplication(sys.argv)
    Application = GUI.Window()
    sys.exit(app.exec())

main()
