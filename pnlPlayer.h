#ifndef PNLPLAYER_H
#define PNLPLAYER_H
#include <vlc/vlc.h>

//(*Headers(pnlPlayer)
#include <wx/panel.h>
#include "wmlist.h"
//*)

struct channel;

class pnlPlayer: public wxPanel
{
	public:

		pnlPlayer(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize,long nStyle=0, wxString sEmpty=wxEmptyString);
		virtual ~pnlPlayer();

		//(*Declarations(pnlPlayer)
		wmList* m_plstSort;
		wmList* m_plstChannels;
		wxPanel* m_pVlcPanel;
		wxPanel* m_ppnlChannels;
		//*)

        void PlayLocation(const channel& aChannel);
        void PlayFile(const wxString& sFile);

        void ShowChannelList(bool bName);

	protected:

		//(*Identifiers(pnlPlayer)
		static const long ID_M_PLST1;
		static const long ID_M_PLST2;
		static const long ID_PANEL4;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(pnlPlayer)
		void OnLeftUp(wxMouseEvent& event);
		void OnlstChannelsSelected(wxCommandEvent& event);
		void OnlstSortSelected(wxCommandEvent& event);
		//*)

		void InitVlc();

		void PopulateChannelNumber();
		void PopulateChannelName();


		DECLARE_EVENT_TABLE()

		libvlc_media_player_t* m_pVlcPlayer;
        libvlc_instance_t* m_pVlcInst;
        libvlc_event_manager_t* m_pVlcEvtMan;
};

#endif
