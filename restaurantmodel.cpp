#include "restaurantmodel.h"
#include "QtPrintSupport/qprintdialog.h"
#include "QtPrintSupport/qprinter.h"
#include <restaurant.h>
#include <QMimeData>
#include <QDataStream>
#include <QIODevice>
#include <QPainter>
#include <QApplication>

// Конструктор класса RestaurantModel
RestaurantModel::RestaurantModel(QObject *parent)
    : QAbstractTableModel(parent) {}

// Возвращает количество строк в модели
int RestaurantModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_restaurants.count();
}

// Возвращает количество столбцов в модели
int RestaurantModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 8; // Name, Address, Cuisine, Rating
}

// Проверяет допустимость значения для заданного столбца
bool RestaurantModel::isValidValue(int column, const QVariant &value) const {
    switch (column) {
    case 5: // Attendance
        return value.toInt() <= 100;
    case 7: // Rating
        return value.toFloat() <= 5.0;
    default:
        return true;
    }
}

// Возвращает данные для отображения в заданной модели и роли
QVariant RestaurantModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
        return QVariant();

    const Restaurant &restaurant = m_restaurants[index.row()];

    switch (index.column()) {
    case 0: return restaurant.getName();
    case 1: return restaurant.getOwner();
    case 2: return restaurant.getAddress();
    case 3: return restaurant.getCuisine();
    case 4: return restaurant.getCapital();
    case 5: return restaurant.getAttendance();
    case 6: return restaurant.getAvgBill();
    case 7: return round(restaurant.getRating()*10)/10;
    default: return QVariant();
    }
}

// Возвращает данные заголовка для указанной ориентации и роли
QVariant RestaurantModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
    case 0: return tr("Name");
    case 1: return tr("Owner");
    case 2: return tr("Address");
    case 3: return tr("Cuisine");
    case 4: return tr("Capital");
    case 5: return tr("Attendance");
    case 6: return tr("AvgBill");
    case 7: return tr("Rating");
    default: return QVariant();
    }
}

// Устанавливает данные для указанного индекса и роли
bool RestaurantModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!isValidValue(index.column(), value)) {
        return false; // Значение не проходит проверку
    }
    if (index.isValid() && role == Qt::EditRole) {
        Restaurant &restaurant = m_restaurants[index.row()];
        switch (index.column()) {
        case 0: restaurant.setName(value.toString()); break;
        case 1: restaurant.setOwner(value.toString()); break;
        case 2: restaurant.setAddress(value.toString()); break;
        case 3: restaurant.setCuisine(value.toString()); break;
        case 4: restaurant.setCapital(value.toInt()); break;
        case 5: restaurant.setAttendance(value.toInt()); break;
        case 6: restaurant.setAvgBill(value.toInt()); break;
        case 7: restaurant.setRating(value.toFloat()); break;
        default: return false;
        }
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

// Возвращает флаги элементов для указанного индекса
Qt::ItemFlags RestaurantModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
    return Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
}

// Вставляет строки в модель
bool RestaurantModel::insertRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_restaurants.insert(row, Restaurant("", "", "", "", 0, 0, 0, 0.0f));
    }
    endInsertRows();
    return true;
}

// Добавляет ресторан в модель
void RestaurantModel::addRestaurant(const Restaurant &restaurant) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_restaurants.append(restaurant);
    endInsertRows();
}

// Удаляет ресторан из модели
void RestaurantModel::removeRestaurant(int row) {
    if (row < 0 || row >= m_restaurants.size()) return;
    beginRemoveRows(QModelIndex(), row, row);
    m_restaurants.removeAt(row);
    endRemoveRows();
}

// Удаляет строки из модели
bool RestaurantModel::removeRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    if (row < 0 || row + count > m_restaurants.size()) return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_restaurants.removeAt(row);
    }
    endRemoveRows();
    return true;
}

// Очищает модель
void RestaurantModel::clear() {
    removeRows(0, rowCount());
}

