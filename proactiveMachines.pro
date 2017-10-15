
TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

# Include path for OpenCV 2.4
#INCLUDEPATH += C:\Workspace\libs\opencv\build\include

# Include path for Opencv 3.1. The one that caffe uses
INCLUDEPATH += C:\Users\rilao\.caffe\dependencies\libraries_v140_x64_py27_1.1.0\libraries\include
# Include path for Caffe
INCLUDEPATH += C:\Workspace\libs\caffe\include
INCLUDEPATH += C:\Workspace\libs\caffe\build\include
INCLUDEPATH += C:\Workspace\libs\caffe\build



SOURCES += main.cpp \
    imageExtractorClass.cpp \
    digitClassifierClass.cpp

HEADERS  += mainwindow.h \
    imageExtractorClass.h \
    digitClassifierClass.h



GLOG_minloglevel=2


CONFIG(release, debug|release){

    message(Release build!)

#OpenCV 2.4
#    LIBS += -LC:\Workspace\libs\opencv\build\x64\vc14\lib    -lopencv_core2413 \
#                                                                        -lopencv_highgui2413   \
#                                                                        -lopencv_imgproc2413

}
CONFIG(debug, debug|release){

    message(Debug build!) #no print

#OpenCV 2.4
#    LIBS += -LC:\Workspace\libs\opencv\build\x64\vc14\lib   -lopencv_core2413d \
#                                                            -lopencv_imgproc2413d \
#                                                            -lopencv_highgui2413d   \
#                                                            -lopencv_imgproc2413d

#OpenCV 3.1

    LIBS += -LC:\Workspace\libs\caffe\build\lib\3rdPartyLibs    -lopencv_core310d \
                                                                        -lopencv_highgui310d   \
                                                                        -lopencv_imgcodecs310d   \
                                                                        -lopencv_imgproc310d    \
                                                                        -lopencv_videoio310d

    LIBS += -LC:\Workspace\libs\caffe\build\lib\Debug -lcaffe-d\
                                                        -lcaffeproto-d

    LIBS += -LC:\Workspace\libs\caffe\build\lib\3rdPartyLibs    -lboost_system-vc140-mt-gd-1_61     \
                                                                        -lboost_thread-vc140-mt-gd-1_61     \
                                                                        -lboost_filesystem-vc140-mt-gd-1_61     \
                                                                        -lboost_chrono-vc140-mt-gd-1_61     \
                                                                        -lboost_date_time-vc140-mt-gd-1_61     \
                                                                        -lboost_atomic-vc140-mt-gd-1_61     \
                                                                        -lboost_python-vc140-mt-gd-1_61    \
                                                                        -lglogd     \
                                                                        -lgflagsd     \
                                                                        -llibprotobufd     \
                                                                        -lcaffehdf5_hl_D     \
                                                                        -lcaffehdf5_D     \
                                                                        -lcaffezlib     \
                                                                        -llmdbd     \
                                                                        -lsnappy_staticd \
                                                                        -lleveldbd

    LIBS += C:\Workspace\libs\caffe\build\lib\3rdPartyLibs\libopenblas.dll.a

    LIBS += -lntdll \
            -lshlwapi
}































