#include "pnlPlayer.h"

#include <vlc/vlc.h>
#include <climits>
#include <wx/log.h>
#include "channelmanager.h"
#include "settingevent.h"
#include "settings.h"
#include "icons/backspace.xpm"
#ifdef __WXGTK__
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
//#include <wx/gtk/win_gtk.h>
//#define GET_XID(window) GDK_WINDOW_XWINDOW(GTK_PIZZA(window->m_wxwindow)->bin_window)
#endif

using namespace std;

//(*InternalHeaders(pnlPlayer)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlPlayer)
const long pnlPlayer::ID_M_PLBL1 = wxNewId();
const long pnlPlayer::ID_M_PLST1 = wxNewId();
const long pnlPlayer::ID_M_PLST2 = wxNewId();
const long pnlPlayer::ID_PANEL4 = wxNewId();
const long pnlPlayer::ID_PANEL1 = wxNewId();
const long pnlPlayer::ID_M_PLST3 = wxNewId();
const long pnlPlayer::ID_M_PBTN1 = wxNewId();
const long pnlPlayer::ID_M_PBTN2 = wxNewId();
const long pnlPlayer::ID_PANEL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlPlayer,pmPanel)
	//(*EventTable(pnlPlayer)
	//*)
END_EVENT_TABLE()

