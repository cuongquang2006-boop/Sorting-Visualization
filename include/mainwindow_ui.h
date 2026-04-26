#ifndef MAINWINDOW_UI_H
#define MAINWINDOW_UI_H

#include "bar.h"
#include <QMainWindow> 
#include <QObject>
#include <QMouseEvent>

#include <vector>

#include <QLineEdit>
#include <qpushbutton.h>
#include <qlabel.h>
#include <QMessageBox>



class QColor;
class Timer;

enum class AlgoType
{
    Selection,
    Bubble,
    Insertion,
    Merge
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;


private:

    AlgoType currentAlgo;

    std::vector<int> originalArray;

    bool isSorting = false;
    bool isPaused = false;
    bool doneSort = false;

    QPushButton* btnApply;
    QPushButton* btnStart;
    QPushButton* btnRandom;
    QPushButton* btnPaused;
    QPushButton* btnStop;

    QLabel* titleLabel = new QLabel("Selection Sort");

    QLineEdit* inputArray;

    std::vector<Bar*> bars;
    
    QTimer* timer;

    int i = 0, j = 1;
    bool isAnimating = false;
    int animDuration = 300;

    int barWidth = 20;
    int spacing = 6;

    QWidget* sidebarWidget;

    struct MergeFrame
    {
        int l, r, mid, i, j, k;
        std::vector<int> temp;
        int phase;
    };
    std::vector<MergeFrame> mergeStack;

    void setupUI();
    std::vector<int> parseInput(QString text);

    void swapAnimation(int a, int b);

private slots:
    bool isSorted();
    void startSort(); 
    void stopSort();
    void finishSort();
    void stepSort();
    void togglePause();
    void randomData();

    void initFromArray(const std::vector<int>& arr);
    void initData();

    void setSpeedSlow();
    void setSpeedMedium();
    void setSpeedFast();

    void setAlgorithm(const QString& name);

    void stepSelectionSort();
    void stepBubbleSort();
    void stepInsertionSort();
    void stepMergeSort();


    QString getAlgoInfo(AlgoType algo);

};

#endif 
