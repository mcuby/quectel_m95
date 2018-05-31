import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        consoleApplication: true
        files: [
            "main.cpp",
            "serialport.cpp",
            "serialport.h",
            "serialportstate.cpp",
            "strategyouts.h",
        ]

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
