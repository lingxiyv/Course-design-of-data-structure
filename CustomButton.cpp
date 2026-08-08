//
// Created by lingxi on 2026/6/29.
//

#include "CustomButton.h"
#include <QPixmap>

bool CustomButton::iconsLoaded = false;
QIcon CustomButton::flagIcon;
QIcon CustomButton::questionIcon;
QIcon CustomButton::boomIcon;
QIcon CustomButton::bombIcon;
QIcon CustomButton::numIcons[9];

CustomButton::CustomButton(int row, int col, QWidget* parent)
    :QPushButton(parent),row_(row),col_(col)
{
    setFixedSize(32,32);
    setIconSize(QSize(24,24));
    setAutoFillBackground(true);
}

void CustomButton::updateAppearance(CellState state, int value)
{
    switch (state)
    {
    case closed:setIcon(QIcon());setEnabled(true);setFlat(false);break;
    case flagged:setIcon(flagIcon);setEnabled(true);setFlat(false);break;
    case question:setIcon(questionIcon);setEnabled(true);setFlat(false);break;
    case detonate:setIcon(boomIcon);setEnabled(false);setFlat(true);break;
    case opened:setEnabled(false);setFlat(true);
        if (value==-9)setIcon(bombIcon);
        else if (value>=1&&value<=8)setIcon(numIcons[value]);
        else setIcon(QIcon());
    }
}

void CustomButton::loadIcons()
{
    if (iconsLoaded)return;
    flagIcon = QIcon(QPixmap(":/images/flag.png"));
    questionIcon = QIcon(":/images/question.png");
    boomIcon = QIcon(":/images/bomb_red.png");
    bombIcon = QIcon(":/images/bomb.png");
    for (int i = 1; i <= 8; ++i) {
        numIcons[i] = QIcon(QPixmap(QString(":/images/num%1.png").arg(i)));
    }
    iconsLoaded = true;
}

void CustomButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(row_, col_);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(row_, col_);
    }
    event->accept();
}
