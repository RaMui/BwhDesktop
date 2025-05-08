#include "CheckBoxHeader.h"
#include <QPainter>
#include <QMouseEvent>
#include <QStyle>
#include <QApplication>

CheckBoxHeader::CheckBoxHeader(Qt::Orientation orientation, QWidget* parent)
    : QHeaderView(orientation, parent)
{
    setSectionsClickable(true);
}

void CheckBoxHeader::setCheckState(Qt::CheckState state)
{
    if (m_checkState != state) {
        m_checkState = state;
        updateSection(0);
    }
}

Qt::CheckState CheckBoxHeader::checkState() const
{
    return m_checkState;
}

void CheckBoxHeader::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
    QHeaderView::paintSection(painter, rect, logicalIndex);

    if (logicalIndex == 0) {
        QStyleOptionButton option;
        option.state |= QStyle::State_Enabled;
        option.state |= (m_checkState == Qt::Checked ? QStyle::State_On :
            m_checkState == Qt::PartiallyChecked ? QStyle::State_NoChange :
            QStyle::State_Off);

        QSize checkBoxSize = style()->sizeFromContents(QStyle::CT_CheckBox, &option, QSize(), this);

        int x = rect.x() + (rect.width() - checkBoxSize.width()) / 2;
        int y = rect.y() + (rect.height() - checkBoxSize.height()) / 2;
        option.rect = QRect(QPoint(x, y), checkBoxSize);

        m_checkboxRect = option.rect;

        style()->drawControl(QStyle::CE_CheckBox, &option, painter);
    }
}

void CheckBoxHeader::mousePressEvent(QMouseEvent* event)
{
    if (orientation() == Qt::Horizontal && m_checkboxRect.contains(event->pos())) {
        if (m_checkState == Qt::Unchecked)
            m_checkState = Qt::Checked;
        else
            m_checkState = Qt::Unchecked;

        emit checkBoxClicked(m_checkState);
        updateSection(0);
    }

    QHeaderView::mousePressEvent(event);
}
