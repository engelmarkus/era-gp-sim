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

#include "ui/gui-project.hpp"

#include <QUrl>
#include <cstdio>
#include <functional>

#include "common/utility.hpp"
#include "ui/snapshot-component.hpp"

GuiProject::GuiProject(QQmlContext* context,
                       const ArchitectureFormula& formula,
                       std::size_t memorySize,
                       const std::string& parserName,
                       std::shared_ptr<SnapshotComponent> snapshotComponent,
                       QObject* parent)
: QObject(parent)
, _projectModule(formula, memorySize, parserName)
, _editorComponent(context,
                   _projectModule.getParserInterface(),
                   _projectModule.getCommandInterface())
, _outputComponent(_projectModule.getMemoryManager(),
                   _projectModule.getMemoryAccess(),
                   context)
, _memoryModel(_projectModule.getMemoryAccess(),
               _projectModule.getMemoryManager(),
               context)
, _registerModel(_projectModule.getArchitectureAccess(),
                 _projectModule.getMemoryManager(),
                 _projectModule.getMemoryAccess(),
                 context)
, _defaultTextFileSavePath()
, _defaultProjectSavePath()
, _snapshotComponent(snapshotComponent)
, _architectureFormulaString(SnapshotComponent::architectureToString(formula)) {
  context->setContextProperty("guiProject", this);
  // set the callback for memory and register
  _projectModule.getMemoryManager().setUpdateRegisterCallback(
      [this](const std::string& name) {
        emit registerChanged(QString::fromStdString(name));
      });

  _projectModule.getMemoryManager().setUpdateMemoryCallback(
      [this](std::size_t address, std::size_t length) {
        emit memoryChanged(address, length);
      });

  _projectModule.getParserInterface().setThrowErrorCallback([this](
      const std::string& message, const std::vector<std::string>& arguments) {
    _throwError(message, arguments);
  });

  _projectModule.getMemoryManager().setErrorCallback([this](
      const std::string& message, const std::vector<std::string>& arguments) {
    _throwError(message, arguments);
  });

  // connect all receiving components to the callback signals
  QObject::connect(this,
                   SIGNAL(registerChanged(const QString&)),
                   &_registerModel,
                   SLOT(updateContent(const QString&)),
                   Qt::QueuedConnection);

  QObject::connect(this,
                   SIGNAL(memoryChanged(std::size_t, std::size_t)),
                   &_memoryModel,
                   SLOT(onMemoryChanged(std::size_t, std::size_t)),
                   Qt::QueuedConnection);

  QObject::connect(this,
                   SIGNAL(memoryChanged(std::size_t, std::size_t)),
                   &_outputComponent,
                   SLOT(updateMemory(std::size_t, std::size_t)),
                   Qt::QueuedConnection);
}

GuiProject::~GuiProject() {
  stop();
}

void GuiProject::changeSystem(std::string base) {
  // Alle Komponenten informieren
}

void GuiProject::parse() {
  _editorComponent.parse();
}

void GuiProject::run() {
  _editorComponent.parse();
  _projectModule.getCommandInterface().execute();
}

void GuiProject::runLine() {
  _editorComponent.parse();
  _projectModule.getCommandInterface().executeNextLine();
}

void GuiProject::runBreakpoint() {
  _editorComponent.parse();
  _projectModule.getCommandInterface().executeToBreakpoint();
}

void GuiProject::stop() {
  _projectModule.stopExecution();
}

void GuiProject::reset() {
  _projectModule.reset();
  _projectModule.getCommandInterface().setExecutionPoint(1);
  _editorComponent.parse(true);
}

void GuiProject::saveText() {
  if (_defaultTextFileSavePath.isEmpty()) {
    emit saveTextAs();
  } else {
    saveTextAs(_defaultTextFileSavePath);
  }
}

void GuiProject::saveTextAs(QUrl path, bool saveAsDefault) {
  QString qName = path.path();
  if (saveAsDefault) _defaultTextFileSavePath = qName;
  std::string name = qName.toStdString();
  std::string text = _editorComponent.getText().toStdString();
  try {
    Utility::storeToFile(name, text);
  } catch (const std::ios_base::failure& exception) {
    _throwError(std::string("Could not save file! ") + exception.what(),
                std::vector<std::string>());
  }
}

void GuiProject::loadText(QUrl path) {
  QString qName = path.path();
  std::string filePath = qName.toStdString();
  std::string text;
  try {
    text = Utility::loadFromFile(filePath);
    QString qText = QString::fromStdString(text);
    _editorComponent.setText(qText);
  } catch (const std::ios_base::failure& exception) {
    _throwError(std::string("Could not load file!") + exception.what(),
                std::vector<std::string>());
  }
}

