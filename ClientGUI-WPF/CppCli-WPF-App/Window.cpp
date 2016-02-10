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
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"

using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo()
{
	connected = false;

  // set event handler
  this->Loaded += gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing += gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  hConnectionButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::connectToServer);
  hDirectoryBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browsingDirectory);
  hFileUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadFile);
  hPatternSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchByPattern);
  hTextSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchByText);
  hFileSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchByFileName);
  hDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::downloadFiles);
  
  // set Window properties
   this->Title = "Client GUI";
  this->Width = 800;
  this->Height = 600;

  // attach dock panel to Window
  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);

  // setup Window controls and views
  setUpTabControl();
  setUpStatusBar();
  setUpConnectionView();
  setUpConnectionView1();
  setUpConnectionView2();
  setUpFileUploadView();
  setUpFileUploadView1();
  setUpFileDownloadView();
  setUpFileDownloadView1();
  setUpFileDownloadView2();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pChann_;
  delete pSendr_;
  delete pRecvr_;
}

//----< setup tab control for GUI >-----------------------------------------------
void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hFileDownloadTab->Header = "File Download";
  hFileUploadTab->Header = "File Upload";
  hConnectionTab->Header = "Connect";
  hTabControl->Items->Add(hConnectionTab);
  hTabControl->Items->Add(hFileUploadTab);
  hTabControl->Items->Add(hFileDownloadTab);
  if (!connected)
  {
	  hFileDownloadTab->IsEnabled = false;
	  hFileUploadTab->IsEnabled = false;
  }
}

//----< setup status bar for activity done over client >-----------------------------------------------
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = " Activity done over Client appers here";
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//----< setup connection tab  >-----------------------------------------------
void WPFCppCliDemo::setUpConnectionView()
{
	Console::Write("\n  Setting up: Connection view");

	hConnectGrid->Margin = Thickness(20);
	hConnectionTab->Content = hConnectGrid;

	RowDefinition^ hRowConnect = gcnew RowDefinition();
	hRowConnect->Height = GridLength(75);
	hConnectGrid->RowDefinitions->Add(hRowConnect);
	hClientLabel->Content = " Client Port Number";
	Border^ hBorderConnect = gcnew Border();
	hBorderConnect->Width = 120;
	hBorderConnect->Height = 30;
	hBorderConnect->BorderThickness = Thickness(1);
	hBorderConnect->BorderBrush = Brushes::Black;
	hBorderConnect->Child = hClientLabel;
	Border^ hBorder3C = gcnew Border();
	hBorder3C->Width = 120;
	hBorder3C->Height = 30;
	hBorder3C->BorderThickness = Thickness(1);
	hBorder3C->BorderBrush = Brushes::Black;
	hBorder3C->Child = hClientPort;
	hClientPort->Text = "1001";
	hStackPanelConnect->Children->Add(hBorderConnect);
	TextBlock^ hSpacerC = gcnew TextBlock();
	hSpacerC->Width = 10;
	hStackPanelConnect->Children->Add(hSpacerC);
	hStackPanelConnect->Children->Add(hBorder3C);
	hStackPanelConnect->Orientation = Orientation::Horizontal;
	hStackPanelConnect->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hConnectGrid->SetRow(hStackPanelConnect, 0);
	hConnectGrid->Children->Add(hStackPanelConnect);
}

