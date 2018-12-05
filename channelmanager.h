#pragma once
#include <wx/xml/xml.h>
#include <wx/filename.h>
#include <map>
#include <list>
#include <set>

struct channel
{
    unsigned long nNumber;
    wxString sName;
    wxString sLocation;
    wxString sPID;
    std::list<wxString> lstOptions;
    std::set<wxString> setTags;
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
        std::set<wxString>::const_iterator GetTagBegin();
        std::set<wxString>::const_iterator GetTagEnd();

        std::map<unsigned long, channel>::const_iterator FindChannel(unsigned long nChannelNumber);


        bool SaveDVBList(const wxFileName& dfnDoc);

    private:
        ChannelManager() : m_nUnknown(1000){}
        ~ChannelManager();

        void DeleteAllChannels();

        void LoadDVBChannel(wxXmlNode* pChannelNode);

        wxXmlNode* CreateTextNode(const wxString& sNode, const wxString& sContent);

        std::map<unsigned long, channel> m_mChannelNumber;
        std::map<wxString, channel> m_mChannelName;

        std::set<wxString> m_setTags;

        unsigned long m_nUnknown;
};
