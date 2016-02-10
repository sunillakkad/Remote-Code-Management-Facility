#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////
// Server1.cpp - C++\CLI implementation of WPF Application           //
//               Demo for CSE 687 Project #4						 //
// ver 2.1                                                           //
// Language:    C++, Visual Studio 13.0						         //
// Platform:    Dell Inspiron N5010, Win7 Professional, SP1          //
// Application: Demonstration for CSE 687, Project #4, Spring 2015   //
// Author:      Sunilkumar Lakkad, Syracuse University               //
//              (315) 751-5834, lakkads1@gmail.com                   //
// Source:		Prof. Jim Fawcett, Syracuse University				 //
//				(315) 443 - 3948, jfawcett@twcny.rr.com			     //
///////////////////////////////////////////////////////////////////////

/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It 
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 2.1 : 27 Apr 2015
*  - added file uploadin and downloading Tab
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>

namespace CppCliWindows
{
  ref class WPFCppCliDemo : Window
  {

    // MockChannel references
    ISendr* pSendr_;
    IRecvr* pRecvr_;
    IMockChannel* pChann_;

    // Controls for Window
    DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
    Grid^ hGrid = gcnew Grid();                    
    TabControl^ hTabControl = gcnew TabControl();
    TabItem^ hFileDownloadTab = gcnew TabItem();
    TabItem^ hFileUploadTab = gcnew TabItem();
    TabItem^ hConnectionTab = gcnew TabItem();
    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();

	//Controls for Connect
	TextBox^ txtBox = gcnew TextBox();
	Grid^ hConnectGrid = gcnew Grid();
	StackPanel^ hStackPanelConnect = gcnew StackPanel();
	StackPanel^ hStackPanelConnectD = gcnew StackPanel();
	StackPanel^ hStackPanelConnection = gcnew StackPanel();
	Button^ hConnectionButton = gcnew Button();
	Label^ hClientLabel = gcnew Label();
	TextBox^ hClientPort = gcnew TextBox();
	Label^ hServerLabel = gcnew Label();
	TextBox^ hServerPort = gcnew TextBox();
	TextBlock^ hTextBlockC = gcnew TextBlock();
	ScrollViewer^ hScrollViewerC = gcnew ScrollViewer();

	// Controls for File Upload
	Grid^ hFileListGrid = gcnew Grid();
	Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
	ListBox^ hListBox = gcnew ListBox();
	Button^ hDirectoryBrowseButton = gcnew Button();
	Button^ hFileUploadButton = gcnew Button();
	TextBlock^ hTextBlock2 = gcnew TextBlock();
	ScrollViewer^ hScrollViewer4 = gcnew ScrollViewer();
	Grid^ hGrid2 = gcnew Grid();

	//Controls for file download
	Grid^ hFileDownloadGrid = gcnew Grid();
	Button^ hFileSearchButton = gcnew Button();
	Button^ hPatternSearchButton = gcnew Button();
	ListBox^ hSearchResults = gcnew ListBox();
	Button^  hDownloadButton = gcnew Button();
	Button^  hTextSearchButton = gcnew Button();
	ScrollViewer^ hScrollViewer2 = gcnew ScrollViewer();
	TextBox^ hPatternTextBox = gcnew TextBox();
	TextBox^ hFileNameTextBox = gcnew TextBox();
	TextBox^ hTextSearchBox = gcnew TextBox();
	ListBox^ hFileListBox = gcnew ListBox();
	StackPanel^ hStackPanel2 = gcnew StackPanel();
	StackPanel^ hStackPanel3 = gcnew StackPanel();

    // receive thread
    Thread^ recvThread;

  public:
    WPFCppCliDemo();
    ~WPFCppCliDemo();

    void setUpStatusBar();
    void setUpTabControl();
	void setUpConnectionView();
	void setUpConnectionView1();
	void setUpConnectionView2();
	void setUpFileUploadView();
	void setUpFileUploadView1();
	void setUpFileDownloadView();
	void setUpFileDownloadView1();
	void setUpFileDownloadView2();

	void connectToServer(Object^ obj, RoutedEventArgs^ args);
	void browsingDirectory(Object^ sender, RoutedEventArgs^ args);
	void uploadFile(Object^ ibj, RoutedEventArgs^ args);
	void searchByPattern(Object^ obj, RoutedEventArgs^ args);
	void searchByText(Object^ obj, RoutedEventArgs^ args);
	void searchByFileName(Object^ obj, RoutedEventArgs^ args);
	void downloadFiles(Object^ obj, RoutedEventArgs^ args);

    void addText(String^ msg);
    void getMessage();
    void OnLoaded(Object^ sender, RoutedEventArgs^ args);
    void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	
  private:
    std::string toStdString(String^ pStr);
    String^ toSystemString(std::string& str);
	bool connected;
  };
}
#endif