void WPFCppCliDemo::setUpConnectionView1()
{
	RowDefinition^ hRowConnectD = gcnew RowDefinition();
	hRowConnectD->Height = GridLength(75);
	hConnectGrid->RowDefinitions->Add(hRowConnectD);
	hServerLabel->Content = " Server Port Number";

	Border^ hBorderConnectD = gcnew Border();
	hBorderConnectD->Width = 120;
	hBorderConnectD->Height = 30;
	hBorderConnectD->BorderThickness = Thickness(1);
	hBorderConnectD->BorderBrush = Brushes::Black;
	hBorderConnectD->Child = hServerLabel;

	Border^ hBorder3CD = gcnew Border();
	hBorder3CD->Width = 120;
	hBorder3CD->Height = 30;
	hBorder3CD->BorderThickness = Thickness(1);
	hBorder3CD->BorderBrush = Brushes::Black;
	hBorder3CD->Child = hServerPort;
	hStackPanelConnectD->Children->Add(hBorderConnectD);
	hServerPort->Text = "2001";

	TextBlock^ hSpacerCD = gcnew TextBlock();
	hSpacerCD->Width = 10;

	hStackPanelConnectD->Children->Add(hSpacerCD);
	hStackPanelConnectD->Children->Add(hBorder3CD);
	hStackPanelConnectD->Orientation = Orientation::Horizontal;
	hStackPanelConnectD->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hConnectGrid->SetRow(hStackPanelConnectD, 1);
	hConnectGrid->Children->Add(hStackPanelConnectD);
}

void WPFCppCliDemo::setUpConnectionView2()
{
	RowDefinition^ hRowConnection = gcnew RowDefinition();
	hRowConnection->Height = GridLength(75);
	hConnectGrid->RowDefinitions->Add(hRowConnection);
	hConnectionButton->Content = " Connect ";

	Border^ hBorderConnection = gcnew Border();
	hBorderConnection->Width = 120;
	hBorderConnection->Height = 30;
	hBorderConnection->BorderThickness = Thickness(1);
	hBorderConnection->BorderBrush = Brushes::Black;
	hBorderConnection->Child = hConnectionButton;

	hStackPanelConnection->Children->Add(hBorderConnection);
	hStackPanelConnection->Orientation = Orientation::Horizontal;
	hStackPanelConnection->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hConnectGrid->SetRow(hStackPanelConnection, 2);
	hConnectGrid->Children->Add(hStackPanelConnection);

	RowDefinition^ hRow3 = gcnew RowDefinition();
	hRow3->Height = GridLength(75);
	hConnectGrid->RowDefinitions->Add(hRow3);

	Border^ hBorderC = gcnew Border();
	hBorderC->Width = 400;
	hBorderC->BorderThickness = Thickness(1);
	hBorderC->BorderBrush = Brushes::Black;
	hBorderC->Child = hTextBlockC;
	hTextBlockC->Padding = Thickness(15);
	hTextBlockC->Text = "";

	hTextBlockC->FontFamily = gcnew Windows::Media::FontFamily("Times New Roman");
	hTextBlockC->FontWeight = FontWeights::Bold;
	hTextBlockC->FontSize = 16;
	hScrollViewerC->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewerC->Content = hBorderC;
	hConnectGrid->SetRow(hScrollViewerC, 3);
	hConnectGrid->Children->Add(hScrollViewerC);
}

//----< setup file upload tab >-----------------------------------------------
void WPFCppCliDemo::setUpFileUploadView()
{
	Console::Write("\n  Setting up: File Upload view");
	hFileListGrid->Margin = Thickness(20);
	hFileUploadTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(150);
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->Children->Add(hBorder1);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hDirectoryBrowseButton->Content = "Browse Directory";
	hDirectoryBrowseButton->Height = 30;
	hDirectoryBrowseButton->Width = 120;
	hDirectoryBrowseButton->BorderThickness = Thickness(2);
	hDirectoryBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hDirectoryBrowseButton, 1);
	hFileListGrid->Children->Add(hDirectoryBrowseButton);
}

void WPFCppCliDemo::setUpFileUploadView1()
{
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hFileListGrid->RowDefinitions->Add(hRow4Def);
	Border^ hBorder4 = gcnew Border();
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hTextBlock2;
	hTextBlock2->Padding = Thickness(15);
	hTextBlock2->Text = "";
	hTextBlock2->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock2->FontWeight = FontWeights::Bold;
	hTextBlock2->FontSize = 16;
	hScrollViewer4->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer4->Content = hBorder4;
	hFileListGrid->SetRow(hScrollViewer4, 2);
	hFileListGrid->Children->Add(hScrollViewer4);

	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow3Def);
	hFileUploadButton->Content = "Upload File";
	hFileUploadButton->Height = 30;
	hFileUploadButton->Width = 120;
	hFileUploadButton->BorderThickness = Thickness(2);
	hFileUploadButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFileUploadButton, 3);
	hFileListGrid->Children->Add(hFileUploadButton);
	hFileUploadButton->IsEnabled = false;

	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

