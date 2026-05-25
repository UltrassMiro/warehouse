#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include "../core/Warehouse.h"
#include "../file/FileManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Warehouse warehouse;
    FileManager fileManager;

    QTableWidget* table;
    QLineEdit* searchLine;
    QComboBox* categoryBox;
    QLabel* totalLabel;

    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    QPushButton* importButton;
    QPushButton* exportButton;
    QPushButton* refreshButton;

    void setupUi();
    void setupStyle();

    void loadData();
    void saveData();

    void refreshTable();
    void refreshCategories();
    void refreshTotal();

    int getSelectedItemId() const;

private slots:
    void addItem();
    void editItem();
    void deleteItem();
    void importExcel();
    void exportExcel();
    void searchChanged();
    void categoryChanged();

public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif