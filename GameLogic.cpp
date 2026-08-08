//
// Created by lingxi on 2026/6/29.
//

#include "GameLogic.h"
#include <queue>
#include <utility>

GameLogic::GameLogic(QObject *parent)
    :QObject(parent),firstClick_(true),gameOver_(false)
{}

void GameLogic::startNewGame(int rows, int cols, int mines)
{
    if (rows <= 1 || cols <= 1|| mines <= 0||rows*cols <= mines+9||rows>100||cols>100)
        return;
    mineField_.initialize(rows,cols,mines);
    firstClick_=true;
    gameOver_=false;
    emit boardReset();//通知UI需要重建整个游戏网格
    emit remainingFlagsChanged(mines);//初始化剩余旗帜计数器
}

void GameLogic::handleLeftClick(int row, int col)
{
    if (gameOver_)return;
    if (!mineField_.inBounds(row,col))return;
    if (mineField_.getCellState(row,col)!=closed)return;
    if (firstClick_)
    {
        mineField_.placeMines(row,col);
        mineField_.calculateNumbers();
        firstClick_=false;
    }
    if (mineField_.isMine(row,col))
    {
        mineField_.setCellState(row,col,detonate);
        emit cellRevealed(row,col,-1);//通知踩雷
        revealAllMines();
        gameOver_=true;
        emit gameOver(false);//通知游戏结束
    }
    revealCell(row,col);
    checkWin();
}

void GameLogic::handleRightClick(int row, int col)
{
    if (gameOver_)return;
    if (!mineField_.inBounds(row,col))return;
    switch (mineField_.getCellState(row,col))
    {
        case closed:if (mineField_.flagsPlaced()==mineField_.getMines())break;
            mineField_.setCellState(row,col,flagged);
            mineField_.incrementFlags();
            emit cellStateChanged(row,col,flagged);//通知更新格子状态
            break;
        case opened:break;
        case flagged:mineField_.setCellState(row,col,question);
            mineField_.decrementFlags();
            emit cellStateChanged(row,col,question);//通知更新格子状态
            break;
        case question:mineField_.setCellState(row,col,closed);
            emit cellStateChanged(row,col,closed);//通知更新格子状态
            break;
        default:break;
    }
    int remaining = mineField_.getMines() - mineField_.flagsPlaced();
    emit remainingFlagsChanged(remaining);//通知更新剩余旗帜计数
}

int GameLogic::rows() const
{
    return mineField_.getRows();
}

int GameLogic::cols() const
{
    return mineField_.getCols();
}

void GameLogic::revealCell(int row, int col)
{
    int val=mineField_.getCellValue(row,col);
    if (val==0)
        bfsReveal(row,col);
    else
    {
        mineField_.setCellState(row, col, opened);
        mineField_.decrementHiddenNonMine();
        emit cellRevealed(row, col, val);
    }
}

void GameLogic::bfsReveal(int startRow, int startCol)
{
    if (!mineField_.inBounds(startRow, startCol) || mineField_.getCellState(startRow, startCol) != closed)
        return;
    std::queue<std::pair<int, int>> que;
    //起点入队前立即标记为opened（避免重复入队）
    mineField_.setCellState(startRow, startCol, opened);
    mineField_.decrementHiddenNonMine();
    emit cellRevealed(startRow, startCol, mineField_.getCellValue(startRow, startCol));
    que.push({startRow, startCol});
    while (!que.empty())
    {
        auto [r, c] = que.front();
        que.pop();
        int val = mineField_.getCellValue(r, c);

        //仅当值为0时扩展邻居（标准扫雷规则）
        if (val == 0)
        {
            for (int dr = -1; dr <= 1; ++dr)
            {
                for (int dc = -1; dc <= 1; ++dc)
                {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr, nc = c + dc;
                    //入队前检查边界+closed状态+非地雷
                    if (mineField_.inBounds(nr, nc) &&mineField_.getCellState(nr, nc) == closed &&
                        !mineField_.isMine(nr, nc))
                    {
                        //入队前立即标记opened（防重复入队）
                        mineField_.setCellState(nr, nc, opened);
                        mineField_.decrementHiddenNonMine();
                        emit cellRevealed(nr, nc, mineField_.getCellValue(nr, nc));
                        que.push({nr, nc});
                    }
                }
            }
        }
    }
}

void GameLogic::checkWin()
{
    if (mineField_.hiddenNonMineCount()==0)
    {
        gameOver_ = true;
        emit gameOver(true);
    }
}

void GameLogic::revealAllMines()
{
    for (int row = 0; row < rows(); ++row)
    {
        for (int col = 0; col < cols(); ++col)
        {
            if (mineField_.isMine(row, col)&&mineField_.getCellState(row,col)==closed)
            {
                mineField_.setCellState(row,col,opened);
                emit cellRevealed(row,col,-9);
            }
        }
    }
}