void GuiProject::saveSnapshot(QString qName) {
  Json snapshot = _projectModule.getMemoryManager().generateSnapshot().get();
  std::string snapshotString = snapshot.dump(4);
  try {
    _snapshotComponent->addSnapshot(
        _architectureFormulaString, qName, snapshotString);
  } catch (const std::exception& exception) {
    _throwError(
        std::string("Could not write snapshot to disk! ") + exception.what(),
        std::vector<std::string>());
  }
}

void GuiProject::loadSnapshot(Json snapshot) {
  _projectModule.getMemoryManager().loadSnapshot(snapshot);
  _editorComponent.parse(true);
}

void GuiProject::saveProject() {
  if (_defaultProjectSavePath.isEmpty()) {
    emit saveProjectAs();
  } else {
    saveProjectAs(QUrl(_defaultProjectSavePath));
  }
}

void GuiProject::saveProjectAs(QUrl path) {
  QFileInfo saveInfo = QFileInfo(path.path());
  QString saveName = saveInfo.fileName();
  QDir saveDirectory = saveInfo.absoluteDir();
  if (!saveDirectory.exists(saveName) || !saveInfo.isDir()) {
    saveDirectory.mkpath(saveName);
  }
  Json snapshot = _projectModule.getMemoryManager().generateSnapshot().get();
  std::string snapshotString = snapshot.dump(4);
  std::string codePath =
      Utility::joinPaths(path.path().toStdString(), saveName.toStdString());
  saveTextAs(QUrl(QString::fromStdString(codePath)), false);
  try {
    std::string snapshotPath = Utility::joinPaths(
        path.path().toStdString(),
        saveName.toStdString() + SnapshotComponent::fileExtension);
    Utility::storeToFile(snapshotPath, snapshot.dump(4));
    _defaultProjectSavePath = path.path();
  } catch (const std::exception& exception) {
    _throwError("Project save failed!", std::vector<std::string>());
  }
}

void GuiProject::removeSnapshot(QString qName) {
  _snapshotComponent->removeSnapshot(_architectureFormulaString, qName);
}

void GuiProject::loadSnapshot(QString qName) {
  try {
    std::string path =
        _snapshotComponent->snapshotPath(_architectureFormulaString, qName);
    Json snapshot = Json::parse(Utility::loadFromFile(path));
    loadSnapshot(snapshot);
  } catch (const std::exception& exception) {
    _throwError(
        std::string("Could not load snapshot from file! ") + exception.what(),
        std::vector<std::string>());
  }
}

QStringList GuiProject::getSnapshots() {
  return _snapshotComponent->getSnapshotList(_architectureFormulaString);
}

std::function<std::string(MemoryValue)> GuiProject::getHexConversion() {
  return hexConversion;
}

std::function<std::string(MemoryValue)> GuiProject::getBinConversion() {
  return binConversion;
}

std::function<std::string(MemoryValue)> GuiProject::getOctConversion() {
  return octConversion;
}

std::function<std::string(MemoryValue)>
GuiProject::getSignedDecimalConversion() {
  return signedDecimalConversion;
}

std::function<std::string(MemoryValue)>
GuiProject::getUnsignedDecimalConversion() {
  return unsignedDecimalConversion;
}

std::function<std::string(MemoryValue)>
GuiProject::getDecimalFloatConversion() {
  return decimalFloatConversion;
}

std::function<MemoryValue(std::string)> GuiProject::getSignedToMemoryValue() {
  return signedToMemoryValue;
}

std::function<MemoryValue(std::string)> GuiProject::getHexToMemoryValue() {
  return hexToMemoryValue;
}

std::function<MemoryValue(std::string)> GuiProject::getBinToMemoryValue() {
  return binToMemoryValue;
}

std::function<MemoryValue(std::string)> GuiProject::getOctToMemoryValue() {
  return octToMemoryValue;
}

std::function<MemoryValue(std::string)> GuiProject::getUnsignedToMemoryValue() {
  return unsignedToMemoryValue;
}

std::function<MemoryValue(std::string)> GuiProject::getFloatToMemoryValue() {
  return floatToMemoryValue;
}

void GuiProject::_throwError(const std::string& message,
                             const std::vector<std::string>& arguments) {
  QString errorMessage = QString::fromStdString(message);
  emit error(errorMessage);
}
