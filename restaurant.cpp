#include "restaurant.h"

// Конструктор по умолчанию
Restaurant::Restaurant()
    : m_name(""), m_owner(""), m_address(""), m_cuisine(""), m_capital(0), m_attendance(0), m_avgbill(0), m_rating(0.0) {}

// Конструктор с параметрами
Restaurant::Restaurant(const QString &name, const QString &owner, const QString &address, const QString &cuisine, int capital, int attendance, int avgbill, float rating)
    : m_name(name), m_owner(owner),  m_address(address), m_cuisine(cuisine), m_capital(capital), m_attendance(attendance), m_avgbill(avgbill), m_rating(rating) {}

// Получение названия ресторана
QString Restaurant::getName() const { return m_name; }
// Получение владельца ресторана
QString Restaurant::getOwner() const { return m_owner; }
// Получение адреса ресторана
QString Restaurant::getAddress() const { return m_address; }
// Получение типа кухни ресторана
QString Restaurant::getCuisine() const { return m_cuisine; }
// Получение капитала ресторана
int Restaurant::getCapital() const { return m_capital; }
// Получение посещаемости ресторана
int Restaurant::getAttendance() const { return m_attendance; }
// Получение среднего счета ресторана
int Restaurant::getAvgBill() const { return m_avgbill; }
// Получение рейтинга ресторана
float Restaurant::getRating() const { return m_rating; }

// Установка названия ресторана
void Restaurant::setName(const QString &name) { m_name = name; }
// Установка владельца ресторана
void Restaurant::setOwner(const QString &owner) { m_owner = owner; }
// Установка адреса ресторана
void Restaurant::setAddress(const QString &address) { m_address = address; }
// Установка типа кухни ресторана
void Restaurant::setCuisine(const QString &cuisine) { m_cuisine = cuisine; }
// Установка капитала ресторана
void Restaurant::setCapital(int capital) { m_capital = capital; }
// Установка посещаемости ресторана
void Restaurant::setAttendance(int attendance) { m_attendance = attendance; }
// Установка среднего счета ресторана
void Restaurant::setAvgBill(int avgbill) { m_avgbill = avgbill; }
// Установка рейтинга ресторана
void Restaurant::setRating(float rating) { m_rating = rating; }
