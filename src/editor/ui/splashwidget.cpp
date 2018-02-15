#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qstyleoption.h>
#include <qpainter.h>

#include "splashwidget.h"

#define SCALE .75
#define IWIDTH 750
#define IHEIGHT 537

#define IMAGEURL ":/res/img/splash.jpg"

#define WIDTH ( (IWIDTH) * (SCALE))
#define HEIGHT ( (IHEIGHT) * (SCALE))


Idogep::SplashWidget::SplashWidget(QWidget* parent) : QWidget(parent)
{
    setGeometry(
        (QApplication::desktop()->width() - WIDTH ) / 2,
        (QApplication::desktop()->height() - HEIGHT ) / 2,
        WIDTH,
        HEIGHT
    );
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

Idogep::SplashWidget::~SplashWidget()
{
}

void Idogep::SplashWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QStyleOption styleOption;
    styleOption.init(this);
    
	QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
	painter.drawPixmap(0, 0, QPixmap(IMAGEURL).scaled(size()));
}

void Idogep::SplashWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
}
