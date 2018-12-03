#ifndef PNLPLAYER_H
#define PNLPLAYER_H
#include <vlc/vlc.h>

//(*Headers(pnlPlayer)
#include "wmbutton.h"
#include "wmlabel.h"
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
		wmList* m_plstLetters;
		wmLabel* m_plblFilter;
		wmButton* m_pbtnBack;
		wmButton* m_pbtnClear;
		wmList* m_plstSort;
		wxPanel* m_ppnlFilter;
		wmList* m_plstChannels;
		wxPanel* m_pVlcPanel;
		wxPanel* m_ppnlChannels;
		//*)

        void PlayLocation(const channel& aChannel);
        void PlayFile(const wxString& sFile);

        void ShowChannelList(bool bName);

	protected:

		//(*Identifiers(pnlPlayer)
		static const long ID_M_PLBL1;
		static const long ID_M_PLST1;
		static const long ID_M_PLST2;
		static const long ID_PANEL4;
		static const long ID_PANEL1;
		static const long ID_M_PLST3;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_PANEL2;
		//*)

	private:

		//(*Handlers(pnlPlayer)
		void OnLeftUp(wxMouseEvent& event);
		void OnlstChannelsSelected(wxCommandEvent& event);
		void OnlstSortSelected(wxCommandEvent& event);
		void OnlstLettersSelected(wxCommandEvent& event);
		void OnbtnBackClick(wxCommandEvent& event);
		void OnbtnClearClick(wxCommandEvent& event);
		//*)

		void InitVlc();

        size_t PopulateChannels();
		size_t PopulateChannelNumber();
		size_t PopulateChannelName();


		DECLARE_EVENT_TABLE()

		libvlc_media_player_t* m_pVlcPlayer;
        libvlc_instance_t* m_pVlcInst;
        libvlc_event_manager_t* m_pVlcEvtMan;

        int m_nSort;
        enum {NAME, NUMBER, TAGS};
};

#endif
