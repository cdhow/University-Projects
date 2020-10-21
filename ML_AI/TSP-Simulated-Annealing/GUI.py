from PyQt5.QtWidgets import *
from PyQt5 import QtCore
from PyQt5 import QtGui
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import Problem
import db_handler as db


#---------Main GUI CLASS -----------
class Window(QMainWindow):

    def __init__(self, parent=None):
        QMainWindow.__init__(self, parent)
        # The current problem that the application is viewing initialise to berlin52
        self.currentProblem = Problem.Problem(None, [])
        # The current Solution
        self.currentSolution = None

        # Run application
        self.statusBar = QStatusBar()
        self.setStatusBar(self.statusBar)
        self.initUI()
        self.show()

    def initUI(self):
        self.setGeometry(400, 200, 1300, 700)
        self.setAutoFillBackground(True)
        self.setStyleSheet("background-color: #b3ffff")

        self.setWindowTitle("TSP Solver")
        self.tool_bar()
        self.layoutUI()

    # Program options
    def tool_bar(self):
        # Actions
        loadAction = QAction("&Load Problem", self)
        loadAction.setShortcut("Ctrl+L")
        loadAction.setStatusTip("Load a problem from the database with options to solve and display. Ctrl+L")
        loadAction.triggered.connect(self.onLoadProbelm)

        solutionsAction = QAction("&View Solution", self)
        solutionsAction.setShortcut("Ctrl+S")
        solutionsAction.setStatusTip("Choose and display a solution from the database, includes more options. Ctrl+S")
        solutionsAction.triggered.connect(self.onViewSolution)

        addProbAction = QAction("&Add Problem", self)
        addProbAction.setShortcut("Ctrl+A")
        addProbAction.setStatusTip("Upload a problem to the database. Ctrl+A")
        addProbAction.triggered.connect(self.onAddProblemToDB)

        # Tool bar Options
        optionsToolBar = self.addToolBar("Options Toolbar")
        optionsToolBar.addAction(loadAction)
        optionsToolBar.addAction(solutionsAction)
        optionsToolBar.addAction(addProbAction)


    def layoutUI(self):
        # Central Widget
        self.cntrlWidget = QWidget(self)
        self.cntrlWidget.setStyleSheet("background-color: #ccffff")
        self.cntrlWidget.setStyleSheet("""
        QLabel{color: #00004d;} 
        QPushButton{background-color: #5e9be5; color: #13060d;} 
        QLineEdit{background-color: #fff;}
        QComboBox""")

        # Main vbox
        vbox = QVBoxLayout(self.cntrlWidget)

        # Graph Canvas
        self.figure = plt.figure(figsize=(20, 10))
        self.figure.set_facecolor("none")
        self.canvas = FigureCanvas(self.figure)
        vbox.addWidget(self.canvas)
        # Tool bar Graph options
        graphToolbar = NavigationToolbar2QT(self.canvas, self.cntrlWidget)


        # Bottom of the window
        vbox.addStretch(1)
        hbox = QHBoxLayout()
        hbox.setContentsMargins(15, 10, 15, 7)

        # Details Section
        # Widgets
        self.detailsName = QLabel("No Problem or Solution Loaded")
        self.detailsName.setStyleSheet("font: 18px")
        self.dimensionsLabel = QLabel("Dimension: n/a")
        self.distanceLabel = QLabel("")
        self.timeLabel = QLabel("")
        # Create Details vbox and add widgets
        d_vbox = QVBoxLayout()
        d_vbox.addWidget(self.detailsName)
        d_vbox.addWidget(self.dimensionsLabel)
        d_vbox.addWidget(self.timeLabel)
        d_vbox.addWidget(self.distanceLabel)

        # Add Details section to the main hbox
        hbox.addLayout(d_vbox)

        # Solution details section
        hbox.addStretch(1)
        # Widgets
        self.authorLabel = QLabel("")
        self.dateLabel = QLabel("")
        self.algoLabel = QLabel("")
        # Create vbox for widgets
        s_vbox = QVBoxLayout()
        s_vbox.addWidget(self.authorLabel)
        s_vbox.addWidget(self.dateLabel)
        s_vbox.addWidget(self.algoLabel)
        hbox.addLayout(s_vbox)

        # Tour Section
        hbox.addStretch(1)
        # Widgets
        self.tourLabel = QLabel("")
        self.tourLabel.setAlignment(QtCore.Qt.AlignCenter)
        self.tourLabel.setStyleSheet("font: 16px")
        self.tourText = QLabel("")
        scroll = QScrollArea()
        scroll.setWidget(self.tourText)
        scroll.setWidgetResizable(True)
        scroll.setFixedWidth(400)
        # scroll.setStyleSheet("background-color: #394a6d")
        # Vbox for tour section
        t_vbox = QVBoxLayout()
        t_vbox.addWidget(self.tourLabel)
        t_vbox.addWidget(scroll)
        hbox.addLayout(t_vbox)

        # Options Sections
        hbox.addStretch(1)
        # Widgets
        solveBtn = QPushButton("SOLVE")
        solveBtn.clicked.connect(self.onSolveBtnClick)
        saveBtn = QPushButton("SAVE")
        saveBtn.clicked.connect(self.onSaveBtnClick)
        solveTimeLabel = QLabel("Solve Time:")
        self.solveTimeInput = QLineEdit()
        onlyInt = QtGui.QIntValidator()
        self.solveTimeInput.setValidator(onlyInt)
        # Add widgets to options hbox and vbox
        o_vbox = QVBoxLayout()
        o_hbox = QHBoxLayout()
        o_hbox.addWidget(solveBtn)
        o_hbox.addWidget(saveBtn)
        o_vbox.addLayout(o_hbox)
        o_hbox1 = QHBoxLayout()
        o_hbox1.addWidget(solveTimeLabel)
        o_hbox1.addWidget(self.solveTimeInput)
        o_vbox.addLayout(o_hbox1)

        # Add option hbox to main hbox
        hbox.addLayout(o_vbox)

        # Add the bottom hbox to the main vbox
        vbox.addLayout(hbox)

        self.setCentralWidget(self.cntrlWidget)

    # Loads a problem from the database
    def onLoadProbelm(self):
            text, okPressed = QInputDialog.getText(self.cntrlWidget, "Load Problem", "Problem Name:")
            if okPressed and text != " ":
                # Load problem and set to current Problem
                temp = db.loadProblem(text)
                if temp != -1:
                    self.currentProblem = temp
                    self.plotProblem()
                    self.detailsName.setText(self.currentProblem.name + " Problem")
                    self.dimensionsLabel.setText("Dimensions: " + str(self.currentProblem.dimension))
                    self.updatePLabels()
                    self.currentSolution = None
                    self.statusBar.showMessage("Problem Loaded.")

                else:
                    # Alert Problem not in db
                    dlg = QMessageBox(self.cntrlWidget)
                    dlg.setWindowTitle("Whoops")
                    dlg.setText("Problem does not exist.")
                    dlg.exec_()

    # Loads a solution from the database
    def onViewSolution(self):
        dlg = ViewSolutionDialog(self.cntrlWidget)
        dlg.exec_()
        # If SolutionID was use
        if dlg.okBtnClicked:
            solutionID = dlg.solutionID
            temp = db.fetchViaID(solutionID)
            # fetchViaID returns -1 if solutionID dne in db
            if temp != -1:
                self.currentSolution = temp
                self.loadSolution()
            else:
                # Alert Solution dne
                dlg = QMessageBox(self.cntrlWidget)
                dlg.setText("The solution you are trying to access does not exist.")
                dlg.exec_()

        # If Best option was chosen
        elif dlg.bestBtnClicked:
            solutionName = dlg.solutionName
            self.currentSolution = db.fetchViaFlag(solutionName, 'best')
            self.loadSolution()

        # If Random option was chosen
        elif dlg.randBtnClicked:
            solutionName = dlg.solutionName
            self.currentSolution = db.fetchViaFlag(solutionName, 'rand')
            self.loadSolution()

    def loadSolution(self):
        self.currentProblem = db.loadProblem(self.currentSolution.problemName)
        self.plotSolution()
        self.detailsName.setText(self.currentSolution.problemName + " Solution")
        self.updateSLabels()
        self.statusBar.showMessage("Solution Loaded")


    # Adds a problem the the database
    def onAddProblemToDB(self):
        name, okPressed = QInputDialog.getText(self.cntrlWidget, "Add Problem to Database", "Problem Name:")
        if okPressed and name != " ":
            # Add problem to the database, db.add returns -1 if problem dne, returns -2 if already in db
            retval = db.add(name, name + ".tsp")
            if retval == -1:
                # Alert error
                dlg = QMessageBox(self.cntrlWidget)
                dlg.setText("Problem file does not exist, check spelling and try again.")
                dlg.exec_()
            elif retval == -2:
                # Alert error
                dlg = QMessageBox(self.cntrlWidget)
                dlg.setText("Problem already exists in database.")
                dlg.exec_()
            else:
                self.statusBar.showMessage("Problem added to the database.")

    # Solves the current problem and displays it
    def onSolveBtnClick(self):
        # Check if there is a problem to solve
        if self.currentProblem.name is not None:
            time = self.solveTimeInput.text()
            if time != '':
                time = int(time)
                self.currentSolution = db.solve(self.currentProblem, time)
                self.plotSolution()
                self.detailsName.setText(self.currentSolution.problemName + " Solution")
                self.updateSLabels()
                self.solveTimeInput.setText("")
                self.statusBar.showMessage("")
                self.statusBar.showMessage("Solution complete using Simulated Annealing algorithm.")
            else:
                # Alert run duration not inputted
                dlg = QMessageBox(self.cntrlWidget)
                dlg.setText("Please enter algorithm run duration before clicking solve.")
                dlg.exec_()
        else:
            # Alert No problem loaded
            dlg = QMessageBox(self.cntrlWidget)
            dlg.setText("There is no problem loaded.")
            dlg.exec_()

    # Pushes solution to database
    def onSaveBtnClick(self):
        if self.currentSolution is not None:
            db.pushSolution(self.currentSolution)
            self.statusBar.showMessage("Solution Saved to database.")

    # Updates Solution labels
    def updateSLabels(self):
        self.distanceLabel.setText("Distance: " + str(self.currentSolution.tourLength))
        self.dimensionsLabel.setText("Dimensions: " + str(self.currentProblem.dimension))
        self.timeLabel.setText("Time: " + str(self.currentSolution.time) + " seconds")
        self.authorLabel.setText("Author: " + self.currentSolution.author)
        self.dateLabel.setText("Created: " + str(self.currentSolution.date))
        self.algoLabel.setText("Algorithm: " + self.currentSolution.algorithm)

        # Tour info
        self.tourLabel.setText(self.currentSolution.problemName + " Tour")
        self.tourText.setText("   " + ", ".join( repr(e) for e in self.currentSolution.tour) + "   ")

    # Updates on labels view problem
    def updatePLabels(self):
        self.distanceLabel.setText("")
        self.timeLabel.setText("")
        self.authorLabel.setText("")
        self.dateLabel.setText("")
        self.algoLabel.setText("")

        # Tour info
        self.tourLabel.setText("")
        self.tourText.setText("")


    def plotProblem(self):
        x = self.currentProblem.getXList()
        y = self.currentProblem.getYList()
        plt.cla()
        ax = self.figure.add_subplot(111)
        ax.scatter(x, y, c='#9999ff', s=10)
        ax.set_title(self.currentProblem.name + " Problem Graph")
        self.canvas.draw()

    def plotSolution(self):
        x = []
        y = []
        # Create list of x values in order of tour
        for i in self.currentSolution.tour:
            x.append(self.currentProblem.get_display(i)[0])
        # Create list of y values in order of tour
        for i in self.currentSolution.tour:
            y.append(self.currentProblem.get_display(i)[1])

        plt.cla()
        ax = self.figure.add_subplot(111)
        ax.plot(x, y, '#9999ff')
        ax.set_title(self.currentProblem.name + " Solution Graph")
        self.canvas.draw()

