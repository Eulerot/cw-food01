#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>

/**
 * @brief The ChartDialog class represents a dialog window for displaying a chart.
 */
class ChartDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs a ChartDialog object.
     * @param data The data to be displayed in the chart.
     * @param parent The parent widget (default is nullptr).
     */
    ChartDialog(QVector<std::pair<int, float>>& data, QWidget *parent = nullptr);

private:
    QChart *chart; ///< The chart object.
    QChartView *chartView; ///< The chart view object.
};

#endif // CHARTDIALOG_H
