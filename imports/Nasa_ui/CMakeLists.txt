### This file is automatically generated by Qt Design Studio.
### Do not change

qt_add_library(Nasa_ui STATIC)
set_source_files_properties(Constants.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
    )

qt6_add_qml_module(Nasa_ui
    URI "Nasa_ui"
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
        QML_FILES
        Constants.qml
        DataStore.qml
        DirectoryFontLoader.qml
        EventListModel.qml
        EventListSimulator.qml
        JsonData.qml
    RESOURCES
        data.json
        models.json
)
