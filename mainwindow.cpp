#include "mainwindow.h"
#include "restaurantmodel.h"
#include "restaurantdetailswindow.h"
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>
#include <QHeaderView>
#include <QClipboard>
#include <QSettings>
#include <QLibraryInfo>
#include "chartdialog.h"
#include <QCloseEvent>

// Инициализация статической переменной
int MainWindow::numfile = 0;

// Конструктор класса MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),\
    m_tableView(new QTableView(this)),
    m_searchBar(new QLineEdit(this)),
    m_model(new RestaurantModel(this)),
    m_controller(new RestaurantController(m_model, this)),
    languageMenu(new QMenu(tr("Language"), this)),
    languageActionGroup(new QActionGroup(this))  // Инициализация меню выбора языка
{
    qApp->installTranslator(&appTranslator); // Установка переводчика для приложения
    qApp->installTranslator(&qtTranslator); // Установка переводчика для Qt
    qmPath = "../cw-food01/translations"; // Установка пути к переводам


    setupMenuBar(); // Настройка строки меню
    setupTableView(); // Настройка таблицы
    setupSearchBar(); // Настройка строки поиска
    setCentralWidget(m_tableView); // Установка центрального виджета

    createLanguageMenu(); // Создание меню выбора языка
    connect(this, &MainWindow::languageChanged, this, &MainWindow::updateLanguageAction); // Связь смены языка с обновлением действий
    setSettings(); // Установка настроек
    m_tableView->setEnabled(false); // Отключение таблицы

}

// Деструктор класса MainWindow
MainWindow::~MainWindow() {
    getSettings(); // Сохранение настроек при закрытии
    delete m_tableView;
    delete m_searchBar;
    delete m_model;
    delete m_controller;
    delete languageMenu;
    delete languageActionGroup;
}

// Обработчик события закрытия окна
void MainWindow::closeEvent(QCloseEvent *event) {
    if (!filename.isEmpty()) { // Проверка, если файл не пустой
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Save"),
                                      tr("Save?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {
            onSaveFile(); // Сохранение файла
            event->accept(); // Продолжаем закрытие окна
        } else if (reply == QMessageBox::No) {
            event->accept(); // Продолжаем закрытие окна
        } else {
            event->ignore(); // Игнорируем событие закрытия
        }
    } else {
        event->accept(); // Продолжаем закрытие окна, если нет несохранённых изменений
    }
}

// Сохранение настроек
void MainWindow::getSettings() {
    QSettings settings("Settings", "MySettings");

    settings.beginGroup("MainWindowGeometry");
    settings.setValue("size", size());
    settings.setValue("position", pos());
    settings.setValue("language", languageActionGroup->checkedAction()->data().toString());
    settings.endGroup();
}

// Загрузка настроек
void MainWindow::setSettings() {
    QSettings settings("Settings", "MySettings");

    settings.beginGroup("MainWindowGeometry");
    resize(settings.value("size").toSize());
    move(settings.value("position").toPoint());

    QString savedLanguage = settings.value("language").toString();

    foreach(QAction *action, languageActionGroup->actions()) {
        if (action->data().toString() == savedLanguage) {
            action->setChecked(true);
            changeLanguage(action->data().toString());
            break;
        }
    }

    settings.endGroup();
}

// Получение текущей локали
QString MainWindow::getCurrentLocale() const {
    return currentLocale;
}

