#include "pnlPlayer.h"

#include <vlc/vlc.h>
#include <climits>
#include <wx/log.h>
#include "channelmanager.h"
#include "settingevent.h"
#include "settings.h"
#include <algorithm>
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
const long pnlPlayer::ID_M_PLST1 = wxNewId();
const long pnlPlayer::ID_M_PLST2 = wxNewId();
const long pnlPlayer::ID_PANEL4 = wxNewId();
const long pnlPlayer::ID_PANEL1 = wxNewId();
const long pnlPlayer::ID_M_PLST4 = wxNewId();
const long pnlPlayer::ID_M_PLBL1 = wxNewId();
const long pnlPlayer::ID_M_PLST3 = wxNewId();
const long pnlPlayer::ID_M_PBTN1 = wxNewId();
const long pnlPlayer::ID_M_PBTN2 = wxNewId();
const long pnlPlayer::ID_PANEL3 = wxNewId();
const long pnlPlayer::ID_M_PLST6 = wxNewId();
const long pnlPlayer::ID_M_PLST5 = wxNewId();
const long pnlPlayer::ID_M_PBTN3 = wxNewId();
const long pnlPlayer::ID_M_PBTN4 = wxNewId();
const long pnlPlayer::ID_PANEL5 = wxNewId();
const long pnlPlayer::ID_M_PSWP1 = wxNewId();
const long pnlPlayer::ID_PANEL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlPlayer,pmPanel)
	//(*EventTable(pnlPlayer)
	//*)
END_EVENT_TABLE()

