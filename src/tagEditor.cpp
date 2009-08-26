#include "tagEditor.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QVBoxLayout>

TagEditor::TagEditor(QWidget* parent) : QDockWidget(parent) {
  setWindowTitle("Tag Editor");
  QWidget* centralWidget = new QWidget(this);
  setWidget(centralWidget);
  QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
  QTreeWidget* subTagTree = new QTreeWidget(centralWidget);
  mainLayout->addWidget(subTagTree);
}