// Сортирует строки в модели по указанному столбцу и порядку
void RestaurantModel::sort(int column, Qt::SortOrder order) {
    std::sort(m_restaurants.begin(), m_restaurants.end(), [column, order](const Restaurant &a, const Restaurant &b) {
        switch (column) {
        case 0: return order == Qt::AscendingOrder ? a.getName() < b.getName() : a.getName() > b.getName();
        case 1: return order == Qt::AscendingOrder ? a.getOwner() < b.getOwner() : a.getOwner() > b.getOwner();
        case 2: return order == Qt::AscendingOrder ? a.getAddress() < b.getAddress() : a.getAddress() > b.getAddress();
        case 3: return order == Qt::AscendingOrder ? a.getCuisine() < b.getCuisine() : a.getCuisine() > b.getCuisine();
        case 4: return order == Qt::AscendingOrder ? a.getCapital() < b.getCapital() : a.getCapital() > b.getCapital();
        case 5: return order == Qt::AscendingOrder ? a.getAttendance() < b.getAttendance() : a.getAttendance() > b.getAttendance();
        case 6: return order == Qt::AscendingOrder ? a.getAvgBill() < b.getAvgBill() : a.getAvgBill() > b.getAvgBill();
        case 7: return order == Qt::AscendingOrder ? a.getRating() < b.getRating() : a.getRating() > b.getRating();
        default: return false;
        }
    });
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

// Возвращает список поддерживаемых типов MIME
QStringList RestaurantModel::mimeTypes() const {
    return { "application/vnd.text.list", "text/plain" };
}

// Создает и возвращает MIME-данные для перетаскиваемых элементов
QMimeData *RestaurantModel::mimeData(const QModelIndexList &indexes) const {
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    QString plainText;
    QSet<int> rows, columns;

    for (const QModelIndex &index : indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << index.row() << index.column() << text;
            plainText.append(text + ";");
            rows.insert(index.row());
            columns.insert(index.column());
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    mimeData->setText(plainText.trimmed()+"\n"); // для перетаскивания в другие программы
    mimeData->setData("rows", QByteArray::number(rows.size())); // кол-во строк
    mimeData->setData("columns", QByteArray::number(columns.size())); // кол-во столбцов

    return mimeData;
}

// Обрабатывает перетаскиваемые MIME-данные и вставляет их в модель
bool RestaurantModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    Q_UNUSED(action);

    if (!data->hasFormat("application/vnd.text.list"))
        return false;

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    int numRows = data->data("rows").toInt();
    int numColumns = data->data("columns").toInt();

    QList<QPair<int, int>> sourceIndices;
    QList<QString> texts;
    while (!stream.atEnd()) {
        int sourceRow, sourceColumn;
        QString text;
        stream >> sourceRow >> sourceColumn >> text;
        sourceIndices.append(qMakePair(sourceRow, sourceColumn));
        texts.append(text);
    }

    if (sourceIndices.isEmpty()) {
        return false;
    }

    int targetRow = row >= 0 ? row : parent.row();
    int targetColumn = column >= 0 ? column : parent.column();

    if (targetRow < 0 || targetColumn < 0) {
        return false;
    }

    // Ensure there are enough rows and columns
    while (targetRow + numRows > rowCount()) {
        insertRows(rowCount(), 1);
    }

    for (int i = 0; i < texts.size(); ++i) {
        int destRow = targetRow + (i / numColumns);
        int destColumn = targetColumn + (i % numColumns);
        setData(index(destRow, destColumn), texts.at(i), Qt::EditRole);
    }

    return true;
}

// Возвращает действия перетаскивания, поддерживаемые моделью
Qt::DropActions RestaurantModel::supportedDropActions() const {
    return Qt::CopyAction | Qt::MoveAction;
}

// Перемещает строки в модели
void RestaurantModel::moveRows(int sourceRow, int count, int destinationRow) {
    if (sourceRow < 0 || sourceRow >= m_restaurants.size() || count <= 0 || sourceRow + count > m_restaurants.size()) {
        return;
    }

    if (destinationRow < 0 || destinationRow >= m_restaurants.size()) {
        return;
    }

    beginMoveRows(QModelIndex(), sourceRow, sourceRow + count - 1, QModelIndex(), destinationRow);

    QVector<Restaurant> temp;
    for (int i = 0; i < count; ++i) {
        temp.append(m_restaurants.takeAt(sourceRow));
    }

    for (int i = 0; i < count; ++i) {
        m_restaurants.insert(destinationRow > sourceRow ? destinationRow - count + i : destinationRow + i, temp.at(i));
    }

    endMoveRows();
}

// Печатает содержимое модели
void RestaurantModel::printOutput() {
    QPrinter printer;
    QPrintDialog dialog(&printer);

    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);

        int rows = rowCount();
        int columns = columnCount();
        int margin = 50;
        int rowHeight = 30;

        QFontMetrics fontMetrics(painter.font());

        // Вычисление максимальной ширины для каждого столбца
        QVector<int> columnWidths(columns);
        for (int col = 0; col < columns; ++col) {
            QString headerText = headerData(col, Qt::Horizontal, Qt::DisplayRole).toString();
            int maxWidth = fontMetrics.horizontalAdvance(headerText) + 20; // horizontalAdvance вместо width
            for (int row = 0; row < rows; ++row) {
                QString cellText = data(index(row, col), Qt::DisplayRole).toString();
                maxWidth = qMax(maxWidth, fontMetrics.horizontalAdvance(cellText) + 20); // horizontalAdvance вместо width
            }
            columnWidths[col] = maxWidth;
        }

        // Печать заголовков
        for (int col = 0; col < columns; ++col) {
            QString headerText = headerData(col, Qt::Horizontal, Qt::DisplayRole).toString();

            // Определение прямоугольника для текста внутри ячейки
            QRect rect(margin + std::accumulate(columnWidths.begin(), columnWidths.begin() + col, 0),
                       margin,
                       columnWidths[col],
                       rowHeight);
            painter.drawRect(rect);

            // Определение текста, который помещается в ячейке
            QString clippedText = fontMetrics.elidedText(headerText, Qt::ElideRight, rect.width() - 10); // Учитываем отступы

            // Выравнивание и отображение текста по центру
            painter.drawText(rect, Qt::AlignCenter, clippedText);
        }

        // Печать данных
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < columns; ++col) {
                QString cellText = data(index(row, col), Qt::DisplayRole).toString();

                // Определение прямоугольника для текста внутри ячейки
                QRect rect(margin + std::accumulate(columnWidths.begin(), columnWidths.begin() + col, 0),
                           margin + (row + 1) * rowHeight,
                           columnWidths[col],
                           rowHeight);
                painter.drawRect(rect);

                // Определение текста, который помещается в ячейке
                QString clippedText = fontMetrics.elidedText(cellText, Qt::ElideRight, rect.width() - 10); // Учитываем отступы

                // Выравнивание и отображение текста по центру
                painter.drawText(rect, Qt::AlignCenter, clippedText);
            }
        }

        painter.end();
    }
}