//----< setup file search and download tab >-----------------------------------------------
void WPFCppCliDemo::setUpFileDownloadView()
{
  Console::Write("\n  Setting up: File Download View");
  hFileDownloadGrid->Margin = Thickness(20);
  hFileDownloadTab->Content = hFileDownloadGrid;
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  hRow1Def->Height = GridLength(150);
  hFileDownloadGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;

  hSearchResults->Items->Add("TESTFOLDER1");
  hSearchResults->Items->Add("TESTFOLDER2");
  hSearchResults->Items->Add("TESTFOLDER3");
  hSearchResults->SelectedIndex = 0;
  hBorder1->Child = hSearchResults;
  hFileDownloadGrid->SetRow(hBorder1, 0);
  hFileDownloadGrid->Children->Add(hBorder1);
  
  RowDefinition^ hRowDef2 = gcnew RowDefinition();
  hRowDef2->Height = GridLength(150);
  hFileDownloadGrid->RowDefinitions->Add(hRowDef2);

  Border^ hBorder3 = gcnew Border();
  hBorder3->Width = 60;
  hBorder3->Height = 30;
  hBorder3->BorderThickness = Thickness(1);
  hBorder3->BorderBrush = Brushes::Black;
  hBorder3->Child = hPatternTextBox;
  hPatternTextBox->Text = "*.*";

  Border^ hBorder5 = gcnew Border();
  hBorder5->Width = 120;
  hBorder5->Height = 30;
  hBorder5->BorderThickness = Thickness(1);
  hBorder5->BorderBrush = Brushes::Green;
  hPatternSearchButton->Content = "SEARCH BY PATTERN";
  hBorder5->Child = hPatternSearchButton;

  hStackPanel2->Children->Add(hBorder3);
  hStackPanel2->Children->Add(hBorder5);
}

void WPFCppCliDemo::setUpFileDownloadView1()
{
	Border^ hBorder7 = gcnew Border();
	hBorder7->Width = 15;
	hBorder7->Height = 15;
	hBorder7->BorderBrush = Brushes::Black;

	Border^ hBorder20 = gcnew Border();
	hBorder20->Width = 120;
	hBorder20->Height = 30;
	hBorder20->BorderThickness = Thickness(1);
	hBorder20->BorderBrush = Brushes::Black;
	hTextSearchButton->Content = "TEXT SEARCH";
	hBorder20->Child = hTextSearchButton;
	Border^ hBorder21 = gcnew Border();
	hBorder21->Width = 120;
	hBorder21->Height = 30;
	hBorder21->BorderThickness = Thickness(1);
	hBorder21->BorderBrush = Brushes::Black;
	hBorder21->Child = hTextSearchBox;

	Border^ hBorder27 = gcnew Border();
	hBorder27->Width = 15;
	hBorder27->Height = 15;
	hBorder27->BorderBrush = Brushes::Black;

	Border^ hBorder6 = gcnew Border();
	hBorder6->Width = 140;
	hBorder6->Height = 30;
	hBorder6->BorderThickness = Thickness(1);
	hBorder6->BorderBrush = Brushes::Black;
	hFileSearchButton->Content = "SEARCH BY FILENAME";
	hBorder6->Child = hFileSearchButton;
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 120;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hFileNameTextBox;

	hStackPanel2->Children->Add(hBorder7);
	hStackPanel2->Children->Add(hBorder21);
	hStackPanel2->Children->Add(hBorder20);
	hStackPanel2->Children->Add(hBorder27);
	hStackPanel2->Children->Add(hBorder4);
	hStackPanel2->Children->Add(hBorder6);
}

