#include "data.h"

/**
 * @brief Data::load
 * @param fileName
 */
void Data::load(QString fileName)
{
    // Выбираем файл
    QFile file(fileName);

    // Пытаемся открыть файл
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Читаем первую строчку
        QString line = file.readLine();

        // Проверяем что за файл
        if (line == "MegaWin ASCII file\n") {
            // Если файл экспорта MegaWin
            this->loadMegawin(file);
        }

        // Закрываем файл
        file.close();
    }
}

/**
 * @brief Data::get
 * @param canal
 * @return
 */
Canal* Data::get(int canal)
{
    if (canal >= 0 && canal < this->_canals.length()) {
        return this->_canals[canal];
    }
}


/**
 * @brief Data::loadMegawin
 * @param file
 */
void Data::loadMegawin(QFile &file)
{
    // Читаем первую строчку
    QString line = file.readLine();

    // Пролистываем до надписи [DATA]
    while (line != "[DATA]\n") {
        line = file.readLine();
    }

    // Очищаем массив данных
    this->_canals.clear();

    // Читаем остальные данные до конца файла, заносим их в массив
    while(!line.isEmpty()) {
        // Читаем строку
        line = file.readLine();

        if (line.isEmpty()) {
           break;
        }

        // Вытаскиваем данные из строки
        QStringList list = line.split("\t");

        // Если каналы еще не созданы
        if (this->_canals.length() == 0) {
            // Добавляем каналы в массив данных
            for(int canal = 0; canal < list.length(); canal++) {
                Canal *newCanal = new Canal();
                this->_canals.push_back(newCanal);
            }
        }

        // Для каждого канала
        for(int canal = 0; canal < 1/*this->_canals.length()*/; canal++)
        {
            // Вытаскиваем само число в виде строки
            QString str = list.at(canal);

            // Записываем в чанк число, заменив запятую на точку
            this->_canals[canal]->add(str.replace(",", ".").toFloat());
        }
    }
}
