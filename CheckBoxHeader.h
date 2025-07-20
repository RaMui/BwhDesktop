#ifndef CHECKBOXHEADER_H
#define CHECKBOXHEADER_H

#include <QHeaderView>
#include <QStyleOptionButton>

class CheckBoxHeader : public QHeaderView
{
    Q_OBJECT

public:
    explicit CheckBoxHeader(Qt::Orientation orientation, QWidget* parent = nullptr);

    void setCheckState(Qt::CheckState state);
    Qt::CheckState checkState() const;

signals:
    void checkBoxClicked(Qt::CheckState state);

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    mutable QRect m_checkboxRect;
    Qt::CheckState m_checkState = Qt::Unchecked;
};

#endif // CHECKBOXHEADER_H
