#pragma once

#include "common.h"
#include "General/SAction.h" // TODO: Remove after SActionHandler split

class SLADELog : public wxLog
{
protected:
// wx2.9.x is no longer supported.
#if (wxMAJOR_VERSION < 3)
#error This will not compile with wxWidgets older than 3.0.0 !
#endif
	void DoLogText(const wxString& msg) override;

public:
	SLADELog() {}
	~SLADELog() {}
};

class MainWindow;
class SAction;
class SActionHandler;
class wxSingleInstanceChecker;
class MainAppFileListener;

class SLADEWxApp : public wxApp
{
friend class SActionHandler;
private:
	MainWindow*					main_window;
	vector<SActionHandler*>		action_handlers;
	bool						init_ok;
	wxStopWatch					timer;
	wxSingleInstanceChecker*	single_instance_checker;
	MainAppFileListener*		file_listener;
	bool						save_config;

public:
	SLADEWxApp();
	~SLADEWxApp();

	bool OnInit() override;
	int OnExit() override;
	void OnFatalException() override;

#ifdef __APPLE__
	virtual void MacOpenFile(const wxString &fileName);
#endif // __APPLE__

	MainWindow*	getMainWindow() { return main_window; }

	bool	singleInstanceCheck();
	void	initLogFile();
	void	initActions();
	void	readConfigFile();
	void	saveConfigFile();
	bool	isInitialised() { return init_ok; }
	void	checkForUpdates(bool message_box);
	void	exitApp(bool save_config = true);

	bool	doAction(string id, int wx_id_offset = 0);

	void	onMenu(wxCommandEvent& e);
	void	onVersionCheckCompleted(wxThreadEvent& e);
	void	onActivate(wxActivateEvent& event);
};

#define theApp ((SLADEWxApp*)wxTheApp)
