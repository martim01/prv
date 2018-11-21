#include "pnlPlayer.h"

#include <vlc/vlc.h>
#include <climits>
#include <wx/log.h>
#include "channelmanager.h"
#ifdef __WXGTK__
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
//#include <wx/gtk/win_gtk.h>
//#define GET_XID(window) GDK_WINDOW_XWINDOW(GTK_PIZZA(window->m_wxwindow)->bin_window)
#endif

using namespace std;

//(*InternalHeaders(pnlPlayer)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlPlayer)
const long pnlPlayer::ID_M_PLST1 = wxNewId();
const long pnlPlayer::ID_M_PLST2 = wxNewId();
const long pnlPlayer::ID_PANEL4 = wxNewId();
const long pnlPlayer::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlPlayer,wxPanel)
	//(*EventTable(pnlPlayer)
	//*)
END_EVENT_TABLE()

pnlPlayer::pnlPlayer(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size,long nStyle, wxString sEmpty)
{
	//(*Initialize(pnlPlayer)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_ppnlChannels = new wxPanel(this, ID_PANEL4, wxPoint(600,0), wxSize(200,480), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	m_ppnlChannels->SetBackgroundColour(wxColour(0,0,0));
	m_plstChannels = new wmList(m_ppnlChannels, ID_M_PLST1, wxPoint(0,0), wxSize(200,430), wmList::STYLE_SELECT, 1, wxSize(-1,30), 1, wxSize(-1,-1));
	m_plstChannels->SetBackgroundColour(wxColour(0,0,0));
	m_plstChannels->SetButtonColour(wxColour(wxT("#000000")));
	m_plstChannels->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstChannels->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plstSort = new wmList(m_ppnlChannels, ID_M_PLST2, wxPoint(0,435), wxSize(200,45), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(3,1));
	m_plstSort->SetBackgroundColour(wxColour(0,0,0));
	m_plstSort->SetButtonColour(wxColour(wxT("#400080")));
	m_plstSort->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_pVlcPanel = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(600,480), wxTAB_TRAVERSAL, _T("ID_PANEL1"));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstChannelsSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstSortSelected);
	m_pVlcPanel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlPlayer::OnLeftUp,0,this);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlPlayer::OnLeftUp);
	//*)

	m_plstChannels->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plstSort->AddButton(wxT("Number"));
	m_plstSort->AddButton(wxT("Name"));

	SetSize(size);
    SetPosition(pos);

	m_ppnlChannels->Hide();
	m_pVlcPanel->SetSize(800,480);

    m_pVlcPlayer = 0;
	//setup vlc
	//"--no-xlib"
    m_pVlcInst = libvlc_new(0, NULL);
    if(m_pVlcInst)
    {
        m_pVlcPlayer = libvlc_media_player_new(m_pVlcInst);
        if(m_pVlcPlayer)
        {
            m_pVlcEvtMan = libvlc_media_player_event_manager(m_pVlcPlayer);
//            if(m_pVlcEvtMan)
//            {
//                libvlc_event_attach(m_pVlcEvtMan, libvlc_MediaPlayerEndReached, ::OnEndReached_VLC, NULL);
//                libvlc_event_attach(m_pVlcEvtMan, libvlc_MediaPlayerPositionChanged, ::OnPositionChanged_VLC, NULL);
//                Connect(wxID_ANY, vlcEVT_END, wxCommandEventHandler(MainWindow::OnEndReached_VLC));
//                Connect(wxID_ANY, vlcEVT_POS, wxCommandEventHandler(MainWindow::OnPositionChanged_VLC));
//            }
//            else
//            {
//                wxLogDebug(wxT("Event Error: %s"), wxString::FromAscii(libvlc_errmsg()).c_str());
//            }
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
    if(m_pVlcPanel->GetSize().x == 800)
    {
        m_pVlcPanel->SetSize(600,480);
        m_ppnlChannels->Show();
    }
    else
    {
        m_pVlcPanel->SetSize(800,480);
        m_ppnlChannels->Show(false);
    }
}

void pnlPlayer::OnlstChannelsSelected(wxCommandEvent& event)
{
    map<size_t, channel>::const_iterator itChannel = ChannelManager::Get().FindChannel(reinterpret_cast<size_t>(event.GetClientData()));
    if(itChannel != ChannelManager::Get().GetChannelNumberEnd())
    {
        PlayLocation(itChannel->second);
    }
}

void pnlPlayer::OnlstSortSelected(wxCommandEvent& event)
{
    m_plstChannels->Freeze();
    m_plstChannels->Clear();
    if(event.GetString() == wxT("Number"))
    {
        PopulateChannelNumber();
    }
    else
    {
        PopulateChannelName();
    }
    m_plstChannels->Thaw();
}


void pnlPlayer::PopulateChannelNumber()
{
    for(map<size_t, channel>::const_iterator itChannel = ChannelManager::Get().GetChannelNumberBegin(); itChannel != ChannelManager::Get().GetChannelNumberEnd(); ++itChannel)
    {
        m_plstChannels->AddButton(wxString::Format(wxT("%04d %s"), itChannel->first, itChannel->second.sName.c_str()), wxNullBitmap, reinterpret_cast<void*>(itChannel->first));
    }
}

void pnlPlayer::PopulateChannelName()
{
    for(map<wxString, channel>::const_iterator itChannel = ChannelManager::Get().GetChannelNameBegin(); itChannel != ChannelManager::Get().GetChannelNameEnd(); ++itChannel)
    {
        m_plstChannels->AddButton(wxString::Format(wxT("%s"), itChannel->first.c_str()), wxNullBitmap, reinterpret_cast<void*>(itChannel->second.nNumber));
    }
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