pnlPlayer::pnlPlayer(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size,long nStyle, wxString sEmpty)
{
	//(*Initialize(pnlPlayer)
	Create(parent, id, wxPoint(0,0), wxSize(1280,800), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_ppnlChannels = new wxPanel(this, ID_PANEL4, wxPoint(1000,0), wxSize(280,800), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	m_ppnlChannels->SetBackgroundColour(wxColour(0,0,0));
	m_plblFilter = new wmLabel(m_ppnlChannels, ID_M_PLBL1, _("Frequency"), wxPoint(5,5), wxSize(270,40), 0, _T("ID_M_PLBL1"));
	m_plblFilter->SetBorderState(uiRect::BORDER_DOWN);
	m_plblFilter->GetUiRect().SetGradient(0);
	m_plblFilter->SetBackgroundColour(wxColour(255,255,255));
	m_plstChannels = new wmList(m_ppnlChannels, ID_M_PLST1, wxPoint(0,50), wxSize(280,700), wmList::STYLE_SELECT, 1, wxSize(-1,30), 1, wxSize(-1,-1));
	m_plstChannels->SetBackgroundColour(wxColour(0,0,0));
	m_plstChannels->SetButtonColour(wxColour(wxT("#000000")));
	m_plstChannels->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstChannels->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plstSort = new wmList(m_ppnlChannels, ID_M_PLST2, wxPoint(0,755), wxSize(280,45), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 3, wxSize(3,1));
	m_plstSort->SetBackgroundColour(wxColour(0,0,0));
	m_plstSort->SetButtonColour(wxColour(wxT("#400080")));
	m_plstSort->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_pVlcPanel = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(1000,600), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_pVlcPanel->SetBackgroundColour(wxColour(0,0,0));
	m_ppnlFilter = new wxPanel(this, ID_PANEL2, wxPoint(0,600), wxSize(1000,200), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	m_ppnlFilter->SetBackgroundColour(wxColour(0,0,0));
	m_plstLetters = new wmList(m_ppnlFilter, ID_M_PLST3, wxPoint(0,35), wxSize(900,165), 0, 0, wxSize(-1,50), 13, wxSize(5,5));
	m_plstLetters->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plstLettersFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plstLetters->SetFont(m_plstLettersFont);
	m_plstLetters->SetButtonColour(wxColour(wxT("#000000")));
	m_plstLetters->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstLetters->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pbtnBack = new wmButton(m_ppnlFilter, ID_M_PBTN1, wxEmptyString, wxPoint(910,92), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnBack->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pbtnBackFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnBack->SetFont(m_pbtnBackFont);
	m_pbtnBack->SetColourSelected(wxColour(wxT("#804000")));
	m_pbtnClear = new wmButton(m_ppnlFilter, ID_M_PBTN2, _("Clear"), wxPoint(910,147), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnClear->SetBackgroundColour(wxColour(64,0,0));
	wxFont m_pbtnClearFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnClear->SetFont(m_pbtnClearFont);
	m_pbtnClear->SetColourSelected(wxColour(wxT("#804000")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstChannelsSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstSortSelected);
	m_pVlcPanel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlPlayer::OnLeftUp,0,this);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstLettersSelected);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlPlayer::OnbtnBackClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlPlayer::OnbtnClearClick);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlPlayer::OnLeftUp);
	//*)
	m_plblFilter->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

	m_plstChannels->SetBorderStyle(uiRect::BORDER_NONE);
	m_plstChannels->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plstSort->AddButton(wxT("Number"));
	m_plstSort->AddButton(wxT("Name"));
	m_plstSort->AddButton(wxT("Type"));

	SetSize(size);
    SetPosition(pos);
    m_pbtnBack->SetBitmapLabel(wxBitmap(backspace_xpm));

    m_pVlcPlayer = 0;
    /*
    const char* const vlc_args[] = {"--no-xlib", "--vout","mmal-xsplitter","--mmal-opaque","--mouse-hide-timeout","1"};
    */

    m_pVlcInst = libvlc_new(0, NULL);
    if(m_pVlcInst)
    {
        m_pVlcPlayer = libvlc_media_player_new(m_pVlcInst);
        if(m_pVlcPlayer)
        {
            m_pVlcEvtMan = libvlc_media_player_event_manager(m_pVlcPlayer);
            InitVlc();
        }
        else
        {
            wxLogDebug(wxT("Player Error: %s"), wxString::FromAscii(libvlc_errmsg()).c_str());
        }
    }
    else
    {
       wxLogDebug(wxT("VLC Error: %s"), wxString::FromAscii(libvlc_errmsg()).c_str());
    }

    wxString sSort(Settings::Get().Read(wxT("Channels"), wxT("Sort"), wxT("Name")));
    wxLogDebug(wxT("Sort = %s"), sSort.c_str());
    m_plstSort->SelectButton(sSort, true);

    unsigned long nProgram = Settings::Get().Read(wxT("Channels"), wxT("Current"), 0);
    if(nProgram != 0)
    {
        PlayProgram(nProgram);
    }


}

pnlPlayer::~pnlPlayer()
{
	//(*Destroy(pnlPlayer)
	//*)
	libvlc_media_player_release(m_pVlcPlayer);
    libvlc_release(m_pVlcInst);
}

void pnlPlayer::InitVlc()
{
    if(m_pVlcPlayer)
    {
        libvlc_video_set_mouse_input(m_pVlcPlayer, false);
        libvlc_video_set_key_input(m_pVlcPlayer, false);

#ifdef __WXGTK__

    GtkWidget* pWidget = m_pVlcPanel->GetHandle();
    gtk_widget_realize(pWidget);
    unsigned int nXid = GDK_WINDOW_XWINDOW(pWidget->window);
    wxLogDebug(wxT("XID = %d"), nXid);
    libvlc_media_player_set_xwindow(m_pVlcPlayer, nXid);

#else
        libvlc_media_player_set_hwnd(m_pVlcPlayer, m_pVlcPanel->GetHandle());

#endif

    }
}


void pnlPlayer::PlayLocation(const channel& aChannel)
{

    if(m_pVlcPlayer)
    {
        libvlc_media_player_stop(m_pVlcPlayer);

        libvlc_media_t* pMedia;
        pMedia = libvlc_media_new_location(m_pVlcInst, aChannel.sLocation.mb_str());
        for(list<wxString>::const_iterator itOption = aChannel.lstOptions.begin(); itOption != aChannel.lstOptions.end(); ++itOption)
        {
            libvlc_media_add_option(pMedia, (*itOption).mb_str());
        }

        libvlc_media_player_set_media(m_pVlcPlayer, pMedia);
        libvlc_media_player_play(m_pVlcPlayer);
        libvlc_media_release(pMedia);
    }
}


void pnlPlayer::PlayFile(const wxString& sFile)
{
    if(m_pVlcPlayer)
    {
        libvlc_media_t* pMedia;
        pMedia = libvlc_media_new_path(m_pVlcInst, sFile.mb_str());
        libvlc_media_player_set_media(m_pVlcPlayer, pMedia);

        libvlc_media_player_play(m_pVlcPlayer);

        libvlc_media_release(pMedia);
    }
}

void pnlPlayer::OnLeftUp(wxMouseEvent& event)
{
    if(m_pVlcPanel->GetSize().x == 1280)
    {
        m_pVlcPanel->SetSize(1000,600);
        m_ppnlChannels->Show();
        m_ppnlFilter->Show();
    }
    else
    {
        m_pVlcPanel->SetSize(1280,800);
        m_ppnlChannels->Show(false);
        m_ppnlFilter->Hide();
    }
}

void pnlPlayer::OnlstChannelsSelected(wxCommandEvent& event)
{
    PlayProgram(reinterpret_cast<unsigned long>(event.GetClientData()));
}

void pnlPlayer::PlayProgram(unsigned long nProgram)
{
    map<unsigned long, channel>::const_iterator itChannel = ChannelManager::Get().FindChannel(nProgram);
    if(itChannel != ChannelManager::Get().GetChannelNumberEnd())
    {
        Settings::Get().Write(wxT("Channels"), wxT("Current"), (int)itChannel->first);

        PlayLocation(itChannel->second);

        m_pVlcPanel->SetSize(1280,800);
        m_ppnlChannels->Show(false);
        m_ppnlFilter->Hide();
    }
}

void pnlPlayer::OnlstSortSelected(wxCommandEvent& event)
{
    wxLogDebug(wxT("OnLstSort: %s"), event.GetString().c_str());
    Settings::Get().Write(wxT("Channels"), wxT("Sort"), event.GetString());

    m_plblFilter->SetLabel(wxEmptyString);
    if(event.GetString() == wxT("Number"))
    {
        m_nSort = NUMBER;
    }
    else
    {
        m_nSort = NAME;
    }
    PopulateChannels();

}


size_t pnlPlayer::PopulateChannelNumber()
{

    for(map<unsigned long, channel>::const_iterator itChannel = ChannelManager::Get().GetChannelNumberBegin(); itChannel != ChannelManager::Get().GetChannelNumberEnd(); ++itChannel)
    {
        wxString sChannel(wxString::Format(wxT("%03d"), itChannel->first));
        if(m_plblFilter->GetLabel().empty() || sChannel.Left(min(m_plblFilter->GetLabel().length(), sChannel.length())) == m_plblFilter->GetLabel().Left(min(m_plblFilter->GetLabel().length(), sChannel.length())))
        {
            m_plstChannels->AddButton(wxString::Format(wxT("%03d  %s"), itChannel->first, itChannel->second.sName.c_str()), wxNullBitmap, reinterpret_cast<void*>(itChannel->first));
        }
    }

    m_plstLetters->Freeze();
    m_plstLetters->Clear();

    for(char c = 49; c < 58; c++)
    {
        m_plstLetters->AddButton(wxString::Format(wxT("%C"), c));
    }
    m_plstLetters->AddButton(wxT("0"));
    m_plstLetters->Thaw();

    return m_plstChannels->GetItemCount();
}

size_t pnlPlayer::PopulateChannelName()
{
    for(map<wxString, channel>::const_iterator itChannel = ChannelManager::Get().GetChannelNameBegin(); itChannel != ChannelManager::Get().GetChannelNameEnd(); ++itChannel)
    {
        wxString sChannel(itChannel->first);
        sChannel.Replace(wxT(" "), wxEmptyString);
        sChannel.MakeUpper();

        if(m_plblFilter->GetLabel().empty() || sChannel.Left(min(m_plblFilter->GetLabel().length(), sChannel.length())) == m_plblFilter->GetLabel().Left(min(m_plblFilter->GetLabel().length(), sChannel.length())))
        {
            m_plstChannels->AddButton(wxString::Format(wxT("%s"), itChannel->first.c_str()), wxNullBitmap, reinterpret_cast<void*>(itChannel->second.nNumber));
        }

    }
    m_plstLetters->Freeze();
    m_plstLetters->Clear();

    for(char c = 49; c < 58; c++)
    {
        m_plstLetters->AddButton(wxString::Format(wxT("%C"), c));
    }
    m_plstLetters->AddButton(wxT("0"));
    for(int i = 0; i < 3; i++)
    {
        m_plstLetters->AddButton(wxEmptyString, wxNullBitmap, 0, wmList::wmHIDDEN);
    }
    for(char c = 65; c < 91; c++)
    {
        m_plstLetters->AddButton(wxString::Format(wxT("%C"), c));
    }
    m_plstLetters->Thaw();

    return m_plstChannels->GetItemCount();
}


void pnlPlayer::ShowChannelList(bool bName)
{
    if(bName)
    {
        m_plstSort->SelectButton(wxT("Name"), true);
    }
    else
    {
        m_plstSort->SelectButton(wxT("Number"), true);
    }
}

void pnlPlayer::OnlstLettersSelected(wxCommandEvent& event)
{
    if(m_plstChannels->GetItemCount() > 1)
    {
        wxString sLabel(m_plblFilter->GetLabel());
        sLabel << event.GetString();
        m_plblFilter->SetLabel(sLabel);
        PopulateChannels();
    }
}

void pnlPlayer::OnbtnBackClick(wxCommandEvent& event)
{
    wxString sLabel(m_plblFilter->GetLabel());
    if(sLabel.empty() == false)
    {
        m_plblFilter->SetLabel(sLabel.Left(sLabel.length()-1));
        PopulateChannels();
    }
}


size_t pnlPlayer::PopulateChannels()
{
    m_plstChannels->Freeze();
    m_plstChannels->Clear();
    switch(m_nSort)
    {
    case NAME:
        PopulateChannelName();
        break;
    case NUMBER:
        PopulateChannelNumber();
        break;
    }
    m_plstChannels->Thaw();

    return m_plstChannels->GetItemCount();
}

void pnlPlayer::OnbtnClearClick(wxCommandEvent& event)
{
    m_plblFilter->SetLabel(wxEmptyString);
    PopulateChannels();
}
