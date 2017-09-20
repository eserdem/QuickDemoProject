# QuickDemoProject
This is a test project to get familiar with QTQuick usage. Application uses QuickControls extension to increase data visualization. MVC basic approach is followed and behaviour is split among multiple entities.

Application visualizes information about host CPU, providing detailed information about its cores.

# How-to Build
This application uses QTQuick and QTQuick Controls, where development was based on QT 5.6.1 on Ubuntu 16.10. In order to build the project, you will need to following:

* QT 5.5
* QtQuick 2.5
* QtQuick.Window 2.2
* QtQuick.Layouts 1.3
* QtQuick.Controls 1.5
* QTCreator 4.0.2 (qmake)
* GCC, make

# Suggested Development Environment
Development of this project has been established on a Microsoft Windows Machine, making use of a Virtual Machine image running on [VMWare Player 12](https://www.vmware.com/go/downloadplayer). Image containing the Ubuntu 16.10 OS can be obtained at [osboxes.org](http://www.osboxes.org/ubuntu/#ubuntu-16-10-vmware).

### Configuring VM OS
In case VM option is preffered, here are the steps that can be taken to achieve a nice working environment. Firstly, optionally we can install open-vm libraries to enahnce VM interaction and achieve high desktop resolutions for convenience.

    sudo apt-get update
    sudo apt-get install open-vm-tools
    sudo apt-get install open-vm-tools-desktop

More importantly, in order to build and develop this project, necessary QT5 development libraries should be installed by following the commands below.

    sudo apt-get install qt5-default qtbase5-dev qtdeclarative5-dev qtcreator
    
After install, you can check if qmake --version prompts expected results.

# Run-time Requirements
Application should be able to run on any Linux environment where /proc/cpuid file is available.

# Application Architecture and Classes

Here is some information about the classes used in this application

### InformationFetcher

This class does low level communication to host and prepares DataModel which will be used to provide data for further visualization

### InformationBinder

This class connects the data model to the view component, and allocates the UI elements as necessary to achieve correct data visualization.

# Known Issues
Below are the known issues of the application
* Due to a known isssue in QT, usage of TableView component causes the "QML Item: Binding loop detected for property 'width'" message although there is no such dependency loop in width usage in the application. Details: https://bugreports.qt.io/browse/QTBUG-50605


