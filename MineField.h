//
// Created by lingxi on 2026/6/29.
//

#ifndef MINESWEEPER_MINEFIELD_H
#define MINESWEEPER_MINEFIELD_H

#include <vector>

/**
 * @enum CellState
 * @brief 表示雷区中单个格子的当前状态
 */
enum CellState
{
    opened,///< 格子已翻开，显示数字或空白
    closed,///< 格子未翻开，处于默认状态
    flagged,///< 格子被标记为旗帜
    question,///< 格子被标记为问号
    detonate///< 格子已翻开，雷被引爆
};
/**
 * @struct Cell
 * @brief 表示雷区中的单个格子，包含该格子的所有状态和属性
 */
typedef struct Cell
{
    bool isBomb;///<标记该格子是否为地雷（true=是地雷，false=安全区域）
    int roundMines;///<标记该格子周围8个相邻格子中的地雷数量
    CellState state;///<当前格子的状态（opened/closed/flagged/question）
} Cell;
/**
 * @class MineField
 * @brief 雷区管理类，雷区初始化、地雷布置、游戏逻辑判断等
 * 一维向量模拟二维雷区，通过index()方法实现坐标到索引的转换
 * 初始化时需指定行数、列数和地雷总数，首次点击时保证点击位置安全
 */
class MineField
{
private:
    int max_rows_;///<雷区总行数
    int max_cols_;///<雷区总列数
    int all_mines_;///<雷区总地雷数
    int flagsPlaced_;///<当前已放置的旗帜数量
    int closeNonMines;///<剩余需要标记的安全区域数量（用于判断游戏是否胜利）
    std::vector<Cell> board_;///<一维存储所有格子，行优先
    /**
     * @brief 将二维坐标转换为一维向量的索引
     * @param row 行坐标
     * @param col 列坐标
     * @return一维索引值
     */
    int index(int row, int col) const { return row * max_cols_ + col; }
public:
    /**
     * @brief 默认构造函数
     * 创建一个空雷区
     * 默认9*9，10雷
     */
    MineField();
    /**
     * @brief 初始化雷区
     * 分配内存并初始化所有格子为默认状态
     * 设置初始状态（closed，无雷）
     * 设置胜利计数器（closeNonMines=总格子数-雷数）
     */
    void initialize(int rows, int cols, int mines);
    /**
     * @brief 布雷（确保首次点击安全）
     * @param excludeRow 首次点击的行（此位置及周围不会生成雷）
     * @param excludeCol 首次点击的列
     */
    void placeMines(int excludeRow, int excludeCol);
    /**
     * @brief 计算所有非雷格子周围的雷数量
     * 遍历整个雷区，对每个安全格子统计其周围8个相邻格子中的雷总数
     */
    void calculateNumbers();
    /**
     * @brief 获取指定格子周围8个相邻格子中的雷数量
     * @param row 行坐标
     * @param col 列坐标
     * @return 雷数量（0-8）
     */
    int adjacentMines(int row, int col) const;
    /**
     * @brief 检查指定格子是否为雷
     * @param row 行坐标
     * @param col 列坐标
     * @return true=是雷，false=安全
     */
    bool isMine(int row, int col) const;
    /**
     * @brief 检查坐标是否在雷区内
     * @param row 行坐标
     * @param col 列坐标
     * @return true=在雷区内，false=越界
     */
    bool inBounds(int row, int col) const;
    int getRows() const { return max_rows_; }   ///< 获取雷区总行数
    int getCols() const { return max_cols_; }   ///< 获取雷区总列数
    int getMines() const { return all_mines_; } ///< 获取实际布置的雷总数
    /**
     * @brief 获取格子的显示值
     * @param row 行坐标
     * @param col 列坐标
     * @return
     *   -9:该格是雷
     *   -1:雷被引爆
     *   0-8:格子翻开后显示的周围雷数
     */
    int getCellValue(int row, int col) const;
    /**
     * @brief 获取格子的当前状态
     * @param row 行坐标
     * @param col 列坐标
     * @return CellState枚举值
     */
    CellState getCellState(int row, int col) const;
    /**
     * @brief 设置格子的状态
     * @param row 行坐标
     * @param col 列坐标
     * @param state 目标状态
     */
    void setCellState(int row, int col, CellState state);
    /**
     * @brief 获取剩余需要翻开的安全格子数量
     * @return 剩余安全格子数
     * 当该值为0时，所有安全格子已被翻开，玩家获胜
     */
    int hiddenNonMineCount() const { return closeNonMines; }
    /**
     * @brief 减少剩余安全格子计数
     */
    void decrementHiddenNonMine() { --closeNonMines; }
    int flagsPlaced() const { return flagsPlaced_; }///< 获取当前已放置的旗帜总数
    void incrementFlags() { ++flagsPlaced_; }///< 增加旗帜计数
    void decrementFlags() { --flagsPlaced_; }///< 减少旗帜计数
};
#endif // MINESWEEPER_MINEFIELD_H