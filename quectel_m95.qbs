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
            "strategyouts.cpp",
            "strategyouts.h",
        ]

        cpp.cxxFlags: ["-std=c++17"]

        Group {     // Properties for the produced executable

            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