// Создание меню выбора языка
void MainWindow::createLanguageMenu() {
    languageActionGroup = new QActionGroup(this); // Создание группы действий для языков

    // Установка связи пунктов меню со слотом смены языка приложения
    connect(languageActionGroup, &QActionGroup::triggered, this, [=](QAction *action) {
        changeLanguage(action->data().toString());
    });

    QDir dir(qmPath); // Определяем каталог, где лежат файлы переводов

    QStringList fileNames = dir.entryList(QStringList("cw-food01_*.qm")); // Получение списка файлов переводов
    qDebug() << qmPath;
    for (int i = 0; i < fileNames.size(); i++) {
        // Получаем i-ую локаль
        QString locale = fileNames[i];
        // Удаляем символы в строке сначала и по символ "_" включительно
        locale.remove(0, locale.indexOf('_') + 1);
        // Удаляем символы в строке начиная с символа точки
        locale.truncate(locale.lastIndexOf('.'));
        // Создаём временную переменную перевода для языкового пункта меню
        QTranslator translator;
        // Загружаем перевод из файла перевода "*.qm"
        translator.load(fileNames[i], qmPath);

        QString language = translator.translate("MainWindow", "Language");
        if (locale == "en_US") language = "English";
        else if (locale == "fr_FR") language = "Français";
        else if (locale == "ru_RU") language = "Русский";
        QAction *action = new QAction(language, this);
        action->setCheckable(true);
        // Задаём внутренние данные хранимые в i-ом пункте.
        // В нашем случае это локаль вида "ru_RU"
        action->setData(locale);
        // Добавляем i-ый пункт в меню на нашей форме "mainwindow.ui"
        languageMenu->addAction(action);
        // Добавляем i-ый пункт в единую группу пунктов
        languageActionGroup->addAction(action);
        // Задаём английский язык в качестве выбранного по умолчанию
        if (currentLocale == locale || (locale == "en_US" && currentLocale.isEmpty())) {
            action->setChecked(true);
            changeLanguage(locale); // Вызываем сразу, чтобы обновить перевод
        }
    }
}

// Смена языка
void MainWindow::changeLanguage(const QString &locale) {
    if (currentLocale == locale) return; // Если выбран тот же язык, ничего не делаем

    appTranslator.load("cw-food01_" + locale, qmPath); // Загрузка перевода приложения
    QString choppedLocale = locale;
    choppedLocale.chop(3);
    qtTranslator.load("qt_" + choppedLocale + ".qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);

    retranslateUi(); // Перевод интерфейса
    currentLocale = locale;
    emit languageChanged(locale); // Отправляем сигнал о смене языка

    updateLanguageAction(locale); // Обновляем галочки в меню
}

// Обновление действий языка
void MainWindow::updateLanguageAction(const QString &locale) {
    foreach (QAction *action, languageActionGroup->actions()) {
        if (action->data().toString() == locale) {
            action->setChecked(true);
        } else {
            action->setChecked(false);
        }
    }
}

// Перевод интерфейса пользователя
void MainWindow::retranslateUi() {
    menuBar()->clear();
    setupMenuBar(); // Настройка строки меню
    setupSearchBar(); // Настройка строки поиска
    m_tableView->setModel(m_model); // Установка модели таблицы
}

// Настройка строки меню
void MainWindow::setupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));

    fileMenu->addAction(tr("Open"), this, &MainWindow::onOpenFile);

    saveAction = fileMenu->addAction(tr("Save"), this, &MainWindow::onSaveFile);
    saveAsAction = fileMenu->addAction(tr("Save As"), this, &MainWindow::onSaveAsFile);
    fileMenu->addAction(tr("New"), this, &MainWindow::onNewFile);
    fileMenu->addAction(tr("New Window"), this, &MainWindow::onNewFileWindow);
    printAction = fileMenu->addAction(tr("Print"), this, &MainWindow::onPrint);
    fileMenu->addAction(tr("Close"), this, &MainWindow::close);
    fileMenu->addAction(tr("Quit"), qApp, &QApplication::quit);

    QMenu *editMenu = menuBar()->addMenu(tr("Edit"));

    addRestaurantAction = editMenu->addAction(tr("Add"), this, &MainWindow::onAddRestaurant);
    deleteRestaurantAction = editMenu->addAction(tr("Delete"), this, &MainWindow::onDeleteRestaurant);
    copyAction = editMenu->addAction(tr("Copy"), this, &MainWindow::onCopy);
    pasteAction = editMenu->addAction(tr("Paste"), this, &MainWindow::onPaste);
    cutAction = editMenu->addAction(tr("Cut"), this, &MainWindow::onCut);

    showChartAction = new QAction(tr("Show Chart"), this);
    connect(showChartAction, &QAction::triggered, this, &MainWindow::showChart);
    menuBar()->addMenu(languageMenu);

    menuBar()->addAction(showChartAction);
    QMenu *aboutMenu = menuBar()->addMenu(tr("About"));
    aboutMenu->addAction(tr("About me"), this, &MainWindow::onAboutMe);
    aboutMenu->addAction(tr("About QT"), this, &MainWindow::onAboutQT);

    // Отключение действий по умолчанию
    saveAction->setEnabled(false);
    saveAsAction->setEnabled(false);
    printAction->setEnabled(false);
    addRestaurantAction->setEnabled(false);
    deleteRestaurantAction->setEnabled(false);
    copyAction->setEnabled(false);
    pasteAction->setEnabled(false);
    cutAction->setEnabled(false);
    showChartAction->setEnabled(false);

}

