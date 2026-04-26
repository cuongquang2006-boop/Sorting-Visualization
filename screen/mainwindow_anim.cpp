#include "mainwindow_ui.h"

#include <QTimer>
#include <QEasingCurve>

#include <QPainter>
#include <QLinearGradient>
#include <QColor>
#include <QFont>

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <QRect>

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>


// input from array
void MainWindow::initFromArray(const std::vector<int>& arr)
{
    originalArray = arr;

    for (auto b : bars)
    {
        b->disconnect();
        delete b;
    }
    bars.clear();

    mergeStack.clear();

    int n = arr.size();
    int totalWidth = n * (barWidth + spacing);
    int startX = (width() - totalWidth) / 2;
    int baseY = height() - 80;

    for (int val : arr)
    {
        Bar* b = new Bar(val, startX, baseY);

        connect(b, &Bar::changed, this, QOverload<>::of(&MainWindow::update));

        bars.push_back(b);
        startX += barWidth + spacing;
    }

    update();
}


// random array
void MainWindow::initData()
{
    for (auto b : bars)
    {
        b->disconnect();
        delete b;
    }

    originalArray.clear();
    bars.clear();

    mergeStack.clear();

    int n = 10;
    int totalWidth = n * (barWidth + spacing);
    int startX = (width() - totalWidth) / 2;
    int baseY = height() - 80;

    for (int i = 0; i < n; i++)
    {
        int val = rand() % 100 + 10;
        originalArray.push_back(val);

        Bar* b = new Bar(val, startX, baseY);

        connect(b, &Bar::changed, this, QOverload<>::of(&MainWindow::update));

        bars.push_back(b);

        startX += barWidth + spacing;
    }

    update();
}

// painter sorting UI
void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QLinearGradient bg(0, 0, width(), height());
    bg.setColorAt(0.0, QColor(252, 253, 255));
    bg.setColorAt(0.5, QColor(246, 248, 252));
    bg.setColorAt(1.0, QColor(238, 242, 250));
    p.fillRect(rect(), bg);

    if (bars.empty()) return;

    int n = bars.size();

    int maxVal = 1;
    for (auto b : bars)
        maxVal = std::max(maxVal, b->value);

    int availableWidth = width() * 0.85;
    int availableHeight = height() * 0.45;
    int baseY = height() * 0.65;

    int spacing = std::max(2, availableWidth / (n * 12));

    int barWidth = (availableWidth - (n - 1) * spacing) / n;
    barWidth = std::clamp(barWidth, 8, 32);

    if (n > 50)
    {
        barWidth = std::max(4, barWidth);
        spacing = 2;
    }

    int totalWidth = n * barWidth + (n - 1) * spacing;
    int startX = (width() - totalWidth) / 2;

    for (int i = 0; i < n; i++)
    {
        Bar* b = bars[i];

        int h = (b->value * availableHeight) / maxVal;
        int targetX = startX + i * (barWidth + spacing);

        if (!isAnimating)
            b->setX(targetX);

        int x = b->x();
        int y = baseY - h;

        QRect r(x, y, barWidth, h);

        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 0, 0, 18));
        p.drawRoundedRect(r.adjusted(1, 2, 1, 2), 4, 4);

        p.setBrush(b->color);
        p.drawRoundedRect(r, 5, 5);

        if (barWidth >= 14)
        {
            QFont font;
            font.setFamily("Tahoma");
            font.setBold(true);
            font.setPointSize(9);
            p.setFont(font);

            p.setPen(QColor(60, 60, 60));

            QRect textRect(x, baseY + 6, barWidth, 20);
            p.drawText(textRect, Qt::AlignCenter, QString::number(b->value));
        }
    }
}

// animation swap
void MainWindow::swapAnimation(int a, int b)
{
    if (isAnimating) return;
    isAnimating = true;

    timer->stop();

    Bar* A = bars[a];
    Bar* B = bars[b];

    int x1 = A->x();
    int x2 = B->x();

    QPropertyAnimation* animA = new QPropertyAnimation(A, "x");
    animA->setDuration(animDuration);
    animA->setStartValue(x1);
    animA->setEndValue(x2);

    QPropertyAnimation* animB = new QPropertyAnimation(B, "x");
    animB->setDuration(animDuration);
    animB->setStartValue(x2);
    animB->setEndValue(x1);

    QEasingCurve smooth(QEasingCurve::InOutQuad);
    animA->setEasingCurve(smooth);
    animB->setEasingCurve(smooth);

    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    group->addAnimation(animA);
    group->addAnimation(animB);

    connect(animA, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });

    connect(animB, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });

    connect(group, &QParallelAnimationGroup::finished, this, [=]() {

        std::swap(bars[a], bars[b]);
        isAnimating = false;

        if (!isSorting) {
            update();
            return;
        }
        if (isPaused)
        {
            timer->stop();
            update();
            return;
        }
        timer->start();

        update();
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}