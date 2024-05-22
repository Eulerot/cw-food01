#ifndef RESTAURANTCONTROLLER_H
#define RESTAURANTCONTROLLER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "restaurantmodel.h"

class RestaurantController : public QObject {
    Q_OBJECT

public:
    // Конструктор класса RestaurantController
    RestaurantController(RestaurantModel *model, QObject *parent = nullptr);

    // Функция для загрузки данных о ресторанах из файла
    void loadFromFile(const QString &filename);

    // Функция для сохранения данных о ресторанах в файл
    void saveToFile(const QString &filename);

private:
    RestaurantModel *m_model; // Модель данных о ресторанах
};

#endif // RESTAURANTCONTROLLER_H
