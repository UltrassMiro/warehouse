#include "MainWindow.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QSpinBox>
#include <QHeaderView>
#include <QSet>
#include "../models/Item.h"
#include "../file/ExcelExporter.h"
#include "../file/ExcelImporter.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setupUi();
    setupStyle();
    loadData();
    refreshCategories();
    refreshTable();
    refreshTotal();
}
// Qt інтерфейс
void MainWindow::setupUi() {
    setWindowTitle("Warehouse Manager");
    resize(900, 600);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    QHBoxLayout* filterLayout = new QHBoxLayout();

    searchLine = new QLineEdit(this);
    searchLine->setPlaceholderText("Search by name...");

    categoryBox = new QComboBox(this);
    categoryBox->addItem("All categories");

    filterLayout->addWidget(searchLine);
    filterLayout->addWidget(categoryBox);

    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(
        {"ID", "Name", "Quantity", "Category"}
    );

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    addButton = new QPushButton("Add", this);
    editButton = new QPushButton("Edit", this);
    deleteButton = new QPushButton("Delete", this);
    importButton = new QPushButton("Import Excel", this);
    exportButton = new QPushButton("Export Excel", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(importButton);
    buttonLayout->addWidget(exportButton);

    totalLabel = new QLabel(this);

    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(table);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(totalLabel);

    connect(addButton, &QPushButton::clicked,
            this, &MainWindow::addItem);

    connect(editButton, &QPushButton::clicked,
            this, &MainWindow::editItem);

    connect(deleteButton, &QPushButton::clicked,
            this, &MainWindow::deleteItem);

    connect(importButton, &QPushButton::clicked,
            this, &MainWindow::importExcel);

    connect(exportButton, &QPushButton::clicked,
            this, &MainWindow::exportExcel);

    connect(searchLine, &QLineEdit::textChanged,
            this, &MainWindow::searchChanged);

    connect(categoryBox,
            &QComboBox::currentTextChanged,
            this,
            &MainWindow::categoryChanged);
}

// Стилі для інтерфейсу
void MainWindow::setupStyle() {
    setStyleSheet(
        "QMainWindow { background-color: #f4f6f8; }"
        "QTableWidget { background-color: white; border: 1px solid #ccd1d9; font-size: 14px; }"
        "QHeaderView::section { background-color: #2f3640; color: white; padding: 6px; }"
        "QLineEdit, QComboBox { padding: 8px; font-size: 14px; border: 1px solid #b2bec3; border-radius: 5px; }"
        "QPushButton { background-color: #0984e3; color: white; padding: 8px 12px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #0767b1; }"
        "QLabel { font-size: 14px; font-weight: bold; }"
    );
}
void MainWindow::loadData() {
    fileManager.load(warehouse);
}

void MainWindow::saveData() {
    fileManager.save(warehouse);
}

int MainWindow::getSelectedItemId() const {
    int row = table->currentRow();

    if (row < 0)
        return -1;

    return table->item(row, 0)->text().toInt();
}
// Оновлення таблиці
void MainWindow::refreshTable() {

    QString search = searchLine->text().toLower();
    QString category = categoryBox->currentText();

    table->setRowCount(0);

    for (const auto& item : warehouse.getItems()) {

        QString name = QString::fromStdString(item.getName());
        QString itemCategory = QString::fromStdString(item.getCategory());

        bool matchesSearch = name.toLower().contains(search);
        bool matchesCategory = category == "All categories" || itemCategory == category;

        if (!matchesSearch || !matchesCategory)
            continue;

        int row = table->rowCount();

        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(QString::number( item.getId())));
        table->setItem(row, 1, new QTableWidgetItem(name));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(item.getQuantity())));
        table->setItem(row, 3, new QTableWidgetItem(itemCategory));
    }

    refreshTotal();
}

void MainWindow::searchChanged() {
    refreshTable();
}

void MainWindow::categoryChanged() {
    refreshTable();
}

void MainWindow::refreshTotal() {
    totalLabel->setText("Total amount of products: "
        + QString::number( warehouse.getTotalItems()));
}

