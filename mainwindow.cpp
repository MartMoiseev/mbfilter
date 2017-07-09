#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog.h"
#include "QFile.h"
#include "chartview.h"

#include <QByteArray>
#include <QDebug.h>
#include <QSplitter>
#include <QLine>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _data = new Data();

    _splitter = new QSplitter();
    _splitter->setOrientation(Qt::Vertical);
    //_splitter->setChildrenCollapsible(false);

    _ui->canalLayout->addWidget(_splitter);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete _data;
    delete _ui;
}

/**
 * @brief MainWindow::on_actionOpen_triggered
 */
void MainWindow::on_actionOpen_triggered()
{
    // Открываем диалоговое окно
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Открыть"),
        0,
        tr("Все (*.asc *.txt);;ASC-файлы (*.asc);;Текстовые файлы (*.txt)")
    );

    // Продолжаем, если файл был выбран
    if(!fileName.isEmpty())
    {
        // Загружаем файл
        this->_data->loadFromFile(fileName);
        this->setWindowTitle(fileName.split("/").back());

        _ui->statusBar->showMessage("Файл \"" + fileName.split("/").back() + "\" успешно загружен!", 7000);

        if (_splitter) {
            delete _splitter;

            _splitter = new QSplitter();
            _splitter->setOrientation(Qt::Vertical);
            _splitter->setChildrenCollapsible(false);

            _ui->canalLayout->addWidget(_splitter);
        }

        int max = this->_data->countCanal();
        for (int c = 0; c < max; c++) {
            this->addLayout(c, this->_data->getCanal(c));
        }

        QMessageBox::information(this, "Загрузка завершена", "Файл " + fileName + " успешно загружен.");
    }
}

/**
 * @brief MainWindow::addLayout
 */
void MainWindow::addLayout(int id, Canal* canal)
{
    QSizePolicy expand(QSizePolicy::Expanding, QSizePolicy::Expanding);
    expand.setHorizontalStretch(1);
    expand.setVerticalStretch(1);

    QLabel* label = new QLabel;
    label->setText(canal->getName());

    QPushButton* undo = new QPushButton(this);
    undo->setText("Отменить");
    undo->setEnabled(false);

    QSpinBox* gate = new QSpinBox(this);
    gate->setSuffix(" ед");
    gate->setPrefix("Gate: ");
    gate->setMinimum(1);
    gate->setMaximum(1000);
    gate->setValue(50);
    gate->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    gate->setSingleStep(5);

    QPushButton* preview = new QPushButton(this);
    preview->setText("Просмотр");
    preview->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    QSpinBox* cutout = new QSpinBox(this);
    cutout->setSuffix(" шт");
    cutout->setPrefix("Cut: ");
    cutout->setMinimum(2);
    cutout->setMaximum(100);
    cutout->setValue(10);
    cutout->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    gate->setSingleStep(5);

    QPushButton* filter = new QPushButton("Отфильтровать", this);
    filter->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    QCheckBox* bypass = new QCheckBox("Оригинал", this);
    bypass->setChecked(true);

    QSpacerItem* spacer = new QSpacerItem(100, 100);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(label);
    leftLayout->addWidget(gate);
    leftLayout->addWidget(preview);
    leftLayout->addWidget(cutout);
    leftLayout->addWidget(filter);
    leftLayout->addWidget(bypass);
    leftLayout->addSpacerItem(spacer);
    leftLayout->addWidget(undo);

    ChartView* plot = new ChartView(id);

    plot->setCanal(canal);
    plot->setSizePolicy(expand);
    plot->setFixedHeight(256);
    plot->setInteraction(QCP::iRangeDrag, true);
    plot->xAxis->setRange(0, canal->length());
    plot->setInteraction(QCP::iRangeZoom, true);

    //plot->xAxis->setLabel("msec");
    //plot->yAxis->setLabel("mkV");

    plot->xAxis->setRange(0, canal->length());
    plot->yAxis->setRange(-500, 500);

    plot->replot();

    QHBoxLayout *rightLayout = new QHBoxLayout;
    rightLayout->addWidget(plot);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    QWidget *canalWidget = new QWidget;
    canalWidget->setLayout(mainLayout);

    _splitter->addWidget(canalWidget);

    // предварительный просмотр
    connect(preview, SIGNAL(clicked(bool)), plot, SLOT(preview()));
    connect(preview, SIGNAL(clicked(bool)), this, SLOT(previewFinished()));

    connect(bypass, SIGNAL(clicked(bool)), plot, SLOT(setBypass(bool)));
    connect(plot, SIGNAL(changeBypass(bool)), bypass, SLOT(setChecked(bool)));

    connect(undo, SIGNAL(clicked(bool)), plot, SLOT(undo()));
    connect(plot, SIGNAL(undoDisabled(bool)), undo, SLOT(setDisabled(bool)));

    // изменение параметров фильтрации
    connect(gate, SIGNAL(valueChanged(int)), plot, SLOT(setGate(int)));
    connect(cutout, SIGNAL(valueChanged(int)), plot, SLOT(setCutout(int)));

    // фильтрация
    connect(filter, SIGNAL(clicked(bool)), plot, SLOT(filter()));

    connect(plot, SIGNAL(changeChanal(int,Canal*)), this->_data, SLOT(setCanal(int,Canal*)));
}



/**
 * @brief MainWindow::on_actionSave_triggered
 */
void MainWindow::on_actionSave_triggered()
{
    // Открываем диалоговое окно
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Сохранить"),
        0,
        tr("ASC-файлы (*.asc)")
    );

    if (!fileName.isEmpty()) {
        if (this->_data->saveToFile(fileName)) {
            QMessageBox::information(this, "Сохранение завершено", "Файл " + fileName + " успешно сохранен.");
        } else {
            QMessageBox::warning(this, "Ошибка сохранения файла", "При сохранении файла " + fileName + " произошла ошибка. Попробуйте сохранить в другую директорию.");
        }
    }
}



/**
 * @brief MainWindow::showMessage
 * @param string
 */
void MainWindow::previewFinished()
{
    _ui->statusBar->showMessage("Поиск пиков завершен!", 3000);
}
