#include "mainwindow_ui.h"

#include <QTimer>
#include <QStringList>
#include <QSize>
#include <QPoint>
#include <QRect>
#include <QEasingCurve>
#include <QCoreApplication>

#include <QColor>
#include <QIcon>

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

#include <QPropertyAnimation>

#include <vector>
#include <cstdlib>
#include <ctime>

// hàm khởi tạo 
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    srand(time(0));
    setWindowTitle("Sorting Visualizer");

    setMinimumSize(1080, 700);

    currentAlgo = AlgoType::Selection;

    setupUI();

    initData();

}

// setupUI 
void MainWindow::setupUI()
{
    QWidget* titleBar = new QWidget();
    
    titleBar->setFixedHeight(60);
    titleBar->setStyleSheet(R"(
        background: white;
        border-top-left-radius: 10px;
        border-top-right-radius: 10px;
    )");

    QHBoxLayout* titleLayout = new QHBoxLayout(titleBar);
    titleLayout->addSpacing(60);
    titleLayout->addWidget(titleLabel);

    titleLabel->setStyleSheet(R"(
    font-family: "Segoe UI", "Tahoma";
    font-size: 26px;
    font-weight: 700;
    color: #1f1f1f;
    letter-spacing: 0.5px;

    padding: 6px 12px;
    border-radius: 8px;
    )");

    // explain icon
    QLabel* helpIcon = new QLabel("?");
    helpIcon->setFixedSize(24, 24);
    helpIcon->setAlignment(Qt::AlignCenter);

    helpIcon->setStyleSheet(R"(
    QLabel {
        background-color: #e6f0f5;
        color: #1f1f1f;
        border-radius: 12px;
        font-weight: bold;
    }
    QLabel:hover {
        background-color: #cfe8ff;
    }
    )");

    helpIcon->setToolTip(
        "Selection Sort:\n"
        "- Time: O(n^2)\n"
        "- Stable: No\n"
        "- Idea: Find minimum and swap\n"
        );

    titleLayout->setContentsMargins(10, 0, 10, 0);
    titleLayout->addWidget(helpIcon);
    titleLayout->addStretch();

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QWidget* content = new QWidget();

    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* viewArea = new QWidget();
    viewArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout* controlBar = new QHBoxLayout();
    controlBar->setContentsMargins(50, 10, 10, 10);

    inputArray = new QLineEdit();
    inputArray->setPlaceholderText("Input array(Ex: 1 3 2 3): ");
    inputArray->setFixedHeight(36);
    inputArray->setStyleSheet(R"(
        QLineEdit 
        {
            background: white;
            padding: 8px 12px;
            border-radius: 6px;
            border: 1px solid #ccc;
            font-size: 14px;
        }
        QLineEdit:focus 
        {
            border: 1px solid #4A90E2;
        }
    )");

    btnApply = new QPushButton("Apply");
    btnStart = new QPushButton(" Start");
    btnRandom = new QPushButton(" Random");
    btnPaused = new QPushButton("Paused");
    btnStop = new QPushButton("Stop");

    QString btnStyle = R"(
    QPushButton {
        background-color: #4A90E2;
        color: white;
        padding: 8px 14px;
        border-radius: 6px;
        font-size: 14px;
    }
    QPushButton:hover {
        background-color: #6aa9ff;
    }
    QPushButton:pressed {
        background-color: #3b7bd6;
    }
    QPushButton:disabled {
        background-color: #a0a0a0;
        color: #e0e0e0;
    }
    )";

    QString pauseStyle = R"(
    QPushButton {
        background-color: #f5a623;
        color: white;
        padding: 8px 14px;
        border-radius: 6px;
        font-size: 14px;
    }
    QPushButton:hover {
        background-color: #ffbe3d;
    }
    QPushButton:pressed {
        background-color: #e0951a;
    }
    QPushButton:disabled {
        background-color: #d6b980;
        color: #eeeeee;
    }
    )";

    QString stopStyle = R"(
    QPushButton {
        background-color: #e74c3c;
        color: white;
        padding: 8px 14px;
        border-radius: 6px;
        font-size: 14px;
    }
    QPushButton:hover {
        background-color: #ff6b5a;
    }
    QPushButton:pressed {
        background-color: #c0392b;
    }
    QPushButton:disabled {
        background-color: #d6a0a0;
        color: #eeeeee;
    }
    )";

    QComboBox* speedBox = new QComboBox();
    speedBox->addItem("Slow");
    speedBox->addItem("Medium");
    speedBox->addItem("Fast");

    speedBox->setFixedHeight(36);
    speedBox->setStyleSheet(R"(
    QComboBox {
        background: white;
        padding: 6px 28px 6px 10px;
        border-radius: 8px;
        border: 1px solid #ccc;
    }
    QComboBox::drop-down {
        width: 22px;
        border-left: 1px solid #ddd;
    }
    QComboBox:hover {
        border: 1px solid #4A90E2;
    }
    QComboBox:focus {
        border: 1px solid #4A90E2;
    }
    )");
    
    btnStart->setStyleSheet(btnStyle);
    btnApply->setStyleSheet(btnStyle);
    btnRandom->setStyleSheet(btnStyle);
    btnPaused->setStyleSheet(pauseStyle);
    btnStop->setStyleSheet(stopStyle);

    controlBar->addStretch();
    controlBar->addWidget(inputArray);
    controlBar->addWidget(btnApply);
    controlBar->addWidget(btnRandom);
    controlBar->addWidget(btnStart);
    controlBar->addWidget(speedBox);
    controlBar->addWidget(btnPaused);
    controlBar->addWidget(btnStop);
    controlBar->addStretch();

    contentLayout->addLayout(titleLayout);
    contentLayout->addWidget(viewArea);
    contentLayout->addStretch();
    contentLayout->addLayout(controlBar);

    QVBoxLayout* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayout->addWidget(titleBar);   
    rootLayout->addWidget(content);

    sidebarWidget = new QWidget(this);
    sidebarWidget->setGeometry(-320, 0, 320, height());
    sidebarWidget->setStyleSheet(R"(
        background: rgba(255,255,255,0.95);
        border-right: 1px solid rgba(0,0,0,0.08);
        border-top-right-radius: 12px;
        border-bottom-right-radius: 12px;
    )");

    // sidebar
    QVBoxLayout* sidebar = new QVBoxLayout(sidebarWidget);
    sidebar->setSpacing(8);

    QPushButton* btnSelection = new QPushButton("  Selection Sort");
    QPushButton* btnBubble = new QPushButton("  Bubble Sort");
    QPushButton* btnInsert = new QPushButton("  Insertion Sort");
    QPushButton* btnMerge = new QPushButton("  Merge Sort");

    btnSelection->setStyleSheet(R"(
    background: #e6f0f5;
    border-radius: 8px;
    )");

    QString sideStyle = R"(
    QPushButton {
        text-align: left;
        padding: 12px;
        border-radius: 8px;
        background-color: transparent;
        font-size: 14px;
        color: #1f1f1f;
        border: none;
    }

    QPushButton:hover {
        background-color: #e6f0f5;
    }

    QPushButton:checked {
        background-color: #cfe8ff;
        color: #1f1f1f;
    }

    QPushButton:disabled {
        background-color: rgba(0,0,0,0.1);
        color: rgba(0,0,0,0.4);
    }
    )";

    // add qss
    btnSelection->setStyleSheet(sideStyle);
    btnBubble->setStyleSheet(sideStyle);
    btnInsert->setStyleSheet(sideStyle);
    btnMerge->setStyleSheet(sideStyle);

    // color holder
    btnSelection->setCheckable(true);
    btnBubble->setCheckable(true);
    btnInsert->setCheckable(true);
    btnMerge->setCheckable(true);

    // add btn to layout
    sidebar->addSpacing(30);
    sidebar->addWidget(btnSelection);
    sidebar->addWidget(btnBubble);
    sidebar->addWidget(btnInsert);
    sidebar->addWidget(btnMerge);
    sidebar->addStretch();

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(30);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0, 0, 0, 80));
    sidebarWidget->setGraphicsEffect(shadow);

    sidebarWidget->raise();

    QPushButton* btnMenu = new QPushButton(this);
    btnMenu->setFixedSize(48, 48);
    btnMenu->move(10, 10);
    btnMenu->raise();

    btnMenu->setIcon(QIcon(":/icons/menu.png"));
    btnMenu->setIconSize(QSize(30, 30));

    btnMenu->setStyleSheet(R"(
        QPushButton {
            background: white;
            border: none;
        }
        QPushButton:hover {
            background: rgba(0,0,0,0.05);
            border-radius: 12px;
        }
        QPushButton:pressed {
            background: rgba(0,0,0,0.1);
        }
    )");

    bool* sidebarVisible = new bool(false);

    connect(btnMenu, &QPushButton::clicked, [=]() 
        {
        *sidebarVisible = !(*sidebarVisible);

        QPropertyAnimation* anim = new QPropertyAnimation(sidebarWidget, "geometry");
        anim->setDuration(200);
        anim->setEasingCurve(QEasingCurve::InOutCubic);

        
        if (*sidebarVisible) 
        {
            anim->setStartValue(QRect(-320, 0, 320, 2000));
            anim->setEndValue(QRect(0, 0, 320, 2000));
        }
        else 
        {
            anim->setStartValue(QRect(0, 0, 320, 2000));
            anim->setEndValue(QRect(-320, 0, 320, 2000));
        }
        anim->start(QAbstractAnimation::DeleteWhenStopped);

        // menu animation
        QPropertyAnimation* btnAnim = new QPropertyAnimation(btnMenu, "pos");
        btnAnim->setDuration(200); // tùy chỉnh tốc độ
        btnAnim->setEasingCurve(QEasingCurve::InOutCubic);

        if (*sidebarVisible)
        {
            btnAnim->setEndValue(QPoint(320, 10));
            btnMenu->setIcon(QIcon(":/icons/back.png"));
        }
        else
        {
            btnAnim->setEndValue(QPoint(10, 10));
            btnMenu->setIcon(QIcon(":/icons/menu.png"));
        }

        btnAnim->start(QAbstractAnimation::DeleteWhenStopped);
        });

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::stepSort);

    // control btn
    connect(btnStart, &QPushButton::clicked, this, &MainWindow::startSort);
    connect(btnRandom, &QPushButton::clicked, this, &MainWindow::randomData);
    connect(speedBox, &QComboBox::currentIndexChanged, this, [=](int index)
        {
            switch (index)
            {
            case 0: setSpeedSlow(); break;
            case 1: setSpeedMedium(); break;
            case 2: setSpeedFast(); break;
            }
        });

    connect(btnApply, &QPushButton::clicked, [=]() 
        {
        auto arr = parseInput(inputArray->text());
        if (!arr.empty()) initFromArray(arr);
        });
    connect(btnPaused, &QPushButton::clicked, this, &MainWindow::togglePause);
    connect(btnStop, &QPushButton::clicked, this, &MainWindow::stopSort);

    //btn menu connect
    connect(btnSelection, &QPushButton::clicked, this,
            [this,helpIcon, btnSelection, btnBubble, btnInsert, btnMerge]() {
                currentAlgo = AlgoType::Selection;
                titleLabel->setText("Selection Sort");
                helpIcon->setToolTip(getAlgoInfo(AlgoType::Selection));
                btnSelection->setChecked(true);
                btnBubble->setChecked(false);
                btnInsert->setChecked(false);
                btnMerge->setChecked(false);
            });

    connect(btnBubble, &QPushButton::clicked, this,
            [this,helpIcon, btnSelection, btnBubble, btnInsert, btnMerge]() {
                currentAlgo = AlgoType::Bubble;
                titleLabel->setText("Bubble Sort");
                helpIcon->setToolTip(getAlgoInfo(AlgoType::Bubble));
                btnSelection->setChecked(false);
                btnBubble->setChecked(true);
                btnInsert->setChecked(false);
                btnMerge->setChecked(false);
            });

    connect(btnInsert, &QPushButton::clicked, this,
            [this,helpIcon, btnSelection, btnBubble, btnInsert, btnMerge]() {
                currentAlgo = AlgoType::Insertion;
                titleLabel->setText("Insertion Sort");
                helpIcon->setToolTip(getAlgoInfo(AlgoType::Insertion));
                btnSelection->setChecked(false);
                btnBubble->setChecked(false);
                btnInsert->setChecked(true);
                btnMerge->setChecked(false);
            });

    connect(btnMerge, &QPushButton::clicked, this,
            [this,helpIcon, btnSelection, btnBubble, btnInsert, btnMerge]() {
                currentAlgo = AlgoType::Merge;
                titleLabel->setText("Merge Sort");
                helpIcon->setToolTip(getAlgoInfo(AlgoType::Merge));
                btnSelection->setChecked(false);
                btnBubble->setChecked(false);
                btnInsert->setChecked(false);
                btnMerge->setChecked(true);
            });

    animDuration = 300;
    timer->setInterval(150);
}

