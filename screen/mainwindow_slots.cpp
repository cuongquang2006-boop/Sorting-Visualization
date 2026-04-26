#include "mainwindow_ui.h"
#include <QTimer>

// run alg anim
void MainWindow::stepSort()
{

    if (isAnimating) return;

    switch (currentAlgo)
    {
    case AlgoType::Selection:
        stepSelectionSort();
        break;

    case AlgoType::Bubble:
        stepBubbleSort();
        break;

    case AlgoType::Insertion:
        stepInsertionSort();
        break;

    case AlgoType::Merge:
        stepMergeSort();
        break;
    }
}


// select
void MainWindow::stepSelectionSort()
{
    if (i >= bars.size() - 1)
    {
        finishSort();

        for (auto b : bars)
            b->color = QColor(100, 255, 150);

        update();
        return;
    }

    for (auto b : bars)
        b->color = QColor(100, 150, 255);

    bars[i]->color = QColor(255, 80, 80);
    bars[j]->color = QColor(255, 180, 80);

    if (bars[i]->value > bars[j]->value)
    {
        swapAnimation(i, j);
        return;
    }

    j++;

    if (j >= bars.size())
    {
        i++;
        j = i + 1;
    }

    update();
}

// bubble
void MainWindow::stepBubbleSort()
{
    if (isAnimating) return;

    if (i >= (int)bars.size() - 1)
    {
        finishSort();

        for (auto b : bars)
            b->color = QColor(100, 255, 150);

        update();
        return;
    }

    if (j >= (int)bars.size() - i - 1)
    {
        bars[bars.size() - i - 1]->color = QColor(100, 255, 150);

        i++;
        j = 0;
        update();
        return;
    }

    for (auto b : bars)
        b->color = QColor(100, 150, 255);

    bars[j]->color = QColor(255, 80, 80);
    bars[j + 1]->color = QColor(255, 180, 80);

    if (bars[j]->value > bars[j + 1]->value)
    {
        swapAnimation(j, j + 1);
        return;
    }

    j++;

    update();
}

// insertion
void MainWindow::stepInsertionSort()
{
    if (i >= bars.size())
    {
        for (auto b : bars)
            b->color = QColor(100, 255, 150); // xanh hoàn thành

        finishSort();
        update();
        return;
    }

    if (j < 0)
    {
        i++;
        j = i - 1;
        return;
    }

    for (auto b : bars)
        b->color = QColor(100, 150, 255);

    bars[i]->color = QColor(255, 80, 80);

    if (j >= 0)
        bars[j]->color = QColor(255, 180, 80);

    if (j >= 0 && bars[j]->value > bars[j + 1]->value)
    {
        swapAnimation(j, j + 1);
        j--;
        return;
    }

    i++;
    j = i - 1;

    update();
}

// merge
void MainWindow::stepMergeSort()
{
    if (isAnimating) return;

    if (mergeStack.empty())
    {
        finishSort();

        for (auto b : bars)
            b->color = QColor(100, 255, 150);

        update();
        return;
    }

    MergeFrame &f = mergeStack.back();

    if (f.l >= f.r)
    {
        mergeStack.pop_back();
        return;
    }

    if (f.phase == 0)
    {
        f.mid = (f.l + f.r) / 2;
        for (auto b : bars)
            b->color = QColor(100, 150, 255);

        for (int idx = f.l; idx <= f.mid; idx++)
            bars[idx]->color = QColor(255, 150, 80);

        for (int idx = f.mid + 1; idx <= f.r; idx++)
            bars[idx]->color = QColor(255, 200, 80);

        f.phase = 1;
        f.i = f.l;
        f.j = f.mid + 1;
        f.k = 0;
        f.temp.clear();

        mergeStack.push_back({f.mid + 1, f.r, 0, 0, 0, 0, {}, 0});
        mergeStack.push_back({f.l, f.mid, 0, 0, 0, 0, {}, 0});

        update();
        return;
    }

    for (auto b : bars)
        b->color = QColor(100, 150, 255);

    for (int idx = f.l; idx <= f.mid; idx++)
        bars[idx]->color = QColor(255, 150, 80);

    for (int idx = f.mid + 1; idx <= f.r; idx++)
        bars[idx]->color = QColor(255, 200, 80);

    if (f.k > 0)
    {
        const int swapLeft = f.j - 1;
        const int swapRight = f.j;
        if (swapLeft >= 0 && swapRight < (int)bars.size())
        {
            swapAnimation(swapLeft, swapRight);
            f.j--;
            f.k--;
            return;
        }
        else
        {
            f.k = 0;
            f.temp.clear();
            update();
            return;
        }
    }

    if (!f.temp.empty())
    {
        const int origJ = f.temp.back();
        f.temp.clear();
        f.i++;
        f.mid++;
        f.j = origJ + 1;
        update();
        return;
    }

    if (f.i <= f.mid && f.j <= f.r)
    {
        bars[f.i]->color = QColor(255, 80, 80);
        bars[f.j]->color = QColor(255, 100, 100);

        if (bars[f.i]->value <= bars[f.j]->value)
        {
            f.i++;
            update();
            return;
        }
        else
        {
            const int origJ = f.j;
            f.k = origJ - f.i;
            f.temp.push_back(origJ);

            const int swapLeft = f.j - 1;
            const int swapRight = f.j;
            if (swapLeft >= 0 && swapRight < (int)bars.size())
            {
                swapAnimation(swapLeft, swapRight);
                f.j--;
                f.k--;
                return;
            }
            else
            {
                f.k = 0;
                f.temp.clear();
                update();
                return;
            }
        }
    }
    mergeStack.pop_back();
    update();
}

