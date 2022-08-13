#include "BrowsePage.h"
#include "Pages.h"
#include "../Messenger.h"
#include "../Controls/DownloadDialog.h"
#include "../../Helpers/ThemeHelpers.h"
#include "../../Models/Configuration.h"

using namespace NickvisionTubeConverter::Helpers;
using namespace NickvisionTubeConverter::Models;
using namespace NickvisionTubeConverter::UI;
using namespace NickvisionTubeConverter::UI::Controls;

namespace NickvisionTubeConverter::UI::Views
{
	BrowsePage::BrowsePage(QWidget* parent) : QWidget{ parent }
	{
		//==UI==//
		//WebView Dark Mode
		if (Configuration::getInstance().getTheme() == Theme::Dark)
		{
			qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--force-dark-mode");
		}
		//Setup UI
		m_ui.setupUi(this);
		//Ribbon
		m_ui.ribbon->setCurrentIndex(0);
		//Download Button
		m_ui.btnDownload->setVisible(false);
		//==Theme==//
		refreshTheme();
	}

	void BrowsePage::refreshTheme()
	{
		m_ui.ribbon->setStyleSheet(ThemeHelpers::getThemedRibbonStyle());
	}

	void BrowsePage::showEvent(QShowEvent* event)
	{
		QWidget::showEvent(event);
		m_ui.webView->setUrl({ "https://www.youtube.com" });
	}

	void BrowsePage::hideEvent(QHideEvent* event)
	{
		QWidget::hideEvent(event);
		m_ui.webView->setUrl({});
	}

	void BrowsePage::on_btnBack_clicked()
	{
		m_ui.webView->back();
	}

	void BrowsePage::on_btnForward_clicked()
	{
		m_ui.webView->forward();
	}

	void BrowsePage::on_btnRefresh_clicked()
	{
		m_ui.webView->reload();
	}

	void BrowsePage::on_btnHome_clicked()
	{
		m_ui.webView->setUrl({ "https://www.youtube.com" });
	}

	void BrowsePage::on_btnDownload_clicked()
	{
		DownloadDialog downloadDialog{ this, m_ui.txtUrl->text() };
		int result{ downloadDialog.exec() };
		if (result == QDialog::Accepted)
		{
			Download download{ downloadDialog.getDownload() };
			Pages downloadsPage{ Pages::Downloads };
			Messenger::getInstance().sendMessage("DownloadsPage.addDownload", &download);
			Messenger::getInstance().sendMessage("MainWindow.changePage", &downloadsPage);
		}
	}

	void BrowsePage::on_webView_urlChanged(const QUrl& url)
	{
		m_ui.txtUrl->setText(url.toString());
		m_ui.btnDownload->setVisible(url.toString().startsWith("https://www.youtube.com/watch?v="));
		m_ui.ribbon->setCurrentIndex(url.toString().startsWith("https://www.youtube.com/watch?v=") ? 1 : 0);
	}
}