// Обновление действий
void MainWindow::updateActtions(){
    saveAction->setEnabled(true);
    saveAsAction->setEnabled(true);
    printAction->setEnabled(true);
    addRestaurantAction->setEnabled(true);
    deleteRestaurantAction->setEnabled(true);
    copyAction->setEnabled(true);
    pasteAction->setEnabled(true);
    cutAction->setEnabled(true);
    m_tableView->setEnabled(true);
    showChartAction->setEnabled(true);

}

// Настройка таблицы
void MainWindow::setupTableView() {
    m_tableView->setModel(m_model); // Установка модели для таблицы
    m_tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_tableView->setDragEnabled(true);
    m_tableView->setAcceptDrops(true);
    m_tableView->setDropIndicatorShown(true);
    m_tableView->setDragDropMode(QAbstractItemView::InternalMove);
    connect(m_tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, [this](int column){
        m_model->sort(column, Qt::AscendingOrder);
    });
    connect(m_tableView, &QTableView::clicked, this, &MainWindow::onCellClicked);
    connect(m_tableView, &QTableView::doubleClicked, this, &MainWindow::onShowDetails);
    connect(m_tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onColumnHeaderClicked);
    m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tableView, &QTableView::customContextMenuRequested, this, &MainWindow::showContextMenu);
}

// Настройка строки поиска
void MainWindow::setupSearchBar() {
    m_searchBar->setPlaceholderText(tr("Search..."));
    menuBar()->setCornerWidget(m_searchBar, Qt::TopRightCorner);
    connect(m_searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearch);
}

// Контекстное меню
void MainWindow::showContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context Menu"), this);
    add = contextMenu.addAction(tr("Add"));
    del = contextMenu.addAction(tr("Del"));
    copy = contextMenu.addAction(tr("Copy"));
    paste = contextMenu.addAction(tr("Paste"));
    cut = contextMenu.addAction(tr("Cut"));
    connect(add, &QAction::triggered, this, &MainWindow::onAddRestaurant);
    connect(del, &QAction::triggered, this, &MainWindow::onDeleteRestaurant);
    connect(copy, &QAction::triggered, this, &MainWindow::onCopy);
    connect(paste, &QAction::triggered, this, &MainWindow::onPaste);
    connect(cut, &QAction::triggered, this, &MainWindow::onCut);

    contextMenu.exec(m_tableView->viewport()->mapToGlobal(pos));
}

// Открытие файла
void MainWindow::onOpenFile() {
    if (!filename.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Save"), tr("Save?"),
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            onSaveFile();
    }
    filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("db Files (*.db)"));
    m_controller->loadFromFile(filename);
    this->setWindowTitle(filename);
    m_tableView->resizeColumnsToContents();
    updateActtions();
}

// Сохранение файла
void MainWindow::onSaveFile() {
    qDebug() << filename;
    m_controller->saveToFile(filename);
}

