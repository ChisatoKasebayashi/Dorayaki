
##環境構築

* ####Qt creater を使用  
  - プロジェクトの新規作成  
  - 作成した.proの文末に以下を追加 くコ:彡  

    `#OpenCV setting`  
    `win32:DEPENDPATH += "C:/OpenCV249/include"`  
    `win32:INCLUDEPATH += "C:/OpenCV249/include"`  
    `win32:LIBS += -L"C:/OpenCV249/x86/vc11/lib"`  
    `win32:LIBS += -lopencv_core249 -lopencv_highgui249 -lopencv_imgproc249`  

  - ビルド->qmakeの実行
