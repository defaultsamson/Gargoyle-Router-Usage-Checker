#include "usagebar.h"

#include <QPainter>
#include <QPainterPath>
#include <QLayout>
#include <QLabel>

#include <iostream>

UsageBar::UsageBar()
{
    _text = new QLabel();

    // Adds centered text
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(_text);

    setLayout(layout);
}

void UsageBar::setText(QString text) {
    _text->setText(text);
}

QString UsageBar::text() {
    return _text->text();
}

void UsageBar::paintEvent(QPaintEvent *) {
    const int BORDER_THICKNESS = 1;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Outline
    QColor outlineColour = palette().color(QPalette::Dark);
    painter.fillRect(0, 0, rect().width(), rect().height(), outlineColour);
    // Base
    QColor bgColour = palette().color(QPalette::Base);
    painter.fillRect(BORDER_THICKNESS, BORDER_THICKNESS, rect().width() - (2 * BORDER_THICKNESS), rect().height() - (2 * BORDER_THICKNESS), bgColour);


    // Fill
    qreal percentageValue = static_cast<qreal>(value() - minimum()) / static_cast<qreal>(maximum() - minimum());
    QColor fillColour = palette().color(QPalette::Active, QPalette::Highlight);
    //painter.setBrush(fillColour);
    //painter.drawRoundedRect(0, 0, rect().width() * percentageValue, rect().height(), CORNER_ROUND_RADIUS, CORNER_ROUND_RADIUS);
    painter.fillRect(BORDER_THICKNESS, BORDER_THICKNESS, (rect().width() * percentageValue) - (2 * BORDER_THICKNESS), rect().height() - (2 * BORDER_THICKNESS), fillColour);

    // Outline
    //QColor outlineColour = palette().color(QPalette::);
    //painter.setBrush(outlineColour);
    //painter.setBrush(outlineColour);
    //painter.setBrush(QColor(60, 60, 60));
    //QPainterPath path;
    //path.addRoundedRect(0, 0, rect().width(), rect().height(), CORNER_ROUND_RADIUS, CORNER_ROUND_RADIUS);

    //painter.drawPath(path);

    //painter.setBrush(color);
    //painter.drawRoundedRect(0, 0, rect().width() * percentageValue, rect().height(), 4, 4);
}
