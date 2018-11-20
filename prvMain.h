/***************************************************************
 * Name:      prvMain.h
 * Purpose:   Defines Application Frame
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-11-20
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef PRVMAIN_H
#define PRVMAIN_H

//(*Headers(prvDialog)
#include <wx/notebook.h>
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include <wx/dialog.h>
//*)
#include "pnlPlayer.h"


class prvDialog: public wxDialog
{
    public:

        prvDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~prvDialog();

    private:

        //(*Handlers(prvDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnlstChannelsSelected(wxCommandEvent& event);
        void OnlstSortSelected(wxCommandEvent& event);
        //*)

        //(*Identifiers(prvDialog)
        static const long ID_PANEL1;
        static const long ID_PANEL5;
        static const long ID_PANEL3;
        static const long ID_M_PSWP1;
        //*)

        //(*Declarations(prvDialog)
        wxPanel* m_ppnlSearch;
        pnlPlayer* m_ppnlVLC;
        wxPanel* m_pppnlSplash;
        wmSwitcherPanel* m_pswpMain;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PRVMAIN_H
