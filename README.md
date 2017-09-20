# QuickDemoProject
This is a test project to get familiar with QTQuick usage. Application also uses QuickControls extension to increase data visualization. MVC basic approach is followed and behaviour is split among multiple entities.

Application visualizes information about host CPU, providing detailed information about its cores. Data is gathered via `proc/cpuinfo` file on Linux. Information about each Core of the CPU is shown on a seperate Tab on a TabView. Each tab consists of a TableView, where key-value pairs of CPU Cores are listed.

As an extension to the original idea, a special tab provides real-time information about current CPU Utilization. Information is gathered by continously reading data from `/proc/stat` and using a set of buffers to calculate delta changes on CPU cycle usages for different usage groups - per CPU Core respectively. In addition to numerical visualization, a colored rectangular shape helps the intuative perception of the percentage values.

# Prerequisities for Building
This application uses QTQuick and QTQuick Controls, where development was based on QT 5.6.1 on Ubuntu 16.10. In order to build the project, you will need to following:

* QT 5
* QtQuick 2.1
* QtQuick.Window 2.2
* QtQuick.Controls 1.0
* QTCreator 4.0.2 (qmake)
* GCC, make

# Development Environment
Development of this project has been established on a Microsoft Windows 10 64-bit Machine, making use of a Virtual Machine image running on free version of [VMWare Player 12](https://www.vmware.com/go/downloadplayer). VMWare Player OS image containing the Ubuntu 16.10 OS can be obtained at [osboxes.org](http://www.osboxes.org/ubuntu/#ubuntu-16-10-vmware). Enabling full VM support on Microsoft Windows 10 OS will require [extra steps](https://kb.vmware.com/kb/2146361).

### Configuring VM OS
In case VM option is preffered, here are the steps that can be taken to achieve a nice working environment. Firstly, optionally we can install open-vm libraries to enahnce VM interaction and achieve high desktop resolutions for convenience.

    sudo apt-get update
    sudo apt-get install open-vm-tools open-vm-tools-desktop

More importantly, in order to build and develop this project, necessary QT5 development libraries should be installed by following the commands below.

    sudo apt-get install qt5-default qtbase5-dev qtdeclarative5-dev qtcreator
    
After install, you can check if qmake --version prompts expected results.

In addition to QT, [git should be available](https://git-scm.com/download/linux) in order to access source code and use source control under Linux. QuickDemoProject should be able to run on any Linux environment where `/proc/cpuid` and `/proc/stat` files are available.

# Application Architecture and Classes

There is a conceptual seperation between the concepts of Data ownership, providing Data via Models to UI Elements and presentation of Data to user. Here is some further information about the classes and entities used in this application:

### InformationFetcher

This class does low level communication to host and prepares DataModel which will be used to provide CPU Core Data for further visualization. `void InformationFetcher::fetchCpuInfo()` method triggers accessing to Linux cpu information source and getting the data ready. Although not required, multiple calls to `void InformationFetcher::fetchCpuInfo()` would update the model data while keeping the instance, enabling UI to automatically update.

Depending on CPU core count, multiple data models are created. After getting core count information via `int InformationFetcher::getCoreCount() const`, each core's corresponding data model can be accessed via `QStandardItemModel* InformationFetcher::getCoreInfo(int coreIndex) const`.

### StatsFetcher

Very similar to InformationFetcher class, this class also deals with Linux sources and prepares data about current CPU Utilization for all Cores. Together with the call of `void StatsFetcher::startFetchingInfo()`, object starts to continously update the model where each CPU's Core utilization values are presented. If `void StatsFetcher::stopFetchingInfo()` is called or object is destroyed, timer mechanism is stopped and destroyed. After the call for starting cyclic updates, singular Model object can be accessed via `QStandardItemModel* StatsFetcher::getCoreStats() const`.

Within simple `StatsFetcherConfig` Enumeration found in `StatsFetcher.h` file, the size of the buffer to store measurement results can be configured by `StatBufferSize`, and the interval duration in milliseconds to collect cpu status can be configured by `StatsUpdateRateinMS`.

### InformationBinder

Models provided by the above mentioned classes can be bound to UI representations via InformationBinder class. This class is capable of dynamically loading QML files (templates) and associating the contexes with the provided Models respectively.

For each CPU Core `void InformationBinder::visualizeCoreInformation(unsigned int coreId, QStandardItemModel& coreDataModel)` is called to create a new Tab QML content, bind with model and attach to TabView in main window.

Once cyclic fetches start, Model provided by StatsFetcher can also be shown in its special Tab by calling `void InformationBinder::visualizeStats(QStandardItemModel& statsDataModel)`. After establishing the binding, data updates do happen automatically as StatsFetcher modifies the Model object continously.

### Testing

Due to its learning-oriented culture, this project yet unfortunately doesn't utilize any sort of test for acceptance. Due to relatively good seperation of function domains in this project, tests can be introduced easily and each and every class can be tested independently.

As a small hint, Linux command stress can be used to emulate CPU loads to check if CPU Utilization function is working correctly. Command can be installed and used as shown below:

    sudo apt-get install stress
    stress --cpu 4 --timeout 10  // Emulate stress on 4 cores for 10 Seconds


### QML Templates & Visualization

Except using main.qml for general layouting, there are 2 main dynamic usages of QML in this project

* For each CPU Core, a new Tab instance is created and dynamically loaded content is attached during run-time. QML contents are normally loaded lazy - meaning they are really prepared when they need to be shown. It was necessary to force-load them to properly bind data Models. Forcing a Tab content to be loaded is achieved by setting its `active` property to true manually. In this project there are 2 tab templates which can be loaded multiple times if desired.

* For percentage visualization on CPU Utilization tab, a delegate approach is used. In this small custom delegate code, data is simply processed and a rectangule shape is accordingly modified to achieve this numeric presentation.


# Known Issues
Below are the known issues of the application
* Due to a known isssue in QT, usage of TableView component causes the "QML Item: Binding loop detected for property 'width'" message although there is no such dependency loop in width usage in the application. Details: https://bugreports.qt.io/browse/QTBUG-50605



