#pragma once
#include <wx/xml/xml.h>
#include <wx/filename.h>
#include <map>
#include <list>


struct channel
{
    unsigned long nNumber;
    wxString sName;
    wxString sLocation;
    wxString sPID;
    std::list<wxString> lstOptions;
};

class ChannelManager
{
    public:
        static ChannelManager& Get();

        bool LoadDVBList(const wxFileName& fnDoc);

        std::map<unsigned long, channel>::const_iterator GetChannelNumberBegin();
        std::map<unsigned long, channel>::const_iterator GetChannelNumberEnd();
        std::map<wxString, channel>::const_iterator GetChannelNameBegin();
        std::map<wxString, channel>::const_iterator GetChannelNameEnd();

        std::map<unsigned long, channel>::const_iterator FindChannel(unsigned long nChannelNumber);


    private:
        ChannelManager(){}
        ~ChannelManager();

        void DeleteAllChannels();

        void LoadDVBChannel(wxXmlNode* pChannelNode);


        std::map<unsigned long, channel> m_mChannelNumber;
        std::map<wxString, channel> m_mChannelName;
};
