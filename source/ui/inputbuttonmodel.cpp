#include <iostream>

#include "ui/inputbuttonmodel.hpp"
#include "core/conversions.hpp"

InputButtonModel::InputButtonModel(QQmlContext* context, MemoryAccess m) : QObject(), context(context), start(0), memoryAccess(m){
    context->setContextProperty("inputButtonMod", this);
}

void InputButtonModel::buttonClicked(int id){
    //id an startadresse im Memory speichern
    MemoryValue m=conversions::convert(id, 8);
    memoryAccess.putMemoryValueAt(start, m);
    std::cout << id<<std::endl;
}

QString InputButtonModel::getStart(){
    return QString::fromStdString(std::to_string(start));
}

void InputButtonModel::setStart(int start){
    if(memoryAccess.getMemorySize().get() > start+2){
        this->start=start;
    }
}