// check sorted
bool MainWindow::isSorted()
{
    for (int k = 0; k < bars.size() - 1; k++)
    {
        if (bars[k]->value > bars[k + 1]->value)
            return false;
    }
    return true;
}

// start animation
void MainWindow::startSort()
{
    if (isAnimating)
    {
        QMessageBox::warning(this, "Warning", "Animation is running!");
        return;
    }

    if (bars.empty())
    {
        QMessageBox::warning(this, "Warning", "No data!");
        return;
    }

    if (isSorted())
    {
        QMessageBox::warning(this, "Warning", "Array already sorted!");
        return;
    }

    isSorting = true;
    isPaused = false;

    btnApply->setDisabled(true);
    btnStart->setDisabled(true);
    btnRandom->setDisabled(true);

    i = 0;
    j = 1;

    mergeStack.clear();
    
    if (currentAlgo == AlgoType::Merge)
    {
        mergeStack.push_back({0, (int)bars.size() - 1, 0, 0, 0, 0, {}, 0});
    }

    timer->start();
}

// stop animation
void MainWindow::stopSort()
{
    if (!isSorting) return;
    timer->stop();
    isSorting = false;
    isPaused = false;

    auto reply = QMessageBox::question(
        this,
        "Confirm Stop",
        "Are you sure you want to stop and reset the sorting process?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes)
    {
        i = 0;
        j = 1;

        mergeStack.clear();

        bars.clear();
        initFromArray(originalArray);

        btnStart->setEnabled(true);
        btnApply->setEnabled(true);
        btnRandom->setEnabled(true);

        update();
    }
    else
    {
        isSorting = true;
        if (!isAnimating)
            timer->start();
    }
}

// finish animation
void MainWindow::finishSort()
{
    timer->stop();
    isSorting = false;

    btnStart->setEnabled(true);
    btnApply->setEnabled(true);
    btnRandom->setEnabled(true);
}

// paused
void MainWindow::togglePause()
{
    if (timer->isActive())
    {
        if (isAnimating) isPaused = true;
        else
        {
            timer->stop();
            isPaused = true;
        }
    }
    else if (isPaused)
    {
        isPaused = false;
        if (!isAnimating)
            timer->start();
    }
    btnPaused->setText(isPaused ? "Resume" : "Pause");
    update();
}

// speed
void MainWindow::setSpeedSlow()
{
    timer->setInterval(300);
    animDuration = 500;
}

void MainWindow::setSpeedMedium()
{
    timer->setInterval(150);
    animDuration = 300;
}

void MainWindow::setSpeedFast()
{
    timer->setInterval(50);
    animDuration = 150;
}

// random
void MainWindow::randomData()
{
    if (isAnimating) return;
    timer->stop();

    isSorting = false;
    isPaused = false;

    i = 0;
    j = 1;

    mergeStack.clear();
    initData();

    btnStart->setEnabled(true);
    btnApply->setEnabled(true);
    btnRandom->setEnabled(true);

    update();
}