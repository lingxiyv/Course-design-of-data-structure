//
// Created by lingxi on 2026/6/29.
//

#ifndef MINESWEEPER_GAMELOGIC_H
#define MINESWEEPER_GAMELOGIC_H

#include <QObject>
#include "MineField.h"
/**
 * @class GameLogic
 * @brief 核心逻辑控制器，实现游戏规则、用户交互处理和状态管理
 * 继承自QObject，使用Qt信号槽机制与UI层解耦，负责：
 * 游戏生命周期管理（开始、进行、结束）
 * 处理玩家左右键点击事件
 * 实现扫雷核心规则（首次点击安全、空白区域自动展开等）
 * 游戏状态验证（胜利/失败判断）
 */
class GameLogic : public QObject {
    Q_OBJECT  // 必须的Qt宏，启用信号槽机制和元对象系统
private:
    MineField mineField_;///< 雷区数据管理对象
    bool firstClick_;///< 标记是否为当前游戏的首次点击
    bool gameOver_;///< 游戏结束标志
    /**
     * @brief 揭开单个格子
     * @param row 行坐标
     * @param col 列坐标
     */
    void revealCell(int row, int col);
    /**
     * @brief 广度优先搜索展开空白区域
     * @param startRow 起始行（必须是值为0的格子）
     * @param startCol 起始列
     * 算法流程：
     * 1. 创建队列，起始点入队
     * 2. 当队列非空：
     *    a. 出队一个格子
     *    b. 揭示该格子（调用revealCell）
     *    c. 若该格子值为0，将其8个邻居中未翻开的格子入队
     * 3. 自动跳过已标记旗帜的格子
     * 避免递归导致的栈溢出，高效处理大面积空白区域
     */
    void bfsReveal(int startRow, int startCol);
    /**
     * @brief 胜利条件检查
     */
    void checkWin();
    /**
     * @brief 揭开所有地雷
     */
    void revealAllMines();
public:
    /**
     * @brief 构造函数
     * @param parent Qt对象树中的父对象
     * 初始化游戏状态为"未开始"，设置初始标志位
     */
    explicit GameLogic(QObject *parent = nullptr);
    /**
     * @brief 启动新游戏
     * @param rows 雷区行数
     * @param cols 雷区列数
     * @param mines 地雷总数
     */
    void startNewGame(int rows, int cols, int mines);
    /**
     * @brief 处理鼠标左键点击事件（翻开格子）
     * @param row 点击的行坐标
     * @param col 点击的列坐标
     */
    void handleLeftClick(int row, int col);
    /**
     * @brief 处理鼠标右键点击事件（标记/切换格子状态）
     * @param row 点击的行坐标
     * @param col 点击的列坐标
     */
    void handleRightClick(int row, int col);
    int rows() const;  ///< 获取当前雷区行数（代理mineField_.getRows()）
    int cols() const;  ///< 获取当前雷区列数（代理mineField_.getCols()）
signals:
    /**
     * @brief 格子翻开信号
     * @param row 行坐标
     * @param col 列坐标
     * @param value 格子值
     *   - -1: 踩雷（游戏结束时）
     *   - -9: 地雷（游戏结束时）
     *   - 0: 空白区域
     *   - 1-8: 周围地雷数量
     */
    void cellRevealed(int row, int col, int value);
    /**
     * @brief 格子状态变更信号
     * @param row 行坐标
     * @param col 列坐标
     * @param state 新状态
     * 右键点击导致状态变化时
     */
    void cellStateChanged(int row, int col, CellState state);
    /**
     * @brief 游戏结束信号
     * @param win true=胜利，false=失败
     * - 胜利：所有安全格子被翻开
     * - 失败：点击到地雷
     */
    void gameOver(bool win);
    /**
     * @brief 剩余旗帜计数变更信号
     * @param remaining 剩余旗帜数
     * 旗帜标记数量变化时
     */
    void remainingFlagsChanged(int remaining);
    /**
     * @brief 雷区重置信号
     * 新游戏开始时
     * 通知UI层销毁旧网格并重建新雷区界面
     */
    void boardReset();
};

#endif //MINESWEEPER_GAMELOGIC_H