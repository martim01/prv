/***************************************************************
 * Name:      prvApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-11-20
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "prvApp.h"
#include "settings.h"
#include <wx/stdpaths.h>

//(*AppHeaders
#include "prvMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(prvApp);

bool prvApp::OnInit()
{
    Settings::Get().ReadSettings(wxString::Format(wxT("%s/prv/prv.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str()));

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	prvDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}


int prvApp::FilterEvent(wxEvent& event)
{
//    if(event.GetEventType() == wxEVT_LEFT_UP)
//    {
//        wxLogDebug(wxT("LEFT UP"));
//        return -1;
//    }

    return -1;
}
