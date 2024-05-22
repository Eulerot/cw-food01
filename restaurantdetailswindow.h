#ifndef RESTAURANTDETAILSWINDOW_H
#define RESTAURANTDETAILSWINDOW_H

#include <QWidget>
#include "restaurant.h"

class QLabel;

class RestaurantDetailsWindow : public QWidget {
    Q_OBJECT

public:
    // Конструктор класса RestaurantDetailsWindow
    explicit RestaurantDetailsWindow(const Restaurant &restaurant, QWidget *parent = nullptr);

private:
    QLabel *m_nameLabel; // Метка для отображения названия ресторана
    QLabel *m_ownerLabel; // Метка для отображения владельца ресторана
    QLabel *m_addressLabel; // Метка для отображения адреса ресторана
    QLabel *m_cuisineLabel; // Метка для отображения кухни ресторана
    QLabel *m_capitalLabel; // Метка для отображения капитала ресторана
    QLabel *m_attendanceLabel; // Метка для отображения посещаемости ресторана
    QLabel *m_avgbillLabel; // Метка для отображения среднего счета в ресторане
    QLabel *m_ratingLabel; // Метка для отображения рейтинга ресторана
};

#endif // RESTAURANTDETAILSWINDOW_H
