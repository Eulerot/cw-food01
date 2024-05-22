#include "restaurantcontroller.h"

// Конструктор класса RestaurantController
RestaurantController::RestaurantController(RestaurantModel *model, QObject *parent)
    : QObject(parent), m_model(model) {}

// Функция загрузки данных из файла
void RestaurantController::loadFromFile(const QString &filename) {
    QFile file(filename); // Создание объекта файла
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return; // Открытие файла для чтения

    QTextStream in(&file); // Создание потока для чтения из файла
    m_model->clear(); // Очистка модели

    while (!in.atEnd()) { // Чтение данных до конца файла
        QString line = in.readLine(); // Чтение строки
        QStringList fields = line.split(";"); // Разделение строки на поля
        if (fields.size() == 8) { // Проверка количества полей
            // Создание объекта Restaurant и добавление его в модель
            Restaurant restaurant(fields[0], fields[1], fields[2], fields[3], fields[4].toInt(), fields[5].toInt(), fields[6].toInt(), fields[7].toFloat());
            m_model->addRestaurant(restaurant);
        }
    }
}

// Функция сохранения данных в файл
void RestaurantController::saveToFile(const QString &filename) {
    QFile file(filename); // Создание объекта файла
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return; // Открытие файла для записи

    QTextStream out(&file); // Создание потока для записи в файл
    for (int row = 0; row < m_model->rowCount(); ++row) { // Проход по всем строкам модели
        // Получение ресторана из модели и запись его данных в файл
        Restaurant restaurant = m_model->m_restaurants.at(row);
        out << restaurant.getName() << ";"
            << restaurant.getOwner() << ";"
            << restaurant.getAddress() << ";"
            << restaurant.getCuisine() << ";"
            << restaurant.getCapital() << ";"
            << restaurant.getAttendance() << ";"
            << restaurant.getAvgBill() << ";"
            << restaurant.getRating() << "\n";
    }
}
