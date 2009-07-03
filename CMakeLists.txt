project(kimberlite)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )
cmake_minimum_required(VERSION 2.6)

find_package(KDE4 4.1.0 REQUIRED)
find_package(Qt4 4.5.0 REQUIRED)
include(KDE4Defaults)

      find_package(KDE4 REQUIRED)
      include_directories( ${KDE4_INCLUDES}${QT_INCLUDE_DIR}/QtWebKit )

      set(kimberlite_SRCS
      main.cpp
      mainwindow.cpp
      src/CSSbeginnerWidget.cpp
      src/addProprietyWidget.cpp
      src/htmlParser.cpp
      src/cssParser.cpp
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
      src/miniClasses.h
      src/colorComboBox.cpp
      src/headerComboBox.cpp
      )

      kde4_add_executable(kimberlite ${kimberlite_SRCS})

      target_link_libraries(kimberlite ${KDE4_KDEUI_LIBS}
      ${KDE4_KIO_LIBS} ${QT_QTSQL_LIBRARY} ${QT_WEBKIT_LIBRARY}${QT_LIBRARIES} QtWebKit)

      install(TARGETS kimberlite DESTINATION ${BIN_INSTALL_DIR})
      install( FILES 
	tips 
	kimberlite.db
      DESTINATION ${DATA_INSTALL_DIR}/kimberlite )

      install( FILES 
	tips 
	kimberlite.db 
	data/home.htm
      DESTINATION ${DATA_INSTALL_DIR}/kimberlite/data )
      
      install( FILES 
	template/default.wkp
      DESTINATION ${DATA_INSTALL_DIR}/kimberlite/template )
      
      install( FILES 
	  pixmap/tag.png
	  pixmap/state.png
	  pixmap/id.png
	  pixmap/class.png
	  pixmap/tag.png
	  pixmap/logo.png
	  pixmap/showCSS.png
	  pixmap/showDebugger.png
	  pixmap/showHTML.png
	  pixmap/showInspector.png
	  pixmap/showProject.png
	  pixmap/anchor.png
      DESTINATION ${DATA_INSTALL_DIR}/kimberlite/pixmap )
      
      install( FILES 
	  pixmap/tags/br.png
	  pixmap/tags/hr.png
	  pixmap/tags/space.png
	  pixmap/tags/tab.png
	  pixmap/tags/textarea.png
	  pixmap/tags/checkbox.png
	  pixmap/tags/radio.png
	  pixmap/tags/password.png
	  pixmap/tags/button.png
	  pixmap/tags/combobox.png
	  pixmap/tags/hidden.png
	  pixmap/tags/label.png
	  pixmap/tags/list.png
	  pixmap/tags/reset.png
	  pixmap/tags/submit.png
	  pixmap/tags/upload.png
	  pixmap/tags/textline.png
      DESTINATION ${DATA_INSTALL_DIR}/kimberlite/pixmap/tags/ )
      
      install( FILES 
	  pixmap/browserLogo/IE.png
	  pixmap/browserLogo/FF.png
	  pixmap/browserLogo/KQ.png
	  pixmap/browserLogo/SF.png
	  pixmap/browserLogo/CR.png
	  pixmap/browserLogo/OP.png
	  pixmap/browserLogo/w3c.png
      DESTINATION ${DATA_INSTALL_DIR}/kimberlite/pixmap/browserLogo/ )

      install( FILES 
	  pixmap/about/bar-bottom-middle.png
	  pixmap/about/box-top-right.png
	  pixmap/about/kde_infopage_rtl.css
	  pixmap/about/box-top-middle.png
	  pixmap/about/bottom-right.png
	  pixmap/about/box-bottom-right.png
	  pixmap/about/bar-top-left.png
	  pixmap/about/bar-middle-right.png
	  pixmap/about/bar-top-middle.png
	  pixmap/about/bar-bottom-right.png
	  pixmap/about/bar-middle-left.png
	  pixmap/about/box-bottom-middle.png
	  pixmap/about/bottom-left.png
	  pixmap/about/top-middle.png
	  pixmap/about/bottom-middle.png
	  pixmap/about/bar-bottom-left.png
	  pixmap/about/box-middle-left.png
	  pixmap/about/box-middle-right.png
	  pixmap/about/kde_infopage.css
	  pixmap/about/box-bottom-left.png
	  pixmap/about/box-top-left.png
	  pixmap/about/top-right.png
	  pixmap/about/bar-top-right.png
      DESTINATION ${DATA_INSTALL_DIR}/kimberlite/pixmap/about/ )

      
      install( FILES  
	kimberlite.desktop 
      DESTINATION ${XDG_APPS_INSTALL_DIR} )
      
      
      add_subdirectory( pixmap/logo )
      add_subdirectory( pixmap/icon )
