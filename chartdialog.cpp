#include "chartdialog.h"
#include "qboxlayout.h"

// Конструктор класса ChartDialog
ChartDialog::ChartDialog(QVector<std::pair<int, float>>& data, QWidget *parent)
    : QDialog(parent) {
    // Создание серии столбцов
    QBarSeries *series = new QBarSeries();

    // Создание набора столбцов
    QBarSet *set = new QBarSet(tr("Data"));

    // Список категорий (меток) для оси X
    QStringList categories;
    for (const auto& pair : data) {
        *set << pair.second; // Добавление значений в набор столбцов
        categories << QString::number(pair.first); // Добавление меток категорий
    }

    // Добавление набора столбцов в серию
    series->append(set);

    // Создание графика
    chart = new QChart();
    chart->addSeries(series); // Добавление серии в график
    chart->setTitle(tr("Chart")); // Установка заголовка графика
    chart->setAnimationOptions(QChart::SeriesAnimations); // Установка анимации для серии

    // Создание оси категорий (X)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories); // Добавление категорий в ось X
    chart->addAxis(axisX, Qt::AlignBottom); // Добавление оси X в график и выравнивание по нижнему краю
    series->attachAxis(axisX); // Присоединение серии к оси X

    // Создание оси значений (Y)
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft); // Добавление оси Y в график и выравнивание по левому краю
    series->attachAxis(axisY); // Присоединение серии к оси Y

    // Создание виджета для отображения графика
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // Включение сглаживания для графика

    // Создание и настройка компоновки
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chartView); // Добавление виджета с графиком в компоновку

    setLayout(mainLayout); // Установка компоновки для диалогового окна
}
