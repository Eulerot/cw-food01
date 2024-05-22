#include "restaurantdetailswindow.h"
#include <QVBoxLayout>
#include <QLabel>

// Конструктор класса RestaurantDetailsWindow
RestaurantDetailsWindow::RestaurantDetailsWindow(const Restaurant &restaurant, QWidget *parent)
    : QWidget(parent),
    // Инициализация меток с информацией о ресторане
    m_nameLabel(new QLabel("Name: " + restaurant.getName(), this)),
    m_ownerLabel(new QLabel("Owner: " + restaurant.getOwner(), this)),
    m_addressLabel(new QLabel("Address: " + restaurant.getAddress(), this)),
    m_cuisineLabel(new QLabel("Cuisine: " + restaurant.getCuisine(), this)),
    m_capitalLabel(new QLabel("Capital: " +  QString::number(restaurant.getCapital()), this)),
    m_attendanceLabel(new QLabel("Attendance: " + QString::number(restaurant.getAttendance()), this)),
    m_avgbillLabel(new QLabel("AvgBill: " + QString::number(restaurant.getAvgBill()), this)),
    m_ratingLabel(new QLabel("Rating: " + QString::number(restaurant.getRating()), this)) {

    // Создание и настройка макета
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_nameLabel);      // Добавление метки имени
    layout->addWidget(m_ownerLabel);     // Добавление метки владельца
    layout->addWidget(m_addressLabel);   // Добавление метки адреса
    layout->addWidget(m_cuisineLabel);   // Добавление метки кухни
    layout->addWidget(m_capitalLabel);   // Добавление метки капитала
    layout->addWidget(m_attendanceLabel);// Добавление метки посещаемости
    layout->addWidget(m_avgbillLabel);   // Добавление метки среднего счета
    layout->addWidget(m_ratingLabel);    // Добавление метки рейтинга
    setLayout(layout);                   // Установка макета на окно
    setWindowTitle("Restaurant Details");// Установка заголовка окна
}