void WPFCppCliDemo::setUpFileDownloadView2()
{
	hStackPanel2->Orientation = Orientation::Horizontal;
	hFileDownloadGrid->SetRow(hStackPanel2, 1);
	hFileDownloadGrid->Children->Add(hStackPanel2);

	RowDefinition^ hRowDef3 = gcnew RowDefinition();
	hRowDef3->Height = GridLength(150);
	hFileDownloadGrid->RowDefinitions->Add(hRowDef3);

	Border^ hBorder8 = gcnew Border();
	hBorder8->Width = 150;
	hBorder8->Height = 30;
	hBorder8->BorderBrush = Brushes::Black;
	hBorder8->BorderThickness = Thickness(1);
	hDownloadButton->Content = "DOWNLOAD";
	hBorder8->Child = hDownloadButton;
	hDownloadButton->IsEnabled = false;

	Border^ hBorder10 = gcnew Border();
	hBorder10->Width = 50;
	hBorder10->Height = 30;
	hBorder10->BorderBrush = Brushes::Black;

	Border^ hBorder9 = gcnew Border();
	hBorder9->Width = 450;
	hBorder9->Height = 150;
	hBorder9->BorderBrush = Brushes::Black;
	hBorder9->BorderThickness = Thickness(1);
	hBorder9->Child = hFileListBox;
	hFileListBox->SelectedIndex = 0;

	hStackPanel3->Children->Add(hBorder9);
	hStackPanel3->Children->Add(hBorder10);
	hStackPanel3->Children->Add(hBorder8);
	hStackPanel3->Orientation = Orientation::Horizontal;
	hFileDownloadGrid->SetRow(hStackPanel3, 2);
	hFileDownloadGrid->Children->Add(hStackPanel3);
}

//----< connect to the server >-----------------------------------------------
void WPFCppCliDemo::connectToServer(Object^ obj, RoutedEventArgs^ args)
{
	String^ clientPort = hClientPort->Text;
	String^ destPort = hServerPort->Text;
	if (clientPort == "" || destPort == "")
	{
		return;
	}
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel("localhost", toStdString(clientPort), "localhost", toStdString(destPort), "ClientGUI-WPF\\TESTFOLDER");
	pChann_->start();
	pChann_->postMessage("STRING_HANDLING_REQUEST", "CONNECT");
	delete pObjFact;
	hTextBlockC->Text = "Client Connected to the Server";

	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();

	hFileDownloadTab->IsEnabled = true;
	hFileUploadTab->IsEnabled = true;
	connected = true;
	hStatus->Text = "Client with Port: "+ clientPort+ " Connected To Server on Port: " + destPort+ "";
}

//----< eowsing directory available at client >-----------------------------------------------
void WPFCppCliDemo::browsingDirectory(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for Directory";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox->Items->Add(L"<> " + dirs[i]);
		hFileUploadButton->IsEnabled = true;
	}
	std::cout << "\n  Browsing for Directory";
	hStatus->Text = "Browsing for Directory";
}