pnlPlayer::pnlPlayer(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size,long nStyle, wxString sEmpty)
{
	//(*Initialize(pnlPlayer)
	Create(parent, wxID_ANY, wxPoint(0,0), wxSize(1280,800), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_ppnlChannels = new wxPanel(this, ID_PANEL4, wxPoint(1000,0), wxSize(280,800), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	m_ppnlChannels->SetBackgroundColour(wxColour(0,0,0));
	m_plstChannels = new wmList(m_ppnlChannels, ID_M_PLST1, wxPoint(0,10), wxSize(280,740), wmList::STYLE_SELECT, 1, wxSize(-1,30), 1, wxSize(-1,-1));
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
	m_pswpFilter = new wmSwitcherPanel(m_ppnlFilter, ID_M_PSWP1, wxPoint(0,0), wxSize(1000,200), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpFilter->SetPageNameStyle(0);
	pnlNumbers = new wxPanel(m_pswpFilter, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	pnlNumbers->SetBackgroundColour(wxColour(0,0,0));
	m_plstNumbers = new wmList(pnlNumbers, ID_M_PLST4, wxPoint(0,33), wxSize(692,54), 0, 0, wxSize(-1,50), 10, wxSize(5,5));
	m_plstNumbers->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plstNumbersFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plstNumbers->SetFont(m_plstNumbersFont);
	m_plstNumbers->SetButtonColour(wxColour(wxT("#000000")));
	m_plstNumbers->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstNumbers->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plblFilter = new wmLabel(pnlNumbers, ID_M_PLBL1, _("_"), wxPoint(697,40), wxSize(293,40), 0, _T("ID_M_PLBL1"));
	m_plblFilter->SetBorderState(uiRect::BORDER_DOWN);
	m_plblFilter->GetUiRect().SetGradient(0);
	m_plblFilter->SetBackgroundColour(wxColour(255,255,255));
	m_plstLetters = new wmList(pnlNumbers, ID_M_PLST3, wxPoint(0,85), wxSize(900,115), 0, 0, wxSize(-1,50), 13, wxSize(5,5));
	m_plstLetters->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plstLettersFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plstLetters->SetFont(m_plstLettersFont);
	m_plstLetters->SetButtonColour(wxColour(wxT("#000000")));
	m_plstLetters->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstLetters->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pbtnBack = new wmButton(pnlNumbers, ID_M_PBTN1, wxEmptyString, wxPoint(910,92), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnBack->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pbtnBackFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnBack->SetFont(m_pbtnBackFont);
	m_pbtnBack->SetColourSelected(wxColour(wxT("#804000")));
	m_pbtnClear = new wmButton(pnlNumbers, ID_M_PBTN2, _("Clear"), wxPoint(910,147), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnClear->SetBackgroundColour(wxColour(64,0,0));
	wxFont m_pbtnClearFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnClear->SetFont(m_pbtnClearFont);
	m_pbtnClear->SetColourSelected(wxColour(wxT("#804000")));
	pnlTags = new wxPanel(m_pswpFilter, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	pnlTags->SetBackgroundColour(wxColour(0,0,0));
	m_plstFilter = new wmList(pnlTags, ID_M_PLST6, wxPoint(0,0), wxSize(900,34), 0, 0, wxSize(-1,30), 8, wxSize(5,5));
	m_plstFilter->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plstFilterFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plstFilter->SetFont(m_plstFilterFont);
	m_plstFilter->SetButtonColour(wxColour(wxT("#FFFFFF")));
	m_plstFilter->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstFilter->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plstFilter->SetDisabledColour(wxColour(wxT("#FFFFFF")));
	m_plstFilter->SetTextButtonColour(wxColour(wxT("#000000")));
	m_plstFilter->SetTextDisabledButtonColour(wxColour(wxT("#000000")));
	m_plstTags = new wmList(pnlTags, ID_M_PLST5, wxPoint(0,40), wxSize(900,160), 0, 0, wxSize(-1,30), 8, wxSize(5,5));
	m_plstTags->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plstTagsFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plstTags->SetFont(m_plstTagsFont);
	m_plstTags->SetButtonColour(wxColour(wxT("#000000")));
	m_plstTags->SetPressedButtonColour(wxColour(wxT("#FF8040")));
	m_plstTags->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pbtnTagsBack = new wmButton(pnlTags, ID_M_PBTN3, wxEmptyString, wxPoint(910,92), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnTagsBack->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pbtnTagsBackFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnTagsBack->SetFont(m_pbtnTagsBackFont);
	m_pbtnTagsBack->SetColourSelected(wxColour(wxT("#804000")));
	m_ptbnTagsClear = new wmButton(pnlTags, ID_M_PBTN4, _("Clear"), wxPoint(910,147), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_ptbnTagsClear->SetBackgroundColour(wxColour(64,0,0));
	wxFont m_ptbnTagsClearFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_ptbnTagsClear->SetFont(m_ptbnTagsClearFont);
	m_ptbnTagsClear->SetColourSelected(wxColour(wxT("#804000")));
	m_pswpFilter->AddPage(pnlNumbers, _("Numbers"), false);
	m_pswpFilter->AddPage(pnlTags, _("Tags"), false);

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstChannelsSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstSortSelected);
	m_pVlcPanel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlPlayer::OnLeftUp,0,this);
	Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstLettersSelected);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstLettersSelected);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlPlayer::OnbtnBackClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlPlayer::OnbtnClearClick);
	Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlPlayer::OnlstTagsSelected);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlPlayer::OnbtnTagsBackClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlPlayer::OntbnTagsClearClick);
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
    m_pbtnTagsBack->SetBitmapLabel(wxBitmap(backspace_xpm));

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

    for(char c = 49; c < 58; c++)
    {
        m_plstNumbers->AddButton(wxString::Format(wxT("%C"), c));
    }
    m_plstNumbers->AddButton(wxT("0"));


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

    m_sFilter = wxEmptyString;
    m_plblFilter->SetLabel(wxT("_"));
    if(event.GetString() == wxT("Number"))
    {
        m_nSort = NUMBER;
        m_pswpFilter->ChangeSelection(0);
    }
    else if(event.GetString() == wxT("Name"))
    {
        m_nSort = NAME;
        m_pswpFilter->ChangeSelection(0);
    }
    else
    {
        m_nSort = TAGS;
        m_pswpFilter->ChangeSelection(1);
    }
    PopulateChannels();

}


size_t pnlPlayer::PopulateChannelNumber()
{

    for(map<unsigned long, channel>::const_iterator itChannel = ChannelManager::Get().GetChannelNumberBegin(); itChannel != ChannelManager::Get().GetChannelNumberEnd(); ++itChannel)
    {
        wxString sChannel(wxString::Format(wxT("%03d"), itChannel->first));
        if(m_sFilter.empty() || sChannel.Left(min(m_sFilter.length(), sChannel.length())) == m_sFilter.Left(min(m_sFilter.length(), sChannel.length())))
        {
            m_plstChannels->AddButton(wxString::Format(wxT("%03d  %s"), itChannel->first, itChannel->second.sName.c_str()), wxNullBitmap, reinterpret_cast<void*>(itChannel->first));
        }
    }

    m_plstLetters->Freeze();
    m_plstLetters->Clear();

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

        if(m_sFilter.empty() || sChannel.Left(min(m_sFilter.length(), sChannel.length())) == m_sFilter.Left(min(m_sFilter.length(), sChannel.length())))
        {
            m_plstChannels->AddButton(wxString::Format(wxT("%s"), itChannel->first.c_str()), wxNullBitmap, reinterpret_cast<void*>(itChannel->second.nNumber));
        }

    }
    m_plstLetters->Freeze();
    m_plstLetters->Clear();

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
        m_sFilter << event.GetString();
        m_plblFilter->SetLabel(wxString::Format(wxT("%s_"), m_sFilter.c_str()));
        PopulateChannels();
    }
}

