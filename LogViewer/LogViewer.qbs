import qbs.FileInfo

QtApplication {
    Depends { name: "Qt.widgets" }

    cpp.defines: [
        // You can make your code fail to compile if it uses deprecated APIs.
        // In order to do so, uncomment the following line.
        //"QT_DISABLE_DEPRECATED_BEFORE=0x060000" // disables all the APIs deprecated before Qt 6.0.0
    ]

    cpp.includePaths: "inc/Logic"
    files: [
        "QCheckableComboBox.h",
        "logentryview.cpp",
        "logentryview.h",
        "logentryview.ui",
        "main.cpp",
        "logviewer.cpp",
        "logviewer.h",
        "logviewer.ui",
        "LogViewer_en_BE.ts",
        "QLogFileWidget.h",
        "Logic/clogfile.cpp",
        "Logic/clogfile.h",
        "Logic/cfunctracer.cpp",
        "Logic/ctracer.cpp",
        "Logic/cscopedtimer.cpp",
        "Logic/cconversion.cpp",
        "inc/Logic/cfunctracer.h",
        "inc/Logic/ctracer.h",
        "inc/Logic/cscopedtimer.h",
        "inc/Logic/cconversion.h",
        "searchform.cpp",
        "searchform.h",
        "searchform.ui",
    ]

    Group {
        fileTagsFilter: "qm"
        Qt.core.resourcePrefix: "/i18n"
        fileTags: "qt.core.resource_data"
    }

    install: true
    installDir: qbs.targetOS.contains("qnx") ? FileInfo.joinPaths("/tmp", name, "bin") : base
}
