/***************************************************************
 * Name:      prvApp.h
 * Purpose:   Defines Application Class
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-11-20
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef PRVAPP_H
#define PRVAPP_H

#include <wx/app.h>

class prvApp : public wxApp
{
    public:
        virtual bool OnInit();

        /**  Captures events allowing modification or filtering before passing to normal application event handlers */
        int FilterEvent(wxEvent& event);
};

#endif // PRVAPP_H
