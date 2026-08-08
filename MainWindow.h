//
// Created by lingxi on 2026/6/29.
//

#ifndef MINESWEEPER_MAINWINDOW_H
#define MINESWEEPER_MAINWINDOW_H
#include <QMainWindow>      // 主窗口基类
#include <QGridLayout>      // 游戏面板网格布局
#include <QLabel>           // 状态显示组件
#include <QAction>          // 菜单项操作
#include <QMenu>            // 游戏难度菜单
#include "GameLogic.h"      // 游戏核心逻辑层
#include "CustomButton.h"   // 自定义格子按钮
/**
 * @class MainWindow
 * @brief 扫雷游戏主窗口控制器
 * 实现MVC架构中的Controller
 */
class MainWindow : public QMainWindow {
    Q_OBJECT  // Qt元对象系统必需宏
private:
    GameLogic *gameLogic;///<游戏逻辑层实例（唯一拥有者，自动销毁）
    QWidget *centralWidget;///<中央控件（持有gridLayout和minesLabel）
    QGridLayout *gridLayout;///<游戏面板网格布局（动态尺寸）
    QLabel *minesLabel;///<地雷计数器显示（格式："剩余: 10"）
    QList<CustomButton*> buttons;///<按钮指针列表（row*cols + row顺序存储）
    int currentRows;///<当前生效的行数（用于新游戏重建）
    int currentCols;///<当前生效的列数
    int currentMines;///<当前生效的地雷数
    /**
    * @brief 启动新游戏
    * @param rows 网格行数（≥9）
    * @param cols 网格列数（≥9）
    * @param mines 地雷总数（≥10，< rows*cols*0.85）
    * 在调用前验证参数有效性
    */
    void startGame(int rows, int cols, int mines);
    /**
     * @brief 重建按钮网格
     */
    void rebuildButtons();
    /**
     * @brief 清理现有按钮
     */
    void clearButtons();
public:
    /**
     * @brief 主窗口构造函数
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /**
     * @brief 析构函数
     */
    ~MainWindow();
private slots:
    /**
     * @brief 游戏重置处理槽
     * 触发条件：GameLogic发出boardReset信号
     * 重建所有CustomButton（保留当前rows/cols/mines配置）
     */
    void onBoardReset();
    /**
     * @brief 格子揭示处理槽
     * 触发条件：GameLogic揭示非雷格子
     * @param row 被揭示格子的逻辑行
     * @param col 被揭示格子的逻辑列
     * @param value 格子数值
     */
    void onCellRevealed(int row, int col, int value);
    /**
     * @brief 格子状态变更处理槽
     * 触发条件：右键标记操作
     * @param row 格子逻辑行
     * @param col 格子逻辑列
     * @param state 新状态
     */
    void onCellStateChanged(int row, int col, CellState state);
    /**
     * @brief 剩余旗帜计数更新槽
     * 触发条件：标记/取消标记旗帜
     * @param remaining 当前剩余未标记地雷数
     */
    void onRemainingFlagsChanged(int remaining);
    /**
     * @brief 游戏结束处理槽
     * 触发条件：揭示地雷（失败）或揭示所有安全格子（胜利）
     * @param win true=胜利，false=失败
     */
    void onGameOver(bool win);
    /**
     * @brief 新游戏请求槽
     * 触发条件：用户选择"新游戏"菜单项
     */
    void onNewGame();
    /**
     * @brief 初级难度槽
     * - 9x9 网格
     * - 10 颗地雷
     * - 初始格子安全保证
     */
    void onBeginner();
    /**
     * @brief 中级难度槽
     * - 16x16 网格
     * - 40 颗地雷
     */
    void onIntermediate();
    /**
     * @brief 高级难度槽
     * - 16x30 网格
     * - 99 颗地雷
     */
    void onExpert();
    /**
     * @brief 自定义难度槽
     */
    void onCustomGame();
};
#endif //MINESWEEPER_MAINWINDOW_H