void MainWindow::refreshCategories() {
    QString current = categoryBox->currentText();

    categoryBox->blockSignals(true);
    categoryBox->clear();
    categoryBox->addItem("All categories");

    QSet<QString> categories;

    for (const auto& item :
         warehouse.getItems()) {

        categories.insert(QString::fromStdString( item.getCategory()));
    }

    for (const QString& category : categories) {
        categoryBox->addItem( category);
    }

    int index = categoryBox->findText(current);

    if (index >= 0)
        categoryBox->setCurrentIndex(index);

    categoryBox->blockSignals(false);
}
// Додавання
void MainWindow::addItem() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add product");

    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    QSpinBox quantitySpin;
    QComboBox categoryCombo;

    categoryCombo.addItems({
        "Tools",
        "Screws and nuts",
        "Paints",
        "Uniform",
        "Other"
    });

    quantitySpin.setMinimum(0);
    quantitySpin.setMaximum(1000000);

    form.addRow("Name:", &nameEdit);
    form.addRow("Quantity:", &quantitySpin);
    form.addRow("Category:", &categoryCombo);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    form.addRow(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = nameEdit.text().trimmed();
        QString category = categoryCombo.currentText();

        if (name.isEmpty()) {
            QMessageBox::warning(this, "Error", "Name cannot be empty.");
            return;
        }

        warehouse.addItem(
            Item(name.toStdString(), quantitySpin.value(), category.toStdString()));

        saveData();
        refreshCategories();
        refreshTable();
    }
}
// Редагування
void MainWindow::editItem() {
    int id = getSelectedItemId();

    if (id == -1) {
        QMessageBox::warning(this, "Error", "Select product for editing.");
        return;
    }

    Item* item = warehouse.findItemById(id);

    if (item == nullptr) {
        QMessageBox::warning(this, "Error", "Product not found.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Edit product");

    QFormLayout form(&dialog);

    QLineEdit nameEdit(QString::fromStdString(item->getName()));

    QSpinBox quantitySpin;
    QComboBox categoryCombo;

    categoryCombo.addItems({
        "Tools",
        "Screws and nuts",
        "Paints",
        "Uniform",
        "Other"
    });

    int categoryIndex = categoryCombo.findText(QString::fromStdString(item->getCategory()));

    if (categoryIndex >= 0) {
        categoryCombo.setCurrentIndex(categoryIndex);
    }

    quantitySpin.setMinimum(0);
    quantitySpin.setMaximum(1000000);
    quantitySpin.setValue(item->getQuantity());

    form.addRow("Name:", &nameEdit);
    form.addRow("Quantity:", &quantitySpin);
    form.addRow("Category:", &categoryCombo);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    form.addRow(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = nameEdit.text().trimmed();
        QString category = categoryCombo.currentText();

        if (name.isEmpty()) {
            QMessageBox::warning(this, "Error", "Name cannot be empty.");
            return;
        }

        warehouse.updateItem(
            id, name.toStdString(), quantitySpin.value(), category.toStdString());

        saveData();
        refreshCategories();
        refreshTable();
    }
}
// Видалення
void MainWindow::deleteItem() {

    int id = getSelectedItemId();

    if (id == -1) {

        QMessageBox::warning(this, "Error", "Select product for deletion.");

        return;
    }

    auto reply = QMessageBox::question(
        this, "Confirmation", "Delete selected product?");

    if (reply == QMessageBox::Yes) {

        warehouse.removeItem(id);

        saveData();
        refreshCategories();
        refreshTable();
    }
}
// Експорт в Excel
void MainWindow::exportExcel() {

    QString path = QFileDialog::getSaveFileName(
        this, "Export Excel", "warehouse.xlsx", "Excel (*.xlsx)");

    if (path.isEmpty())
        return;

    try {

        ExcelExporter::exportToExcel(warehouse, path.toStdString());
        QMessageBox::information(this, "Success", "Export completed.");
    }
    catch (exception& e) {
        QMessageBox::critical(this, "Export error", e.what());
    }
}
// Імпорт з Excel
void MainWindow::importExcel() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "Import Excel", "", "Excel files (*.xlsx)");

    if (filePath.isEmpty()) {
        return;
    }

    try {
        ExcelImporter::importFromExcel(warehouse, filePath.toStdString());

        saveData();
        refreshCategories();
        refreshTable();

        QMessageBox::information(this, "Success", "Excel import completed.");
    }
    catch (const exception& e) {
        QMessageBox::critical(this, "Import error", e.what());
    }
}