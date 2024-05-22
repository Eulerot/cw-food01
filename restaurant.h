#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <QString>

class Restaurant {
public:
    // Конструктор по умолчанию
    Restaurant();

    // Конструктор с параметрами
    Restaurant(const QString &name, const QString &owner, const QString &address, const QString &cuisine, int capital, int attendance, int avgbill, float rating);

    // Функции получения значений полей
    QString getName() const;
    QString getOwner() const;
    QString getAddress() const;
    QString getCuisine() const;
    int getCapital() const;
    int getAttendance() const;
    int getAvgBill() const;
    float getRating() const;

    // Функции установки значений полей
    void setName(const QString &name);
    void setOwner(const QString &owner);
    void setAddress(const QString &address);
    void setCuisine(const QString &cuisine);
    void setCapital(int capital);
    void setAttendance(int attendance);
    void setAvgBill(int avgbill);
    void setRating(float rating);

private:
    QString m_name;         // Название ресторана
    QString m_owner;        // Владелец ресторана
    QString m_address;      // Адрес ресторана
    QString m_cuisine;      // Кухня ресторана
    int m_capital;          // Капитал ресторана
    int m_attendance;       // Посещаемость ресторана
    int m_avgbill;          // Средний счет в ресторане
    float m_rating;         // Рейтинг ресторана
};

#endif // RESTAURANT_H
