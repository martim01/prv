#include "channelmanager.h"
#include <wx/log.h>

using namespace std;

ChannelManager& ChannelManager::Get()
{
    static ChannelManager ch;
    return ch;
}

ChannelManager::~ChannelManager()
{
    DeleteAllChannels();
}

bool ChannelManager::LoadDVBList(const wxFileName& fnDoc)
{
    wxXmlDocument xmlDoc;
    if(xmlDoc.Load(fnDoc.GetFullPath()) && xmlDoc.GetRoot())
    {
        for(wxXmlNode* pTrackListNode = xmlDoc.GetRoot()->GetChildren(); pTrackListNode; pTrackListNode = pTrackListNode->GetNext())
        {
            if(pTrackListNode->GetName().CmpNoCase(wxT("trackList")) == 0)
            {
                for(wxXmlNode* pTrackNode = pTrackListNode->GetChildren(); pTrackNode; pTrackNode = pTrackNode->GetNext())
                {
                    if(pTrackNode->GetName().CmpNoCase(wxT("track")) == 0)
                    {
                        LoadDVBChannel(pTrackNode);
                    }
                }
                break;
            }
        }
        return true;
    }
    wxLogDebug(wxT("Could't open file %s"), fnDoc.GetFullPath().c_str());
    return false;
}



map<size_t, channel>::const_iterator ChannelManager::GetChannelNumberBegin()
{
    return m_mChannelNumber.begin();
}


map<size_t, channel>::const_iterator ChannelManager::GetChannelNumberEnd()
{
    return m_mChannelNumber.end();
}


map<wxString, channel>::const_iterator ChannelManager::GetChannelNameBegin()
{
    return m_mChannelName.begin();
}


map<wxString, channel>::const_iterator ChannelManager::GetChannelNameEnd()
{
    return m_mChannelName.end();
}



void ChannelManager::DeleteAllChannels()
{
    m_mChannelNumber.clear();
    m_mChannelName.clear();
}


void ChannelManager::LoadDVBChannel(wxXmlNode* pChannelNode)
{
    channel aChannel;
    for(wxXmlNode* pNode = pChannelNode->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        if(pNode->GetName().CmpNoCase(wxT("title")) == 0)
        {
            aChannel.sName = pNode->GetNodeContent().AfterFirst(wxT(' '));
        }
        else if(pNode->GetName().CmpNoCase(wxT("location")) == 0)
        {
            aChannel.sLocation = pNode->GetNodeContent();
        }
        else if(pNode->GetName().CmpNoCase(wxT("extension")) == 0)
        {
            for(wxXmlNode* pVlcNode = pNode->GetChildren(); pVlcNode; pVlcNode = pVlcNode->GetNext())
            {
                if(pVlcNode->GetName().CmpNoCase(wxT("vlc:id")) == 0)
                {
                    pVlcNode->GetNodeContent().ToULong(&aChannel.nNumber);
                }
                else if(pVlcNode->GetName().CmpNoCase(wxT("vlc:option")) && pVlcNode->GetNodeContent().BeforeFirst(wxT('=')).CmpNoCase(wxT("program"))==0)
                {
                    aChannel.sProgram = pVlcNode->GetNodeContent();
                }
            }
        }
    }
    if(aChannel.sLocation.empty() == false)
    {
        m_mChannelNumber.insert(make_pair(aChannel.nNumber, aChannel));
        m_mChannelName.insert(make_pair(aChannel.sName, aChannel));
    }
}


map<size_t, channel>::const_iterator ChannelManager::FindChannel(size_t nChannelNumber)
{
    return m_mChannelNumber.find(nChannelNumber);
}
