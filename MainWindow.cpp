//
// Created by lingxi on 2026/6/29.
//

#include "MainWindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QStatusBar>
#include <QMenuBar>

void MainWindow::startGame(int rows, int cols, int mines)
{
    currentRows = rows;
    currentCols = cols;
    currentMines = mines;
    gameLogic->startNewGame(rows, cols, mines);
}

void MainWindow::rebuildButtons()
{
    clearButtons();// 已包含 buttons.clear()
    int rows = gameLogic->rows();
    int cols = gameLogic->cols();
    // 不要 resize，直接循环 append
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            auto* btn = new CustomButton(r, c, centralWidget);
            connect(btn, &CustomButton::leftClicked, gameLogic, &GameLogic::handleLeftClick);
            connect(btn, &CustomButton::rightClicked, gameLogic, &GameLogic::handleRightClick);
            gridLayout->addWidget(btn, r, c);
            buttons.append(btn);// 添加后列表大小正好为 rows*cols
        }
    }
    centralWidget->adjustSize();
    adjustSize();
}

void MainWindow::clearButtons()
{
    while (QLayoutItem *item = gridLayout->takeAt(0)) {
        if (QWidget *w = item->widget()) {
            delete w;
        }
        delete item;
    }
    buttons.clear();
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), gameLogic(new GameLogic(this)), centralWidget(new QWidget(this)),
    gridLayout(new QGridLayout()),currentRows(9),currentCols(9), currentMines(10)
{
    setWindowTitle("扫雷游戏");
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);
    minesLabel=new QLabel(this);
    statusBar()->addPermanentWidget(minesLabel);
    QMenu *gameMenu=menuBar()->addMenu("&游戏设置");
    QAction *newGameAction=gameMenu->addAction(tr("&新游戏"),this,&MainWindow::onNewGame);
    gameMenu->addSeparator();
    QAction *beginnerAction=gameMenu->addAction(tr("&初级"),this,&MainWindow::onBeginner);
    QAction *IntermediateAction=gameMenu->addAction(tr("&中级"),this,&MainWindow::onIntermediate);
    QAction *ExpertAction=gameMenu->addAction(tr("&高级"),this,&MainWindow::onExpert);
    gameMenu->addSeparator();
    QAction *customAction=gameMenu->addAction(tr("&自定义"),this,&MainWindow::onCustomGame);
    connect(gameLogic, &GameLogic::boardReset, this, &MainWindow::onBoardReset);
    connect(gameLogic, &GameLogic::cellRevealed, this, &MainWindow::onCellRevealed);
    connect(gameLogic, &GameLogic::cellStateChanged, this, &MainWindow::onCellStateChanged);
    connect(gameLogic, &GameLogic::remainingFlagsChanged, this, &MainWindow::onRemainingFlagsChanged);
    connect(gameLogic, &GameLogic::gameOver, this, &MainWindow::onGameOver);
    CustomButton::loadIcons();
    startGame(currentRows, currentCols, currentMines);
}

MainWindow::~MainWindow(){}

void MainWindow::onBoardReset()
{
    rebuildButtons();
}

void MainWindow::onCellRevealed(int row, int col, int value)
{
    int index=row*currentCols+col;
    if (index>=0&&index<buttons.size())
    {
        if (value==-1)
        {
            buttons[index]->updateAppearance(detonate,-1);
        }
        else buttons[index]->updateAppearance(opened, value);
    }
}

void MainWindow::onCellStateChanged(int row, int col, CellState state)
{
    int index=row*currentCols+col;
    if (index>=0&&index<buttons.size())
    {
        buttons[index]->updateAppearance(state,0);
    }
}

void MainWindow::onRemainingFlagsChanged(int remaining)
{
    minesLabel->setText(tr("剩余旗帜: %1").arg(remaining));
}

void MainWindow::onGameOver(bool win)
{
    QString msg = win ? tr("游戏结束，你赢了！") : tr("哦！你踩到雷了！");
    QMessageBox::information(this, tr("Game Over"), msg,
                             QMessageBox::Ok);
    startGame(currentRows, currentCols, currentMines);
}

void MainWindow::onNewGame()
{
    startGame(currentRows, currentCols, currentMines);
}

void MainWindow::onBeginner()
{
    startGame(9,9,10);
}

void MainWindow::onIntermediate()
{
    startGame(16,16,40);
}

void MainWindow::onExpert()
{
    startGame(16,30,99);
}

void MainWindow::onCustomGame()
{
    bool ok;
    int rows=QInputDialog::getInt(this,tr("&自定义难度"),tr("行数:"),
        currentRows,5,34,1,&ok);
    if (!ok)return;
    int cols=QInputDialog::getInt(this,tr("&自定义难度"),tr("列数:"),
        currentCols,5,40,1,&ok);
    if (!ok)return;
    int maxMines=rows*cols-1;
    int mines=QInputDialog::getInt(this,tr("&自定义难度"),tr("雷数:"),
        currentMines,1,maxMines,1,&ok);
    if (!ok)return;
    startGame(rows,cols,mines);
}