# ------- END CLASSS ----------


#------- View Solution dialog CLASS
class ViewSolutionDialog(QDialog):

    def __init__(self, *args, **kwargs):
        super(ViewSolutionDialog, self).__init__(*args, **kwargs)

        self.setWindowTitle('View Solution')
        self.setGeometry(750, 400, 500, 300)

        # Values used in main program window
        self.solutionID = None
        self.solutionName = None

        # Flags to determine what button has been clicked
        self.bestBtnClicked = False
        self.randBtnClicked = False
        self.okBtnClicked = False

        # -- Widgets
        # Choose by Category label
        chooseCatLabel = QLabel("Select a Category Below and Choose the Best or a Random")
        chooseCatLabel.setAlignment(QtCore.Qt.AlignCenter)
        chooseCatLabel.setStyleSheet("font: 17px")
        # Category Label
        catLabel = QLabel("Solution Category: ")
        # Category (Solution name of choice) combobox
        self.combo = QComboBox(self)
        solutionNames = db.fetchSolutionNames()
        self.combo.addItem('All')
        self.combo.addItems(solutionNames)

        # Buttons
        self.bestBtn = QPushButton("Best")
        self.bestBtn.clicked.connect(self.onBestBtn)
        self.bestBtn.clicked.connect(self.close)
        self.randBtn = QPushButton("Random")
        self.randBtn.clicked.connect(self.onRandBtn)
        self.randBtn.clicked.connect(self.close)


        # Choose by id label
        idChoiceLabel = QLabel("Or Choose Solution by ID")
        idChoiceLabel.setStyleSheet("font: 17px")
        idChoiceLabel.setAlignment(QtCore.Qt.AlignCenter)
        self.choiceID = QLineEdit(self)
        self.choiceID.setAlignment(QtCore.Qt.AlignCenter)
        onlyInt = QtGui.QIntValidator()
        self.choiceID.setValidator(onlyInt)

        # Choice id ok Button
        self.okBtn = QPushButton("Ok")
        self.okBtn.clicked.connect(self.onOkBtn)
        self.okBtn.clicked.connect(self.close)

        # --- Layout
        vbox = QVBoxLayout()
        vbox.addWidget(chooseCatLabel)

        hbox_1 = QHBoxLayout()
        hbox_1.addWidget(catLabel)
        hbox_1.addWidget(self.combo)
        vbox.addLayout(hbox_1)

        hbox_2 = QHBoxLayout()
        hbox_2.addWidget(self.bestBtn)
        hbox_2.addWidget(self.randBtn)
        vbox.addLayout(hbox_2)

        vbox.addStretch(1)

        vbox.addWidget(idChoiceLabel)
        hbox_3 = QHBoxLayout()
        hbox_3.addWidget(self.choiceID)
        hbox_3.addWidget(self.okBtn)
        vbox.addLayout(hbox_3)

        self.setLayout(vbox)


    # IF Ok is pressed store solutionID, update ok flag
    # IF Random was pressed store Solution name, update random flag
    # IF Best was pressed store Solution name, update best flag

    # Returns the solution name and updates the bestBtnClicked flag
    def onBestBtn(self):
        self.solutionName = str(self.combo.currentText())
        self.bestBtnClicked = True

    # Returns the solution name and updates the randBtnClicked flag
    def onRandBtn(self):
        self.solutionName = str(self.combo.currentText())
        self.randBtnClicked = True

    # Returns the solutionID and updates the okBtnClicked flag
    def onOkBtn(self):
        self.solutionID = int(self.choiceID.text())
        self.okBtnClicked = True

# --------END viewSolution CLASS