// get information
QString MainWindow::getAlgoInfo(AlgoType algo)
{
    switch (algo)
    {
    case AlgoType::Selection:
        return "Selection Sort:\n"
               "- Time: O(n²)\n"
               "- Stable: No\n"
               "\nSteps:\n"
               "1. Find minimum element\n"
               "2. Swap with current position\n"
               "3. Repeat for next index";

    case AlgoType::Bubble:
        return "Bubble Sort:\n"
               "- Time: O(n²)\n"
               "- Stable: Yes\n"
               "\nSteps:\n"
               "1. Compare adjacent elements\n"
               "2. Swap if needed\n"
               "3. Largest element moves to end";

    case AlgoType::Insertion:
        return "Insertion Sort:\n"
               "- Time: O(n²)\n"
               "- Stable: Yes\n"
               "\nSteps:\n"
               "1. Take current element\n"
               "2. Shift larger elements right\n"
               "3. Insert into correct position";

    case AlgoType::Merge:
        return "Merge Sort:\n"
               "- Time: O(n log n)\n"
               "- Stable: Yes\n"
               "\nSteps:\n"
               "1. Split array into halves\n"
               "2. Recursively sort each half\n"
               "3. Merge sorted arrays";

    default:
        return "";
    }
}

// String to Int
std::vector<int> MainWindow::parseInput(QString text)
{
    std::vector<int> result;

    if (text.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Data is empty!");
        return result;
    }

    QStringList parts = text.split(" ", Qt::SkipEmptyParts);

    if (parts.size() < 2)
    {
        QMessageBox::warning(this, "Warning", "Need at least 2 elements!");
        return result;
    }

    for (auto& p : parts)
    {
        bool ok;
        int val = p.toInt(&ok);

        if (!ok)
        {
            QMessageBox::warning(this, "Warning", "Invalid number detected!");
            return std::vector<int>();
        }

        result.push_back(val);
    }

    return result;
}

// set name alg
void MainWindow::setAlgorithm(const QString& name)
{
        if (name == "Selection") currentAlgo = AlgoType::Selection;
        else if (name == "Bubble") currentAlgo = AlgoType::Bubble;
        else if (name == "Insertion") currentAlgo = AlgoType::Insertion;
        else if (name == "Merge") currentAlgo = AlgoType::Merge;
        titleLabel->setText(name);
}