// Сохранение файла как
void MainWindow::onSaveAsFile() {
    filename = QFileDialog::getSaveFileName(this, tr("Save File"), "", "db Files (*.db)");
    if (!filename.isEmpty()) {
        m_controller->saveToFile(filename);
    }
}

// Создание нового файла
void MainWindow::onNewFile() {
    if (filename.isEmpty()) {
        filename = "untitled" + QString::number(numfile);
        numfile++;
        this->setWindowTitle(filename);
        m_model->clear();
    } else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Save"), tr("Save?"),
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            onSaveFile();
        filename = "untitled" + QString::number(numfile);
        numfile++;
        this->setWindowTitle(filename);
        m_model->clear();
    }
    m_tableView->resizeColumnsToContents();
    updateActtions();

}

// Открытие нового окна
void MainWindow::onNewFileWindow() {
    MainWindow *newWindow = new MainWindow;

    // Подключаем новое окно к сигналу смены языка
    connect(this, &MainWindow::languageChanged, newWindow, &MainWindow::changeLanguage);
    connect(newWindow, &MainWindow::languageChanged, this, &MainWindow::changeLanguage);
    connect(this, &MainWindow::updateLanguageAction, newWindow, &MainWindow::updateLanguageAction);
    connect(newWindow, &MainWindow::updateLanguageAction, this, &MainWindow::updateLanguageAction);

    // Установим текущий язык в новом окне
    newWindow->changeLanguage(currentLocale);

    // Установим текущий язык в группу действий языка в новом окне
    foreach (QAction *action, newWindow->languageActionGroup->actions()) {
        if (action->data().toString() == currentLocale) {
            action->setChecked(true);
        }
    }

    newWindow->show();
}

// Добавление строки ресторана
void MainWindow::onAddRestaurant() {
    QString name = QInputDialog::getText(this, tr("Add Restaurant"), tr("Name:"));
    if (name.isEmpty()) return;

    QString owner = QInputDialog::getText(this, tr("Add Restaurant"), tr("Owner:"));
    if (owner.isEmpty()) return;

    QString address = QInputDialog::getText(this, tr("Add Restaurant"), tr("Address:"));
    if (address.isEmpty()) return;

    QString cuisine = QInputDialog::getText(this, tr("Add Restaurant"), tr("Cuisine:"));
    if (cuisine.isEmpty()) return;

    int capital = QInputDialog::getInt(this, tr("Add Restaurant"), tr("Capital:"));
    int attendance = QInputDialog::getInt(this, tr("Add Restaurant"), tr("Attendance:"), 0, 0, 100); // Валидация
    int avgbill = QInputDialog::getInt(this, tr("Add Restaurant"), tr("Average Bill:"));
    float rating = QInputDialog::getDouble(this, tr("Add Restaurant"), tr("Rating:"), 0.0, 0.0, 5.0); // Валидация

    m_model->addRestaurant(Restaurant(name, owner, address, cuisine, capital, attendance, avgbill, rating));
}

// Получение индекса клетки
void MainWindow::onCellClicked(const QModelIndex &index) {
    if (index.isValid()) {
        selectedrow = index.row(); // Сохраняем индекс кликнутой строки
    }
}

// Удаление строки ресторана
void MainWindow::onDeleteRestaurant() {
    if (selectedrow >= 0) {
        m_model->removeRestaurant(selectedrow);
        selectedrow = -1;
    }
}

// Поиск по столбцу
void MainWindow::onSearch(const QString &text) {
    for (int row = 0; row < m_model->rowCount(); ++row) {
        QModelIndex index = m_model->index(row, m_currentSearchColumn);
        bool match = m_model->data(index).toString().contains(text, Qt::CaseInsensitive);
        m_tableView->setRowHidden(row, !match);
    }
}

// Окно деталей
void MainWindow::onShowDetails() {
    QModelIndexList selectedRows = m_tableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        int row = selectedRows.first().row();
        Restaurant restaurant = m_model->m_restaurants.at(row);
        RestaurantDetailsWindow *detailsWindow = new RestaurantDetailsWindow(restaurant, this);
        detailsWindow->show();
    }
}

