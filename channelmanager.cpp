#include "channelmanager.h"
#include <wx/log.h>
#include "inimanager.h"
#include "inisection.h"

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
        for(wxXmlNode* pChannelNode = xmlDoc.GetRoot()->GetChildren(); pChannelNode; pChannelNode = pChannelNode->GetNext())
        {
            if(pChannelNode->GetName().CmpNoCase(wxT("channel")) == 0)
            {
                LoadDVBChannel(pChannelNode);
            }
        }
        return true;
    }
    wxLogDebug(wxT("Could't open file %s"), fnDoc.GetFullPath().c_str());
    return false;
}

map<unsigned long, channel>::const_iterator ChannelManager::GetChannelNumberBegin()
{
    return m_mChannelNumber.begin();
}


map<unsigned long, channel>::const_iterator ChannelManager::GetChannelNumberEnd()
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
        if(pNode->GetName().CmpNoCase(wxT("name")) == 0)
        {
            aChannel.sName = pNode->GetNodeContent();
        }
        else if(pNode->GetName().CmpNoCase(wxT("location")) == 0)
        {
            aChannel.sLocation = pNode->GetNodeContent();
        }
        else if(pNode->GetName().CmpNoCase(wxT("options")) == 0)
        {
            for(wxXmlNode* pVlcNode = pNode->GetChildren(); pVlcNode; pVlcNode = pVlcNode->GetNext())
            {
                aChannel.lstOptions.push_back(wxString::Format(wxT("%s=%s"), pVlcNode->GetName().c_str(), pVlcNode->GetNodeContent().c_str()));
                if(pVlcNode->GetName().CmpNoCase(wxT("program")) == 0)
                {
                    aChannel.sPID = pVlcNode->GetNodeContent();
                }
            }
        }
        else if(pNode->GetName().CmpNoCase(wxT("number")) == 0)
        {
            pNode->GetNodeContent().ToULong(&aChannel.nNumber);
        }
    }
    if(aChannel.sLocation.empty() == false)
    {
        if(aChannel.nNumber == 0)
        {
            aChannel.nNumber = m_nUnknown;
            ++m_nUnknown;
        }
        if(m_mChannelNumber.insert(make_pair(aChannel.nNumber, aChannel)).second == false)
        {
            wxLogDebug(wxT("Duplicate channel number %d = %s"), aChannel.nNumber, aChannel.sName.c_str());
        }
        if(m_mChannelName.insert(make_pair(aChannel.sName, aChannel)).second == false)
        {
            wxLogDebug(wxT("Duplicate channel name %d = %s"), aChannel.nNumber, aChannel.sName.c_str());
        }
    }
}


map<unsigned long, channel>::const_iterator ChannelManager::FindChannel(unsigned long nChannelNumber)
{
    return m_mChannelNumber.find(nChannelNumber);
}


bool ChannelManager::SaveDVBList(const wxFileName& fnDoc)
{
    wxXmlNode* pRoot = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("channels"));

    for(map<unsigned long, channel>::iterator itChannel = m_mChannelNumber.begin(); itChannel != m_mChannelNumber.end(); ++itChannel)
    {
        wxXmlNode* pChannelNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("channel"));
        wxXmlNode* pNumberNode = CreateTextNode(wxT("number"), wxString::Format(wxT("%03d"), itChannel->first));
        wxXmlNode* pNameNode = CreateTextNode(wxT("name"), itChannel->second.sName);
        wxXmlNode* pLocationNode = CreateTextNode(wxT("location"), itChannel->second.sLocation);

        wxXmlNode* pOptionNodes = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("options"));
        for(list<wxString>::iterator itOption = itChannel->second.lstOptions.begin(); itOption != itChannel->second.lstOptions.end(); ++itOption)
        {
            pOptionNodes->AddChild(CreateTextNode((*itOption).BeforeFirst(wxT('=')), (*itOption).AfterFirst(wxT('='))));
        }
        pChannelNode->AddChild(pNameNode);
        pChannelNode->AddChild(pNumberNode);
        pChannelNode->AddChild(pLocationNode);
        pChannelNode->AddChild(pOptionNodes);

        pRoot->AddChild(pChannelNode);
    }

    wxXmlDocument xmlDoc;
    xmlDoc.SetRoot(pRoot);
    xmlDoc.Save(fnDoc.GetFullPath());
}

wxXmlNode* ChannelManager::CreateTextNode(const wxString& sNode, const wxString& sContent)
{
    wxXmlNode* pNameNode = new wxXmlNode(wxXML_ELEMENT_NODE, sNode);
    pNameNode->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, sContent));
    return pNameNode;
}
