/***************************************************************
 * Name:      prvMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-11-20
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "prvMain.h"
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include "channelmanager.h"

//(*InternalHeaders(prvDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(prvDialog)
const long prvDialog::ID_PANEL1 = wxNewId();
const long prvDialog::ID_PANEL5 = wxNewId();
const long prvDialog::ID_PANEL3 = wxNewId();
const long prvDialog::ID_M_PSWP1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(prvDialog,wxDialog)
    //(*EventTable(prvDialog)
    //*)
END_EVENT_TABLE()

prvDialog::prvDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(prvDialog)
    Create(parent, id, _("PRV"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
    SetClientSize(wxSize(1280,800));
    m_pswpMain = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,0), wxSize(1280,800), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
    m_pswpMain->SetPageNameStyle(0);
    m_pppnlSplash = new wxPanel(m_pswpMain, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    m_pppnlSplash->SetBackgroundColour(wxColour(128,0,0));
    m_ppnlVLC = new pnlPlayer(m_pswpMain, ID_PANEL5, wxPoint(0,0), wxSize(600,480), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    m_ppnlSearch = new wxPanel(m_pswpMain, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    m_ppnlSearch->SetBackgroundColour(wxColour(0,0,255));
    m_pswpMain->AddPage(m_pppnlSplash, _("Splash"), false);
    m_pswpMain->AddPage(m_ppnlVLC, _("Player"), false);
    m_pswpMain->AddPage(m_ppnlSearch, _("Search"), false);
    //*)

    m_pswpMain->ChangeSelection(wxT("Player"));


    ChannelManager::Get().LoadDVBList(wxFileName(wxT("channels.xml")));
    ChannelManager::Get().SaveDVBList(wxFileName(wxT("prv_channels.xml")));


}

prvDialog::~prvDialog()
{
    //(*Destroy(prvDialog)
    //*)
}

void prvDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void prvDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void prvDialog::OnlstChannelsSelected(wxCommandEvent& event)
{
}

void prvDialog::OnlstSortSelected(wxCommandEvent& event)
{
}
