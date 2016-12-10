# PROTECTED STORAGE / ЗАЩИЩЕННОЕ ХРАНИЛИЩЕ

## О программе

Protected storage 0.97 RC

Программа создана на языке C++ с использованием библиотеки FUSE и фреймворка Qt

Обеспечивает создание и использование защищенного хранилища с любыми данными. Смонтированная директория хранилища воспринимается системой как обычный логический диск.

## Конфигурация и запуск

### Внимание: данная программа в силу используемой библиотеки предназначена для работы только на linux-системах. Для запуска вам необходимо установить Qt (https://www.qt.io) и библиотеку FUSE (https://github.com/libfuse/libfuse/releases/tag/fuse-2.9.7). 

Если у вас установлена IDE Qt Creator, просто откройте как проект файл ProtectedStorage.pro и запустите программу.


## Как работать с программой

Прежде всего, заполните все поля:

* Root directory - папка, в которой вы хотите хранить или уже хранятся ваши зашифрованные данные

* Mount directory - папка, в которой вы хотите работать с ними.

* Password - необходим для шифрования и дешифрования данных. Длина должна быть не менее 8 символов.

Далее, нажмите на кнопку "Create storage" для создания хранилища.

Для прекращения работы нажмите "Destroy storage".

#### Внимание: вы не можете создавать более одного хранилища одновременно.

#### В случае ошибки программа сообщит об этом и предложит пути решения.

## 0.97 bug: 
при использовании текстового редактора Kate могут возникать проблемы при сохранении. Используйте другие текстовые редакторы.



## Надеюсь, использование данной программы будет для вас удобным и полезным.



# PROTECTED STORAGE

## About

Protected storage 0.97 RC

Created on C++ using FUSE library and Qt

Provides the functionality of creating and using protected storage with any data.
The mounted directory is similar to an regular logical disk.


## Configure

### Remember: this program can run only on linux systems. You also need Qt framework (https://www.qt.io) and FUSE library (https://github.com/libfuse/libfuse/releases/tag/fuse-2.9.7) installed on your system. 

If you have Qt Creator IDE, simply open as project ProtectedStorage.pro and run.


## How to use

At first, fill in all the fields:

* Root directory - where you want to store your encrypted data.

* Mount directory - where you want to work with it.

* Password - need to encrypt and decrypt. It's length should be at least 8 characters.

Then, push the button "Create storage" to create it.

To destroy the storage, push the button "Destroy storage".

#### Remember: you can't create more than one storage at the same time.

#### If an error occurs, the program will report about it and offer some solutions.

## 0.97 bug:
If you use Kate text editor, you can get some problems while saving the files. Use another text editor.

## Wish you will find the using of this program convenient and useful.

### 2016, Tarasov Kirill, https://github.com/Tarasow/ProtectedStorage

