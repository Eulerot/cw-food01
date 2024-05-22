#ifndef RESTAURANTMODEL_H
#define RESTAURANTMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "qwidget.h"
#include "restaurant.h"

class RestaurantModel : public QAbstractTableModel {
    Q_OBJECT

public:
    // Конструктор класса RestaurantModel
    explicit RestaurantModel(QObject *parent = nullptr);

    // Возвращает количество строк в модели
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // Возвращает количество столбцов в модели
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // Возвращает данные для отображения в заданной модели и роли
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // Возвращает данные заголовка для указанной ориентации и роли
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // Устанавливает данные для указанного индекса и роли
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    // Возвращает флаги элементов для указанного индекса
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    // Вставляет строки в модель
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // Удаляет строки из модели
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // Проверяет допустимость значения для заданного столбца
    bool isValidValue(int column, const QVariant &value) const;

    // Добавляет ресторан в модель
    void addRestaurant(const Restaurant &restaurant);
    // Удаляет ресторан из модели
    void removeRestaurant(int row);
    // Очищает модель
    void clear();
    // Сортирует строки в модели по указанному столбцу и порядку
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    // Печатает содержимое модели
    void printOutput();

    // Возвращает список поддерживаемых типов MIME
    QStringList mimeTypes() const override;
    // Создает и возвращает MIME-данные для перетаскиваемых элементов
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    // Обрабатывает перетаскиваемые MIME-данные и вставляет их в модель
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    // Возвращает действия перетаскивания, поддерживаемые моделью
    Qt::DropActions supportedDropActions() const override;
    // Перемещает строки в модели
    void moveRows(int sourceRow, int count, int destinationRow);

    QVector<Restaurant> m_restaurants;

private:
    // Перемещает строку в модели
    void moveRow(int sourceRow, int destinationRow);
};

#endif // RESTAURANTMODEL_H