//----< perform uploading file selected by user >-----------------------------------------------
void WPFCppCliDemo::uploadFile(Object^ obj, RoutedEventArgs^ args)
{
	if (hListBox->SelectedIndex==-1)
	{
		hTextBlock2->Text += "First Select Any file Using Browse Directory Button";
		hStatus->Text = "First Select Any file Using Browse Directory Button";
	}
	else
	{
		hTextBlock2->Text = "Sending File to the Server \n";
		hStatus->Text = "Sending File to the Server \n";

		std::string filename = toStdString((String^)hListBox->SelectedItem);
		pChann_->postMessage("FILE_HANDLING_REQUEST", filename);

		hTextBlock2->Text += "" + hListBox->SelectedItem + " SENT to the Server \n";
		hStatus->Text = "" + hListBox->SelectedItem + " SENT to the Server \n";
	}
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

//----< perform dowloading of file selected by user >-----------------------------------------------
void WPFCppCliDemo::downloadFiles(Object^ obj, RoutedEventArgs^ args)
{
	if (hFileListBox->SelectedIndex == -1)
	{
		hStatus->Text = "First Select Any file to Download";
	}
	else
	{
		std::string path = toStdString((String^)hSearchResults->SelectedItem);
		std::string filename = toStdString((String^)hFileListBox->SelectedItem);
		pChann_->postMessage("FILE_DOWNLOAD_REQUEST", path + "\\" + filename);
	}

}

//----< search file by filename which reside at server side >-----------------------------------
void WPFCppCliDemo::searchByFileName(Object^ obj, RoutedEventArgs^ args)
{
	std::string path = toStdString((String^)hSearchResults->SelectedItem);
	std::string pattern = toStdString((String^)hFileNameTextBox->Text);
	std::cout << "PAT" << pattern;
	auto index = pattern.find(".");
	if (index == std::string::npos)
		pattern.append(".*");
	std::cout << pattern;
	pChann_->postMessage("FILE_SEARCH_PATTERN", path + "\\" + pattern);
}

//----< search file by search text which reside at server side >-----------------------------------
void WPFCppCliDemo::searchByText(Object^ obj, RoutedEventArgs^ args)
{
	std::string path = toStdString((String^)hSearchResults->SelectedItem);
	std::string pattern = toStdString((String^)hPatternTextBox->Text);
	std::string text = toStdString((String^)hTextSearchBox->Text);
	pChann_->postMessage("FILE_TEXT_SEARCH", path + "\\" + pattern+"\\"+text);
}

//----< search file by pattern which reside at server side >-----------------------------------
void WPFCppCliDemo::searchByPattern(Object^ obj, RoutedEventArgs^ args)
{
	std::string path = toStdString((String^)hSearchResults->SelectedItem);
	std::string pattern = toStdString((String^)hPatternTextBox->Text);
	pChann_->postMessage("FILE_SEARCH_PATTERN", path+"\\"+pattern);
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

//----< add data to GUI when called by dispatcher >-----------------------------------
void WPFCppCliDemo::addText(String^ msg)
{
	hFileListBox->Items->Clear();
	std::string text = toStdString(msg);
	if (text == "FILE_DOWNLOAD_OK")
	{
		hStatus->Text = "File Downloaded Successfully";
		return;
	}
	if (text == "FILE_HANDLING_REPLY")
	{
		hStatus->Text = "File Uploaded Successfully";
		return;
	}
	if (text.size() == 0)
	{
		hStatus->Text = "NO FILES FOUND for selected Search";
		hDownloadButton->IsEnabled = false;
		return;
	}
	else
	{
		auto index = text.find("\n");
		while (index != std::string::npos)
		{
			std::string p = text.substr(0, index);
			text = text.substr(index + 1, text.length() - 1);
			hFileListBox->Items->Add(toSystemString(p));
			index = text.find("\n");
		}
		hFileListBox->SelectedIndex = 0;
		int count = hFileListBox->Items->Count;
		hStatus->Text = "Total "+ count +" Files Found for Selected Search";
		if (hFileListBox->HasItems)
			hDownloadButton->IsEnabled = true;
	}
}

//----< get message from receiving queue >-----------------------------------
void WPFCppCliDemo::getMessage()
{
  // recvThread runs this function
  while (true)
  {
    std::cout << "\n  receive thread calling getMessage()";
	std::string msg = pChann_->getMessage();
    String^ sMsg = toSystemString(msg);
    array<String^>^ args = gcnew array<String^>(1);
    args[0] = sMsg;

    Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
    Dispatcher->Invoke(act, args);  // must call addText on main UI thread
  }
}


void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n  Window closing");
}


[STAThread]
int main(array<System::String^>^ args)
{
  Console::WriteLine(L"\n Starting WPFCppCliDemo");

  Application^ app = gcnew Application();
  app->Run(gcnew WPFCppCliDemo());
  Console::WriteLine(L"\n\n");
}