// Получение id столбца
void MainWindow::onColumnHeaderClicked(int column) {
    static Qt::SortOrder sortOrder = Qt::AscendingOrder;

    // Установка текущего столбца для поиска
    m_currentSearchColumn = column;
    // Сортировка модели по указанному столбцу
    m_model->sort(column, sortOrder);
    onSearch(m_searchBar->text());
    // Переключение порядка сортировки для следующего клика
    sortOrder = (sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
}

// Копирование ячейки
void MainWindow::onCopy() {
    // Получаем выделенные ячейки из модели
    QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedIndexes();

    // Если нет выделенных ячеек, выходим из функции
    if (selectedIndexes.isEmpty()) return;

    // Создаем строку для хранения данных
    QString clipboardData;

    // Получаем данные из выделенных ячеек и добавляем их в строку
    foreach (const QModelIndex &index, selectedIndexes) {
        // Получаем данные из индекса
        QString cellData = index.data(Qt::DisplayRole).toString();

        // Добавляем данные в строку с разделителем табуляции
        clipboardData.append(cellData);
        clipboardData.append('\t');

        // Если достигнут конец строки, добавляем символ новой строки
        if (index.column() == m_model->columnCount() - 1) {
            clipboardData.append('\n');
        }
    }

    // Копируем данные в буфер обмена
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(clipboardData);
}

// Вставка ячейки
void MainWindow::onPaste() {
    // Получаем текст из буфера обмена
    QClipboard *clipboard = QApplication::clipboard();
    QString clipboardText = clipboard->text();

    // Разбиваем текст на строки
    QStringList lines = clipboardText.split('\n', Qt::SkipEmptyParts);

    // Получаем индекс начальной ячейки для вставки
    QModelIndex startIndex = m_tableView->currentIndex();

    // Если нет выделенной ячейки, выбираем верхнюю левую ячейку
    if (!startIndex.isValid()) {
        startIndex = m_model->index(0, 0);
    }

    // Проходимся по строкам из буфера обмена
    foreach (const QString &line, lines) {
        // Разбиваем строку на столбцы
        QStringList columns = line.split('\t', Qt::SkipEmptyParts);

        // Проходимся по столбцам
        foreach (const QString &column, columns) {
            // Вставляем данные в модель
            m_model->setData(startIndex, column);

            // Переходим к следующей ячейке
            startIndex = m_model->index(startIndex.row(), startIndex.column() + 1);
        }

        // Переходим к следующей строке
        startIndex = m_model->index(startIndex.row() + 1, 0);
    }
}

// Вырезание ячейки
void MainWindow::onCut() {
    // Вызываем функцию копирования
    onCopy();

    // Удаляем выделенные данные из модели
    QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedIndexes();
    foreach (const QModelIndex &index, selectedIndexes) {
        m_model->setData(index, QVariant()); // Устанавливаем пустое значение
    }
}

// Обо мне
void MainWindow::onAboutMe() {
    QMessageBox::information(this, tr("About me"), tr("Developed by Gagiev George\nICTMS 2-4\nvar 7"));
}

// Об Qt
void MainWindow::onAboutQT() {
    QApplication::aboutQt();
}

// Печать таблицы
void MainWindow::onPrint() {
    m_model->printOutput();
}

// Получение данных с таблицы
QVector<std::pair<int, float>> MainWindow::extractChartData() const {
    QVector<std::pair<int, float>> data_;

    for (int row = 0; row < m_model->rowCount(); ++row) {
        int x = m_model->data(m_model->index(row, 6)).toInt();
        float y = m_model->data(m_model->index(row, 7)).toFloat();
        data_.append(std::make_pair(x, y));
    }
    std::sort(data_.begin(), data_.end());
    return data_;
}

// Создание окна с графиком
void MainWindow::showChart() {
    QVector<std::pair<int, float>> data_ = extractChartData();
    ChartDialog *dialog = new ChartDialog(data_, this);
    dialog->exec();
    delete dialog;
}



