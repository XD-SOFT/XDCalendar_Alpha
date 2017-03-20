#include "searchbox.h"
#include "config.h"
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPainter>
SearchBox::SearchBox(QWidget *parent) : QWidget(parent)
{
    textEdit = new QLineEdit (this);
    textEdit->setMaximumHeight (Const::Searchbox::kernelHeight);
    textEdit->setFrame (false);
    iconLabel = new QLabel (this);
    iconLabel->setPixmap (QPixmap::fromImage (QImage(":/Icon/Slide/01_79.png")));
    iconLabel->setScaledContents (true);
    iconLabel->setMaximumWidth (Const::Searchbox::kernelHeight);
    iconLabel->setMaximumHeight (Const::Searchbox::kernelHeight);

    auto s = [this] () {emit search (textEdit->text ());};
    connect (textEdit, &QLineEdit::textChanged, this, s);
    connect (textEdit, &QLineEdit::editingFinished, this, s);

    QHBoxLayout* mainLayout = new QHBoxLayout (this);
    mainLayout->setSpacing (0);
    mainLayout->addWidget (textEdit, 10);
    mainLayout->addWidget (iconLabel, 1);

    this->setMaximumHeight (Const::Searchbox::kernelHeight  + (mShadowMargin << 1));
}

void SearchBox::setIcon (const QPixmap& icon)
{
    this->icon = icon;
    iconLabel->setPixmap (icon);
}

SearchBox::~SearchBox () = default;

void drawShadow(QPainter &_painter, qint16 _margin, qreal _radius, QColor _start, QColor _end, qreal _startPosition, qreal _endPosition0, qreal _endPosition1, qreal _width, qreal _height);
void SearchBox::paintEvent (QPaintEvent* ev)
{
    QPainter p (this);
    drawShadow(p, mShadowMargin, 1.0, QColor(120, 120, 120, 32), QColor(255, 255, 255, 0), 0.0, 1.0, 0.6, width(), height());
    QWidget::paintEvent (ev);
}

void drawShadow(QPainter &_painter, qint16 _margin, qreal _radius, QColor _start, QColor _end, qreal _startPosition, qreal _endPosition0, qreal _endPosition1, qreal _width, qreal _height) {
    _painter.setPen(Qt::NoPen);

    QLinearGradient gradient;
    gradient.setColorAt(_startPosition, _start);
    gradient.setColorAt(_endPosition0, _end);
    // Right
    QPointF right0(_width - _margin, _height / 2);
    QPointF right1(_width, _height / 2);
    gradient.setStart(right0);
    gradient.setFinalStop(right1);
    _painter.setBrush(QBrush(gradient));
    _painter.drawRoundRect(QRectF(QPointF(_width - _margin*_radius, _margin), QPointF(_width, _height - _margin)), 0.0, 0.0);
    // Left
    QPointF left0(_margin, _height / 2);
    QPointF left1(0, _height / 2);
    gradient.setStart(left0);
    gradient.setFinalStop(left1);
    _painter.setBrush(QBrush(gradient));
    _painter.drawRoundRect(QRectF(QPointF(_margin *_radius, _margin), QPointF(0, _height - _margin)), 0.0, 0.0);
    // Top
    QPointF top0(_width / 2, _margin);
    QPointF top1(_width / 2, 0);
    gradient.setStart(top0);
    gradient.setFinalStop(top1);
    _painter.setBrush(QBrush(gradient));
    _painter.drawRoundRect(QRectF(QPointF(_width - _margin, 0), QPointF(_margin, _margin)), 0.0, 0.0);
    // Bottom
    QPointF bottom0(_width / 2, _height - _margin);
    QPointF bottom1(_width / 2, _height);
    gradient.setStart(bottom0);
    gradient.setFinalStop(bottom1);
    _painter.setBrush(QBrush(gradient));
    _painter.drawRoundRect(QRectF(QPointF(_margin, _height - _margin), QPointF(_width - _margin, _height)), 0.0, 0.0);
    // BottomRight
    QPointF bottomright0(_width - _margin, _height - _margin);
    QPointF bottomright1(_width, _height);
    gradient.setStart(bottomright0);
    gradient.setFinalStop(bottomright1);
    gradient.setColorAt(_endPosition1, _end);
    _painter.setBrush(QBrush(gradient));
    _painter.drawRoundRect(QRectF(bottomright0, bottomright1), 0.0, 0.0);
    // BottomLeft
    QPointF bottomleft0(_margin, _height - _margin);
    QPointF bottomleft1(0, _height);
    gradient.setStart(bottomleft0);
    gradient.setFinalStop(bottomleft1);
    gradient.setColorAt(_endPosition1, _end);
    _painter.setBrush(QBrush(gradient));
    _painter.drawRoundRect(QRectF(bottomleft0, bottomleft1), 0.0, 0.0);
    // TopLeft
    QPointF topleft0(_margin, _margin);
    QPointF topleft1(0, 0);
    gradient.setStart(topleft0);
    gradient.setFinalStop(topleft1);
    gradient.setColorAt(_endPosition1, _end);
    _painter.setBrush(QBrush(gradient));
    _painter.drawRoundRect(QRectF(topleft0, topleft1), 0.0, 0.0);
    // TopRight
    QPointF topright0(_width - _margin, _margin);
    QPointF topright1(_width, 0);
    gradient.setStart(topright0);
    gradient.setFinalStop(topright1);
    gradient.setColorAt(_endPosition1, _end);
    _painter.setBrush(QBrush(gradient));
    _painter.drawRoundRect(QRectF(topright0, topright1), 0.0, 0.0);
    // Widget
    _painter.setBrush(QBrush("#FFFFFF"));
    _painter.setRenderHint(QPainter::Antialiasing);
    _painter.drawRoundRect(QRectF(QPointF(_margin, _margin), QPointF(_width - _margin, _height - _margin)), _radius, _radius);
}
