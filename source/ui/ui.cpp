/*
* C++ Assembler Interpreter
* Copyright (C) 2016 Chair of Computer Architecture
* at Technical University of Munich
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ui/ui.hpp"

#include <QUrl>

#include "arch/common/architecture-formula.hpp"
#include "common/assert.hpp"
#include "common/utility.hpp"
#include "ui/snapshot-component.hpp"


Ui::Ui(int& argc, char** argv)
: _architectureMap()
, _qmlApplication(argc, argv)
, _engine()
, _projects()
, _snapshots(
      std::make_shared<SnapshotComponent>(Utility::joinToRoot("snapshots"))) {
  _loadArchitectures();
}

int Ui::runUi() {
  qRegisterMetaType<std::size_t>("std::size_t");
  _engine.rootContext()->setContextProperty("ui", this);
  _engine.rootContext()->setContextProperty("snapshotComponent",
                                            _snapshots.get());
  _engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  return _qmlApplication.exec();
}

void Ui::addProject(QQuickItem* tabItem,
                    QQmlComponent* projectComponent,
                    const QVariant& memorySizeQVariant,
                    const QString& architecture,
                    const QString& optionName,
                    const QString& parser) {
  // create ArchitectureFormula
  ArchitectureFormula architectureFormula(architecture.toStdString());

  // add all extensions which are defined for this option
  for (const auto& qstring : _getOptionFormula(architecture, optionName)) {
    architectureFormula.addExtension(qstring.toStdString());
  }
  // get the memory size from the qvariant object.
  std::size_t memorySize = memorySizeQVariant.value<std::size_t>();
  _createProject(tabItem,
                 projectComponent,
                 architectureFormula,
                 memorySize,
                 parser.toStdString());
}

void Ui::_createProject(QQuickItem* tabItem,
                        QQmlComponent* projectComponent,
                        const ArchitectureFormula& architectureFormula,
                        std::size_t memorySize,
                        const std::string& parserName) {
  // parent is tabItem, so it gets destroyed at the same time
  QQmlContext* context = new QQmlContext(qmlContext(tabItem), tabItem);

  // the pointer is not needed anywhere, the object is deleted by qml when
  // tabItem is deleted
  _projects.push_back(new GuiProject(context,
                                     architectureFormula,
                                     memorySize,
                                     parserName,
                                     _snapshots,
                                     tabItem));

  // instantiate the qml project item with the prepared context
  QQuickItem* projectItem =
      qobject_cast<QQuickItem*>(projectComponent->create(context));

  // set the parent of projectItem, so its deletion is handled by qml
  projectItem->setParent(tabItem);

  // set visual parent of the projectItem
  projectItem->setParentItem(tabItem);
}

void Ui::loadProject(QQuickItem* tabItem,
                     QQmlComponent* projectComponent,
                     QUrl fileUrl) {
  std::string fileName = QFileInfo(fileUrl.path()).fileName().toStdString();
  std::string snapshotPath =
      Utility::joinPaths(fileUrl.path().toStdString(),
                         fileName + SnapshotComponent::fileExtension);
  std::string codePath =
      Utility::joinPaths(fileUrl.path().toStdString(), fileName);
  // load the snapshot from file and check if it is valid
  Json data;
  try {
    data = (Json::parse(Utility::loadFromFile(snapshotPath)));
  } catch (const std::exception& exception) {
    _snapshots->snapshotError(QString::fromStdString(
        std::string("Loading snapshot file from save failed. ") +
        exception.what()));
  }
  Snapshot snapshot(data);
  if (!snapshot.isValid()) {
    _snapshots->snapshotError("Loading snapshot file from save failed");
    return;
  }
  // create a new project which is compatible to the snapshot
  _createProject(tabItem,
                 projectComponent,
                 snapshot.getArchitectureFormula(),
                 snapshot.getMemoryByteCount(),
                 snapshot.getParserName());
  // load the snapshot in the new project.
  _projects[_projects.size() - 1]->loadSnapshot(snapshot.getJson());
  _projects[_projects.size() - 1]->loadText(
      QUrl(QString::fromStdString(codePath)));
}

QStringList Ui::getArchitectures() const {
  return _architectureMap.keys();
}

QStringList Ui::getOptionNames(QString architectureName) const {
  auto formulaMap =
      std::get<0>(_architectureMap.find(architectureName).value());
  return formulaMap.keys();
}

QStringList
Ui::_getOptionFormula(QString architectureName, QString optionName) const {
  auto formulaMap =
      std::get<0>(_architectureMap.find(architectureName).value());
  return formulaMap.find(optionName).value();
}

QStringList Ui::getParsers(QString architectureName) const {
  return std::get<1>(_architectureMap.find(architectureName).value());
}

void Ui::_loadArchitectures() {
  assert::that(_architectureMap.empty());
  std::string path = Utility::joinToRoot("isa", "isa-list.json");
  Ui::Json data = Ui::Json::parse(Utility::loadFromFile(path));
  assert::that(data.count("architectures"));
  assert::that(!data["architectures"].empty());

  for (auto& architecture : data["architectures"]) {
    assert::that(architecture.count("name"));
    assert::that(architecture.count("options"));
    assert::that(!architecture["options"].empty());
    assert::that(architecture.count("parsers"));
    assert::that(!architecture["parsers"].empty());

    QMap<QString, QStringList> optionNameFormulaMap;
    QStringList parserList;
    for (const auto& option : architecture["options"]) {
      assert::that(option.count("name"));
      assert::that(option.count("formula"));
      assert::that(!option["formula"].empty());
      QStringList formula;
      for (const auto& extension : option["formula"]) {
        formula.push_back(QString::fromStdString(extension));
      }
      optionNameFormulaMap.insert(QString::fromStdString(option["name"]),
                                  formula);
    }
    for (const auto& parser : architecture["parsers"]) {
      parserList.push_back(QString::fromStdString(parser));
    }
    _architectureMap.insert(QString::fromStdString(architecture["name"]),
                            std::make_tuple(optionNameFormulaMap, parserList));
  }
}

void Ui::removeProject(int index) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects.erase(_projects.begin() + index);
}

void Ui::changeSystem(int index, QString base) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->changeSystem(base.toStdString());
}

void Ui::parse(int index) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->parse();
}

void Ui::run(int index) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->run();
}

void Ui::runLine(int index) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->runLine();
}

void Ui::runBreakpoint(int index) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->runBreakpoint();
}

void Ui::stop(int index) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->stop();
}

void Ui::reset(int index) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->reset();
}

void Ui::saveText(int index) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->saveText();
}

void Ui::saveTextAs(int index, QUrl path) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->saveTextAs(path);
}

void Ui::loadText(int index, QUrl path) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->loadText(path);
}

void Ui::saveSnapshot(int index, QString name) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->saveSnapshot(name);
}

void Ui::loadSnapshot(int index, QString name) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->loadSnapshot(name);
}

void Ui::saveProject(int index) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->saveProject();
}

void Ui::saveProjectAs(int index, QUrl path) {
  assert::that(index >= 0);
  assert::that(index < _projects.size());
  _projects[index]->saveProjectAs(path);
}
