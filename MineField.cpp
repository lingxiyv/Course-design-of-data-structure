//
// Created by lingxi on 2026/6/29.
//

#include "MineField.h"
#include <random>
#include <algorithm>
#include <iostream>

MineField::MineField()
    :max_rows_(9), max_cols_(9),all_mines_(10),flagsPlaced_(0),closeNonMines(71)
{}

void MineField::initialize(int rows, int cols, int mines)
{
    max_rows_ = rows;
    max_cols_ = cols;
    all_mines_ =mines;
    flagsPlaced_=0;
    board_.assign(max_cols_*max_rows_,{false,0,closed});
    closeNonMines=max_cols_*max_rows_-all_mines_;
}

void MineField::placeMines(int excludeRow, int excludeCol)
{
    // 创建可用位置列表（排除首次点击位置）
    std::vector<int> positions;
    positions.reserve(max_rows_ * max_cols_);  // 预分配内存，避免多次realloc
    for (int r = 0; r < max_rows_; ++r) {
        for (int c = 0; c < max_cols_; ++c) {
            // 跳过首次点击的位置（确保第一次点击永远不会是地雷）
            if (r == excludeRow && c == excludeCol) continue;
            //跳过周围，确保首次点击为0
            if (abs(r - excludeRow) <= 1 && abs(c - excludeCol) <= 1)continue;
            positions.push_back(index(r, c));  // 将二维坐标转换为一维索引存储
        }
    }
    // 初始化高质量随机数生成器
    std::random_device rd;       // 真随机数设备（通常基于硬件熵源）
    std::mt19937 g(rd());        // Mersenne Twister 算法生成器，使用真随机数初始化
    std::shuffle(positions.begin(), positions.end(), g);  // 随机打乱位置顺序
    // 在打乱后的位置中放置地雷
    for (int i = 0; i < all_mines_; ++i) {
        board_[positions[i]].isBomb=true; // 将选中的位置标记为地雷
    }
}

void MineField::calculateNumbers()
{
    for (int r = 0; r < max_rows_; ++r) {
        for (int c = 0; c < max_cols_; ++c) {
            int idx = index(r, c);
            if (board_[idx].isBomb == true) continue;  // 跳过雷
            board_[idx].roundMines = adjacentMines(r, c);
        }
    }
}

int MineField::adjacentMines(int row, int col) const
{
    int count = 0;
    for (int dr=-1;dr<=1;++dr)
    {
        for (int dc=-1;dc<=1;++dc)
        {
            if (dr == 0 && dc == 0)continue;
            int nr=row+dr,nc=col+dc;
            if (inBounds(nr,nc)&&isMine(nr,nc))
                ++count;
        }
    }
    return count;
}

bool MineField::isMine(int row, int col) const
{
    return inBounds(row,col)&&board_[index(row,col)].isBomb;
}

bool MineField::inBounds(int row, int col) const
{
    return row >= 0 && row < max_rows_ && col >= 0 && col < max_cols_;
}

int MineField::getCellValue(int row, int col) const
{
    if (!inBounds(row,col)||board_[index(row,col)].state==detonate)return -1;
    if (isMine(row,col))return -9;
    return board_[index(row,col)].roundMines;
}

CellState MineField::getCellState(int row, int col) const
{
    return board_[index(row,col)].state;
}

void MineField::setCellState(int row, int col, CellState state)
{
    board_[index(row,col)].state = state;
}