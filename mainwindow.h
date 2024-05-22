#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLineEdit>
#include <QTranslator>
#include <QLocale>
#include <QActionGroup>
#include "qmenu.h"
#include "restaurantmodel.h"
#include "restaurantcontroller.h"

class QActionGroup;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getCurrentLocale() const;

signals:
    void languageChanged(const QString &locale);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // Обновление действия для изменения языка
    void updateLanguageAction(const QString &locale);

    // Слоты для обработки действий пользователя
    void onOpenFile();
    void onSaveAsFile();
    void onSaveFile();
    void onNewFile();
    void onNewFileWindow();
    void onAddRestaurant();
    void onDeleteRestaurant();
    void onCellClicked(const QModelIndex &index);
    void onSearch(const QString &text);
    void onShowDetails();
    void onColumnHeaderClicked(int column);
    void onAboutMe();
    void onAboutQT();
    void onPrint();
    void onCopy();
    void onPaste();
    void onCut();
    void changeLanguage(const QString &locale);

private:
    // Функции для настройки интерфейса
    void showChart();
    void setupMenuBar();
    void setupTableView();
    void setupSearchBar();
    void createLanguageMenu();
    void retranslateUi();
    void showContextMenu(const QPoint &pos);
    void getSettings();
    void setSettings();
    void updateActtions();
    QVector<QPair<int, float>> extractChartData() const;

    // Объявление элементов интерфейса и связанных объектов
    QAction *showChartAction;
    QTableView *m_tableView;
    QLineEdit *m_searchBar;
    RestaurantModel *m_model;
    RestaurantController *m_controller;
    int selectedrow = -1;
    int m_currentSearchColumn;
    static int numfile;
    QString filename;
    QMenu *languageMenu;
    QTranslator appTranslator;
    QTranslator qtTranslator;
    QActionGroup *languageActionGroup;
    QString qmPath;
    QString currentLocale;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *printAction;
    QAction *addRestaurantAction;
    QAction *deleteRestaurantAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *cutAction;
    QAction *add;
    QAction *del;
    QAction *copy;
    QAction *paste;
    QAction *cut;
};

#endif // MAINWINDOW_H
