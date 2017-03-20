#ifndef WAVEWIDGET_H
#define WAVEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

class WaveWidget : public QWidget
{
    Q_OBJECT

public:
    WaveWidget(QWidget *parent = 0);
    ~WaveWidget();

    qreal progress() const;
    void setProgress(qreal progress);

    qreal speed() const;
    void setSpeed(const qreal &speed);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;


private:
    qreal mProgress;
    QTimer* mTimer = 0;
    qreal mBigNum;
    qreal mFa = 0;
    qreal mSpeed = 0.1;
    QPoint dragPosition;
    qreal _speed;

private slots:
    void shot();
};

#endif // WAVEWIDGET_H
