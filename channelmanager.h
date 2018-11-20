#pragma once
#include <wx/xml/xml.h>
#include <wx/filename.h>
#include <map>

struct channel
{
    unsigned long nNumber;
    wxString sName;
    wxString sLocation;
    wxString sProgram;
};

class ChannelManager
{
    public:
        static ChannelManager& Get();

        bool LoadDVBList(const wxFileName& fnDoc);

        std::map<size_t, channel>::const_iterator GetChannelNumberBegin();
        std::map<size_t, channel>::const_iterator GetChannelNumberEnd();
        std::map<wxString, channel>::const_iterator GetChannelNameBegin();
        std::map<wxString, channel>::const_iterator GetChannelNameEnd();

        std::map<size_t, channel>::const_iterator FindChannel(size_t nChannelNumber);


    private:
        ChannelManager(){}
        ~ChannelManager();

        void DeleteAllChannels();

        void LoadDVBChannel(wxXmlNode* pChannelNode);


        std::map<size_t, channel> m_mChannelNumber;
        std::map<wxString, channel> m_mChannelName;
};
