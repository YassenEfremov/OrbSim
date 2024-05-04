#include <wx/wx.h>

class AFrame : public wxFrame {

public:
	AFrame();

private:
	void OnHello(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
};
