//
// Created by lingxi on 2026/6/29.
//

#ifndef MINESWEEPER_CUSTOMBUTTON_H
#define MINESWEEPER_CUSTOMBUTTON_H

/**
 * @file CustomButton.h
 * @brief 扫雷游戏自定义格子按钮组件
 * 实现扫雷游戏的核心交互单元，每个CustomButton代表游戏面板中的一个格子。
 * 通过状态驱动外观更新，支持左键点击（揭示）和右键点击（标记）两种核心操作。
 * @note 该类严格遵循Qt对象模型规范，配合Q_OBJECT宏使用
 * @warning 在首次实例化前调用loadIcons()初始化资源
 */

#include <QPushButton>    // 基类：标准Qt按钮
#include <QMouseEvent>    // 鼠标事件处理
#include "MineField.h"    // 游戏逻辑层头文件（定义CellState等类型）

/**
 * @class CustomButton
 * @brief 扫雷游戏面板中的可交互格子按钮
 * 每个CustomButton实例对应游戏面板中的一个物理格子，负责：
 * 维护自身在游戏逻辑层中的坐标映射
 * 响应用户鼠标操作并转换为语义化信号
 * 根据游戏状态动态更新视觉表现
 */
class CustomButton : public QPushButton {
    Q_OBJECT  // Qt元对象系统必需宏（支持信号/槽/属性系统）
private:
    int row_;///< 逻辑行号
    int col_;///< 逻辑列号
    static QIcon flagIcon;///<旗帜图标
    static QIcon questionIcon;///<问号图标
    static QIcon boomIcon;///<地雷爆炸图标
    static QIcon bombIcon;///<地雷图标
    static QIcon numIcons[9];///<数字0-8图标
    static bool iconsLoaded;///<资源加载状态标志
public:
    /**
     * @brief 构造函数
     * @param row 按钮关联的逻辑行号
     * @param col 按钮关联的逻辑列号
     * @param parent 父控件指针
     * @note 在调用前确保loadIcons()已完成资源加载
     */
    explicit CustomButton(int row, int col, QWidget *parent = nullptr);
    /**
     * @brief 更新按钮视觉状态
     * 根据游戏逻辑层状态刷新UI表现，实现MVC模式中的View更新。
     * @param state 当前格子状态（来自MineField的CellState枚举）
     * @param value 格子逻辑值（0-8表示周围地雷数，仅当state=Revealed时有效）
     * 状态-外观映射规则：
     * - closed→显示基础背景（无图标）
     * - Flagged→旗帜图标
     * - Question→问号图标
     * - opened→根据value显示数字/地雷图标
     * @note value参数在state!=opened时被忽略
     */
    void updateAppearance(CellState state, int value);
    /**
     * @brief 全局图标资源初始化
     * 懒加载模式：仅首次调用时从资源文件加载所有图标。
     * 后续调用将被忽略（通过iconsLoaded标志控制）。
     * @note 必须在任何CustomButton实例化前调用
     */
    static void loadIcons();
signals:
    /**
     * @signal leftClicked
     * @brief 左键点击事件信号
     * @param row 触发事件的逻辑行号
     * @param col 触发事件的逻辑列号
     */
    void leftClicked(int row, int col);
    /**
     * @signal rightClicked
     * @brief 右键点击事件信号
     * 支持长按右键循环标记（未标记→旗帜→问号→未标记）。
     * @param row 触发事件的逻辑行号
     * @param col 触发事件的逻辑列号
     * @note 实际标记状态变更由GameLogic层处理
     */
    void rightClicked(int row, int col);
protected:
    /**
     * @brief 鼠标按下事件处理器
     * 重写QPushButton的默认实现，实现扫雷游戏特有的交互逻辑：
     * 1. 左键按下 → 立即触发leftClicked信号（非等待释放）
     * 2. 右键按下 → 立即触发rightClicked信号
     * 3. 中键/组合键 → 忽略
     * @param event 鼠标事件对象（包含按钮类型/位置等信息）
     */
    void mousePressEvent(QMouseEvent *event) override;
};

#endif //MINESWEEPER_CUSTOMBUTTON_H