void pnlPlayer::OnbtnBackClick(wxCommandEvent& event)
{
    if(m_sFilter.empty() == false)
    {
        m_sFilter = m_sFilter.Left(m_sFilter.length()-1);
        m_plblFilter->SetLabel(wxString::Format(wxT("%s_"), m_sFilter.c_str()));
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
    case TAGS:
        PopulateChannelTags();
        break;
    }
    m_plstChannels->Thaw();

    return m_plstChannels->GetItemCount();
}

void pnlPlayer::OnbtnClearClick(wxCommandEvent& event)
{
    m_sFilter = wxEmptyString;
    m_plblFilter->SetLabel(wxT("_"));
    PopulateChannels();
}

void pnlPlayer::OnlstTagsSelected(wxCommandEvent& event)
{
    m_lstTags.push_back(event.GetString());
    m_plstFilter->AddButton(event.GetString(), wxNullBitmap, 0, wmList::wmREADONLY);
    m_plstFilter->Refresh();
    PopulateChannelTags();
}

void pnlPlayer::OnbtnTagsBackClick(wxCommandEvent& event)
{
    if(m_lstTags.empty() == false)
    {
        m_plstFilter->DeleteButton(m_plstFilter->FindButton(m_lstTags.back()));
        m_lstTags.pop_back();

    }
    PopulateChannelTags();
}

void pnlPlayer::OntbnTagsClearClick(wxCommandEvent& event)
{
    m_plstFilter->Clear();
    m_lstTags.clear();
    PopulateChannelTags();
}


size_t pnlPlayer::PopulateChannelTags()
{
    m_plstTags->Freeze();
    m_plstTags->Clear();
    m_plstChannels->Freeze();
    m_plstChannels->Clear();

    if(m_lstTags.empty())
    {
        for(set<wxString>::iterator itTag = ChannelManager::Get().GetTagBegin(); itTag != ChannelManager::Get().GetTagEnd(); ++itTag)
        {
            m_plstTags->AddButton(*itTag);
        }
    }
    else
    {

        std::map<wxString, unsigned int> mTags;
        for(map<unsigned long, channel>::const_iterator itChannel = ChannelManager::Get().GetChannelNumberBegin(); itChannel != ChannelManager::Get().GetChannelNumberEnd(); ++itChannel)
        {
            bool bAdd(true);
            for(list<wxString>::const_iterator itTag = m_lstTags.begin(); itTag != m_lstTags.end(); ++itTag)
            {
                if(itChannel->second.setTags.find((*itTag)) == itChannel->second.setTags.end())
                {
                    bAdd = false;
                    break;
                }
            }
            if(bAdd)
            {
                m_plstChannels->AddButton(wxString::Format(wxT("%03d  %s"), itChannel->first, itChannel->second.sName.c_str()), wxNullBitmap, reinterpret_cast<void*>(itChannel->first));

                //Store the tags
                for(set<wxString>::iterator itAdd = itChannel->second.setTags.begin(); itAdd != itChannel->second.setTags.end(); ++itAdd)
                {
                    if(find(m_lstTags.begin(), m_lstTags.end(), (*itAdd)) == m_lstTags.end())
                    {
                        map<wxString, unsigned int>::iterator it = mTags.insert(make_pair((*itAdd),0)).first;
                        it->second++;
                    }
                }
            }
        }

        for(map<wxString, unsigned int>::iterator itTag = mTags.begin(); itTag != mTags.end(); ++itTag)
        {
            if(itTag->second < m_plstChannels->GetItemCount())
            {   //only add the tag if it will do further filtering
                m_plstTags->AddButton(itTag->first);
            }
        }
    }
    m_plstTags->Thaw();

    m_plstChannels->Thaw();
    return m_plstChannels->GetItemCount();
}
