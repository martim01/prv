#ifndef PNLPLAYER_H
#define PNLPLAYER_H
#include <vlc/vlc.h>

//(*Headers(pnlPlayer)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)
#include "pmpanel.h"
#include <list>
struct channel;

class pnlPlayer: public pmPanel
{
	public:

		pnlPlayer(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize,long nStyle=0, wxString sEmpty=wxEmptyString);
		virtual ~pnlPlayer();

		//(*Declarations(pnlPlayer)
		wmButton* m_pbtnTagsBack;
		wxPanel* pnlNumbers;
		wmList* m_plstLetters;
		wmButton* m_ptbnTagsClear;
		wmLabel* m_plblFilter;
		wmList* m_plstTags;
		wmButton* m_pbtnBack;
		wmButton* m_pbtnClear;
		wmList* m_plstSort;
		wmList* m_plstNumbers;
		wxPanel* m_ppnlFilter;
		wmList* m_plstChannels;
		wxPanel* m_pVlcPanel;
		wxPanel* m_ppnlChannels;
		wmSwitcherPanel* m_pswpFilter;
		wmList* m_plstFilter;
		wxPanel* pnlTags;
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
		static const long ID_M_PLST4;
		static const long ID_M_PLBL1;
		static const long ID_M_PLST3;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_PANEL3;
		static const long ID_M_PLST6;
		static const long ID_M_PLST5;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_PANEL5;
		static const long ID_M_PSWP1;
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
		void OnlstTagsSelected(wxCommandEvent& event);
		void OnbtnTagsBackClick(wxCommandEvent& event);
		void OntbnTagsClearClick(wxCommandEvent& event);
		//*)

		void InitVlc();

        size_t PopulateChannels();
		size_t PopulateChannelNumber();
		size_t PopulateChannelName();
		size_t PopulateChannelTags();

		void PlayProgram(unsigned long nProgram);



		DECLARE_EVENT_TABLE()

		libvlc_media_player_t* m_pVlcPlayer;
        libvlc_instance_t* m_pVlcInst;
        libvlc_event_manager_t* m_pVlcEvtMan;

        int m_nSort;
        wxString m_sFilter;
        std::list<wxString> m_lstTags;
        enum {NAME, NUMBER, TAGS};
};

#endif
