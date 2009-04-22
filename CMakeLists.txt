      project(kimberlite)

      find_package(KDE4 REQUIRED)
      include_directories( ${KDE4_INCLUDES}${QT_INCLUDE_DIR}/QtWebKit )

      set(kimberlite_SRCS
      main.cpp
      mainwindow.cpp
      src/CSSbeginnerWidget.cpp
      src/addProprietyWidget.cpp
      src/htmlParser.cpp
      src/cssParser.cpp
      src/stringToTemplate.cpp
      src/ProjectManager_v2.cpp
      src/rtfCssEditor.cpp
      src/rtfHtmlEditor.cpp
      src/template.cpp
      src/ressourcesManager.cpp
      src/newTable.cpp
      src/newPage.cpp
      src/newProject.cpp
      src/newScript.cpp
      src/debugger.cpp
      src/htmlThread.cpp
      )

      kde4_add_executable(kimberlite ${kimberlite_SRCS})

      target_link_libraries(kimberlite ${KDE4_KDEUI_LIBS}
      ${KDE4_KIO_LIBS} ${QT_QTSQL_LIBRARY} ${QT_WEBKIT_LIBRARY}${QT_LIBRARIES} QtWebKit)

      install(TARGETS kimberlite DESTINATION ${BIN_INSTALL_DIR})
      install( FILES tips kimberlite.db
      DESTINATION ${DATA_INSTALL_DIR}/kimberlite )

      install( FILES tips kimberlite.db
      DESTINATION ${DATA_INSTALL_DIR}/kimberlite/data )