#include "data.h"
#include <QTextStream>

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

        // Записываем заголовки документа, чтобы можно было сохранить
        QString line;
        this->_head = "";

        do {
            line = file.readLine();
            this->_head += line;
        } while(line != "[DATA]\n");

        file.reset();

        // Читаем первую строчку
        line = file.readLine();

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
 * @brief Data::save
 * @param fileName
 */
bool Data::save(QString fileName)
{
    // Создаем файл
    QFile saveFile(fileName);

    // Открываем его для чтения
    if (saveFile.open(QIODevice::ReadWrite)) {

        // Создаем поток для записи в файл
        QTextStream stream(&saveFile);

        // Дописываем в файл шапку
        stream << this->_head;

        // ПО всей длинне файла
        for (long i = 0; i < this->_canalsNew.at(0)->length(); i++) {

            // Создаем пустую строку
            QString line = "";

            // По всем каналам файла
            for (int c = 0; c < this->_canalsNew.length(); c++) {
                // Дописываем к строке данные, заменив точку запятой
                line += QString::number(_canalsNew.at(c)->get(i)).replace(".", ",");

                // Если это не последняя запись
                if (c != this->_canalsNew.length() - 1) {

                    // Добавляем к строке символ табуляции
                    line += "\t";
                }
            }

            // Дописываем в файл строку
            stream << line << endl;
        }
        // Закрываем файл
        saveFile.close();
        return true;
    }

    return false;
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

    return new Canal();
}

/**
 * @brief Data::getNew
 * @param canal
 * @return
 */
Canal* Data::getNew(int canal)
{
    if (canal >= 0 && canal < this->_canalsNew.length()) {
        return this->_canalsNew[canal];
    }

    return new Canal();
}

/**
 * @brief Data::getCount
 * @return
 */
int Data::getCount() const
{
    return this->_canals.length();
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
    this->_canalsNew.clear();

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
                newCanal->setName("Канал № " + QString::number(canal + 1));
                this->_canals.push_back(newCanal);

                Canal *newCanal2 = new Canal();
                newCanal2->setName("Канал № " + QString::number(canal + 1));
                this->_canalsNew.push_back(newCanal2);
            }
        }

        // Для каждого канала
        for(int canal = 0; canal < this->_canals.length(); canal++)
        {
            // Вытаскиваем само число в виде строки
            QString str = list.at(canal);

            // Записываем в чанк число, заменив запятую на точку
            this->_canals[canal]->add(str.replace(",", ".").toFloat());
            this->_canalsNew[canal]->add(str.replace(",", ".").toFloat());
        }
    }
}
