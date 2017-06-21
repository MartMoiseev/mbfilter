#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog.h"
#include "QFile.h"

#include <QByteArray>
#include <QDebug.h>
#include <QSplitter>
#include <QLine>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>

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
    _splitter->setChildrenCollapsible(false);

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
        this->_data->load(fileName);
        this->setWindowTitle(fileName.split("/").back());

        _ui->statusBar->showMessage("Файл \"" + fileName.split("/").back() + "\" успешно загружен!", 7000);

        if (_splitter) {
            delete _splitter;

            _splitter = new QSplitter();
            _splitter->setOrientation(Qt::Vertical);
            _splitter->setChildrenCollapsible(false);

            _ui->canalLayout->addWidget(_splitter);
        }

        int max = this->_data->getCount();
        for (int c = 0; c < max; c++) {
            this->addLayout(this->_data->get(c));
        }
    }
}

/**
 * @brief MainWindow::addLayout
 */
void MainWindow::addLayout(Canal* canal)
{
    QSizePolicy expand(QSizePolicy::Expanding, QSizePolicy::Expanding);
    expand.setHorizontalStretch(1);
    expand.setVerticalStretch(1);

    QLabel* label = new QLabel;
    label->setText(canal->getName());

    QSpinBox* gate = new QSpinBox(this);
    gate->setSuffix(" ед");
    gate->setPrefix("Gate: ");
    gate->setMinimum(1);
    gate->setMaximum(1000);
    gate->setValue(50);
    gate->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

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

    QPushButton* filter = new QPushButton(this);
    filter->setText("Отфильтровать");
    filter->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    QSlider* zoomx = new QSlider(this);
    zoomx->setMinimum(1);
    zoomx->setMaximum(15);
    zoomx->setValue(1);
    zoomx->setOrientation(Qt::Horizontal);
    zoomx->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(label);
    leftLayout->addWidget(gate);
    leftLayout->addWidget(preview);
    leftLayout->addWidget(cutout);
    leftLayout->addWidget(filter);
    leftLayout->addWidget(zoomx);
    leftLayout->addStretch();

    ChartView* chart = new ChartView(this);
    chart->setSizePolicy(expand);
    chart->setCanal(canal);
    chart->setFixedHeight(250);
    chart->renderData();

    QSlider* zoomy = new QSlider(this);
    zoomy->setMinimum(1);
    zoomy->setMaximum(20);
    zoomy->setValue(10);
    zoomy->setInvertedAppearance(true);
    zoomy->setTickPosition(QSlider::TicksBothSides);
    zoomy->setTickInterval(9);

    QHBoxLayout *rightLayout = new QHBoxLayout;
    rightLayout->addWidget(chart);
    rightLayout->addWidget(zoomy);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    QWidget *canalWidget = new QWidget;
    canalWidget->setLayout(mainLayout);

    _splitter->addWidget(canalWidget);

    // предварительный просмотр
    connect(preview, SIGNAL(clicked(bool)), chart, SLOT(preview()));
    // изменение параметров фильтрации
    connect(gate, SIGNAL(valueChanged(int)), chart, SLOT(setGate(int)));
    connect(cutout, SIGNAL(valueChanged(int)), chart, SLOT(setCutout(int)));
    // изменение размеров
    connect(zoomx, SIGNAL(valueChanged(int)), chart, SLOT(zoomx(int)));
    connect(zoomy, SIGNAL(valueChanged(int)), chart, SLOT(zoomy(int)));
    // фильтрация
    connect(filter, SIGNAL(clicked(bool)), chart, SLOT(filter()));
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
        this->_data->save(fileName);
    }
}





void MainWindow::on_gate_1_valueChanged(int arg1)
{
    //_ui->chart_1->filter(arg1);
}

void MainWindow::on_preview_1_clicked()
{
    //_ui->chart_1->filter(_ui->gate_1->value());
}

void MainWindow::on_filter_1_clicked()
{
    //_ui->chart_1->cut(_ui->gate_1->value(), _ui->cutout_1->value());
}

void MainWindow::on_zoomx_1_valueChanged(int value)
{
    //_ui->chart_1->setZoom(value, 0);
}

void MainWindow::on_zoomy_1_valueChanged(int value)
{
    //_ui->chart_1->setZoom(0, value / 10.0);
}
