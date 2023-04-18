#include "clogfile.h"
#include <vector>
#include <exception>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "cfunctracer.h"
#include "cscopedtimer.h"
#include "cconversion.h"

long long CLogEntry::m_genID = 0;


CLogFile::CLogFile(const char* file, std::shared_ptr<CTracer> tracer)
    : m_trace(tracer)
    , m_name(file)
    , m_sLine("")
    , m_sTime("")
    , m_sLevel("")
    , m_sProcId("")
    , m_sThreadId("")
    , m_sClassname("")
    , m_sFuncName("")
    , m_sDescription("")
    , m_bDetectedFormat(false)
    , m_TimeIdx(-1)
    , m_LevelIdx(-1)
    , m_ProdIdIdx(-1)
    , m_ThreadIdIdx(-1)
    , m_ClassNameIdx(-1)
    , m_FuncIdx(-1)
    , m_DescIdx(-1)
    , m_maxDescLength(0)
    , m_maxClassLength(0)
    , m_maxFuncLength(0)

{
    CFuncTracer trace("CLogFile::CLogFile", m_trace);
    try
    {
        trace.Info("File : %s", file);
        parse();
        trace.Info("entries: %ld", m_logEntries.size());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
CLogFile::CLogFile(const CLogFile& file)
    : m_trace(file.m_trace)
    , m_name(file.m_name)
    , m_sLine(file.m_sLine)
    , m_sTime(file.m_sTime)
    , m_sLevel(file.m_sLevel)
    , m_sProcId(file.m_sProcId)
    , m_sThreadId(file.m_sThreadId)
    , m_sClassname(file.m_sClassname)
    , m_sFuncName(file.m_sFuncName)
    , m_sDescription(file.m_sDescription)
    , m_bDetectedFormat(file.m_bDetectedFormat)
    , m_TimeIdx(file.m_TimeIdx)
    , m_LevelIdx(file.m_LevelIdx)
    , m_ProdIdIdx(file.m_ProdIdIdx)
    , m_ThreadIdIdx(file.m_ThreadIdIdx)
    , m_ClassNameIdx(file.m_ClassNameIdx)
    , m_FuncIdx(file.m_FuncIdx)
    , m_DescIdx(file.m_DescIdx)
    , m_maxDescLength(file.m_maxDescLength)
    , m_maxClassLength(file.m_maxClassLength)
    , m_maxFuncLength(file.m_maxFuncLength)
{
    CFuncTracer trace("CLogFile::CLogFile(COPY)", file.m_trace);
    try
    {
        std::copy(file.m_logEntries.begin(), file.m_logEntries.end(), std::back_inserter(m_logEntries));
        std::copy(file.m_filteredEntries.begin(), file.m_filteredEntries.end(), std::back_inserter(m_filteredEntries));
        m_ClassOccurences.insert(file.m_ClassOccurences.begin(), file.m_ClassOccurences.end());
        m_FunctionOccurences.insert(file.m_FunctionOccurences.begin(), file.m_FunctionOccurences.end());
        m_FunctionEntries.insert(file.m_FunctionEntries.begin(), file.m_FunctionEntries.end());
        m_FunctionExits.insert(file.m_FunctionExits.begin(), file.m_FunctionExits.end());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

CLogFile::CLogFile(const CLogFile &&file)
{
    CFuncTracer trace("CLogFile::CLogFile(MOVE)", file.m_trace);
    try
    {
        m_name = std::move(file.m_name);
        m_sLine = std::move(file.m_sLine);
        m_sTime = std::move(file.m_sTime);
        m_sLevel = std::move(file.m_sLevel);
        m_sProcId = std::move(file.m_sProcId);
        m_sThreadId = std::move(file.m_sThreadId);
        m_sClassname = std::move(file.m_sClassname);
        m_sFuncName = std::move(file.m_sFuncName);
        m_sDescription = std::move(file.m_sDescription);
        m_TimeIdx = std::move(file.m_TimeIdx);
        m_LevelIdx = std::move(file.m_LevelIdx);
        m_ProdIdIdx = std::move(file.m_ProdIdIdx);
        m_ThreadIdIdx = std::move(file.m_ThreadIdIdx);
        m_ClassNameIdx = std::move(file.m_ClassNameIdx);
        m_FuncIdx = std::move(file.m_FuncIdx);
        m_DescIdx = std::move(file.m_DescIdx);
        m_maxDescLength = std::move(file.m_maxDescLength);
        m_maxClassLength = std::move(file.m_maxClassLength);
        m_maxFuncLength = std::move(file.m_maxFuncLength);
        m_logEntries = std::move(file.m_logEntries);
        m_filteredEntries = std::move(file.m_filteredEntries);
        m_ClassOccurences.insert(file.m_ClassOccurences.begin(), file.m_ClassOccurences.end());
        m_FunctionOccurences.insert(file.m_FunctionOccurences.begin(), file.m_FunctionOccurences.end());
        m_FunctionEntries.insert(file.m_FunctionEntries.begin(), file.m_FunctionEntries.end());
        m_FunctionExits.insert(file.m_FunctionExits.begin(), file.m_FunctionExits.end());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
CLogFile& CLogFile::operator=(CLogFile& file)
{
    CFuncTracer trace("CLogFile::operator=(&)", file.m_trace);
    try
    {
        if (&file != this)
        {
            m_name = file.m_name;
            m_sLine = file.m_sLine;
            m_sTime = file.m_sTime;
            m_sLevel = file.m_sLevel;
            m_sProcId = file.m_sProcId;
            m_sThreadId = file.m_sThreadId;
            m_sClassname = file.m_sClassname;
            m_sFuncName = file.m_sFuncName;
            m_sDescription = file.m_sDescription;
            m_TimeIdx = file.m_TimeIdx;
            m_LevelIdx = file.m_LevelIdx;
            m_ProdIdIdx = file.m_ProdIdIdx;
            m_ThreadIdIdx = file.m_ThreadIdIdx;
            m_ClassNameIdx = file.m_ClassNameIdx;
            m_FuncIdx = file.m_FuncIdx;
            m_DescIdx = file.m_DescIdx;
            m_maxDescLength = file.m_maxDescLength;
            m_maxClassLength = file.m_maxClassLength;
            m_maxFuncLength = file.m_maxFuncLength;
            m_logEntries = file.m_logEntries;
            m_filteredEntries = file.m_filteredEntries;
            m_ClassOccurences.insert(file.m_ClassOccurences.begin(), file.m_ClassOccurences.end());
            m_FunctionOccurences.insert(file.m_FunctionOccurences.begin(), file.m_FunctionOccurences.end());
            m_FunctionEntries.insert(file.m_FunctionEntries.begin(), file.m_FunctionEntries.end());
            m_FunctionExits.insert(file.m_FunctionExits.begin(), file.m_FunctionExits.end());
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return *this;
}
CLogFile& CLogFile::operator=(CLogFile&& file)
{
    CFuncTracer trace("CLogFile::operator=(&&)", file.m_trace);
    try
    {
        if (&file != this)
        {
            m_name = std::move(file.m_name);
            m_sLine = std::move(file.m_sLine);
            m_sTime = std::move(file.m_sTime);
            m_sLevel = std::move(file.m_sLevel);
            m_sProcId = std::move(file.m_sProcId);
            m_sThreadId = std::move(file.m_sThreadId);
            m_sClassname = std::move(file.m_sClassname);
            m_sFuncName = std::move(file.m_sFuncName);
            m_sDescription = std::move(file.m_sDescription);
            m_TimeIdx = std::move(file.m_TimeIdx);
            m_LevelIdx = std::move(file.m_LevelIdx);
            m_ProdIdIdx = std::move(file.m_ProdIdIdx);
            m_ThreadIdIdx = std::move(file.m_ThreadIdIdx);
            m_ClassNameIdx = std::move(file.m_ClassNameIdx);
            m_FuncIdx = std::move(file.m_FuncIdx);
            m_DescIdx = std::move(file.m_DescIdx);
            m_maxDescLength = std::move(file.m_maxDescLength);
            m_maxClassLength = std::move(file.m_maxClassLength);
            m_maxFuncLength = std::move(file.m_maxFuncLength);
            m_logEntries = std::move(file.m_logEntries);
            m_filteredEntries = std::move(file.m_filteredEntries);
            m_ClassOccurences.insert(file.m_ClassOccurences.begin(), file.m_ClassOccurences.end());
            m_FunctionOccurences.insert(file.m_FunctionOccurences.begin(), file.m_FunctionOccurences.end());
            m_FunctionEntries.insert(file.m_FunctionEntries.begin(), file.m_FunctionEntries.end());
            m_FunctionExits.insert(file.m_FunctionExits.begin(), file.m_FunctionExits.end());
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return *this;
}

CLogFile::~CLogFile()
{
    CFuncTracer trace("CLogFile::~CLogFile", m_trace);
    try
    {
        m_logEntries.clear();
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
std::vector<CLogEntry> CLogFile::GetEntries(void)
{
    CFuncTracer trace("CLogFile::GetEntries", m_trace);
    try
    {
        return m_filteredEntries;
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return std::vector<CLogEntry>();
}
std::map<std::string, bool> CLogFile::GetFunctions(void)
{
    CFuncTracer trace("CLogFile::GetFunctions", m_trace);
    try
    {
        if (m_Functions.size() == 0)
        {
            std::for_each(m_filteredEntries.begin(), m_filteredEntries.end(), [=](CLogEntry& entry){
                std::string fullFunctionName = entry.Class() + "::" + entry.FuncName();
                if (m_Functions.find(fullFunctionName) == m_Functions.end())
                    m_Functions.insert(std::make_pair(fullFunctionName, true));
            });
            std::for_each(m_logEntries.begin(), m_logEntries.end(), [=](CLogEntry& entry){
                std::string fullFunctionName = entry.Class() + "::" + entry.FuncName();
                if (m_Functions.find(fullFunctionName) == m_Functions.end())
                    m_Functions.insert(std::make_pair(fullFunctionName, false));
            });
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return m_Functions;
}
std::map<std::string, bool> CLogFile::GetClasses(void)
{
    CFuncTracer trace("CLogFile::GetClasses", m_trace);
    try
    {
        if (m_Classes.size() == 0)
        {
            std::for_each(m_filteredEntries.begin(), m_filteredEntries.end(), [=](CLogEntry& entry){
                if (m_Classes.find(entry.Class()) == m_Classes.end())
                    m_Classes.insert(std::make_pair(entry.Class(), true));
            });
            std::for_each(m_logEntries.begin(), m_logEntries.end(), [=](CLogEntry& entry){
                if (m_Classes.find(entry.Class()) == m_Classes.end())
                    m_Classes.insert(std::make_pair(entry.Class(), false));
            });
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return m_Classes;
}
std::map<std::string, bool> CLogFile::GetTracelevels(void)
{
    CFuncTracer trace("CLogFile::GetTracelevels", m_trace);


    try
    {
        if (m_TraceLevels.size() == 0)
        {

            std::for_each(m_filteredEntries.begin(), m_filteredEntries.end(), [=](CLogEntry& entry){
                if (m_TraceLevels.find(entry.Level()) == m_TraceLevels.end())
                    m_TraceLevels.insert(std::make_pair(entry.Level(), true));
            });
            // Mark the tracelevels false if they do not occurs in the filter list
            std::for_each(m_logEntries.begin(), m_logEntries.end(), [=](CLogEntry& entry){
                if (m_TraceLevels.find(entry.Level()) == m_TraceLevels.end())
                    m_TraceLevels.insert(std::make_pair(entry.Level(), false));
            });
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return m_TraceLevels;
}

void CLogFile::SetTimeFilter(std::string startTime, std::string endTime)
{
    CFuncTracer trace("CLogFile::SetTimeFilter", m_trace);
    try
    {
        unsigned long long uiStartTime = CLogEntry::GetUiTime(startTime);
        unsigned long long uiEndTime = CLogEntry::GetUiTime(endTime);
        if (uiStartTime >= uiEndTime)
        {
            trace.Error("StartTime is bigger then end time");
            return;
        }

        m_filteredEntries.erase(std::remove_if(m_filteredEntries.begin(), m_filteredEntries.end(), [=, &uiStartTime, &uiEndTime](CLogEntry& entry){
                                    return (  (entry.GetTime() < uiStartTime)
                                            ||(entry.GetTime() > uiEndTime));
                               }), m_filteredEntries.end());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CLogFile::SetDescriptionFilter(std::vector<std::string> TextFilters, bool caseSensitive, bool wordOnly)
{
    CFuncTracer trace("CLogFile::SetDescriptionFilter", m_trace);
    try
    {
        std::vector<std::string> filter;
        for(std::string& s : TextFilters)
        {
            if (caseSensitive == false)
                transform(s.begin(), s.end(), s.begin(), ::tolower);

            if (wordOnly == true)
                s = " " + s + " ";
            filter.push_back(s);
        }
        m_filteredEntries.erase(std::remove_if(m_filteredEntries.begin(), m_filteredEntries.end(),[=, &caseSensitive](CLogEntry& entry){
                                    std::string sDesc = entry.Description();
                                    if (caseSensitive == false)
                                        transform(sDesc.begin(), sDesc.end(), sDesc.begin(), ::tolower);

                                    return !InverseContains<std::vector<std::string>>(filter, sDesc);
                                }), m_filteredEntries.end());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CLogFile::SetInverseDescriptionFilter(std::vector<std::string> TextFilters, bool caseSensitive, bool wordOnly)
{
    CFuncTracer trace("CLogFile::SetInverseDescriptionFilter", m_trace);
    try
    {
        std::vector<std::string> filter;
        for(std::string& s : TextFilters)
        {
            if (caseSensitive == false)
                transform(s.begin(), s.end(), s.begin(), ::tolower);

            if (wordOnly == true)
                s = " " + s + " ";
            filter.push_back(s);
        }
        m_filteredEntries.erase(std::remove_if(m_filteredEntries.begin(), m_filteredEntries.end(),[=, &caseSensitive](CLogEntry& entry){
                                    std::string sDesc = entry.Description();
                                    if (caseSensitive == false)
                                        transform(sDesc.begin(), sDesc.end(), sDesc.begin(), ::tolower);

                                    return InverseContains<std::vector<std::string>>(filter, sDesc);
                                }), m_filteredEntries.end());

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred: %s", ex.what());
    }
}
void CLogFile::SetLevelFilter(std::vector<TracerLevel> LevelFilters)
{
    CFuncTracer trace("CLogFile::SetLevelFilter", m_trace);
    try
    {
        m_filteredEntries.erase(std::remove_if(m_filteredEntries.begin(), m_filteredEntries.end(), [=](CLogEntry& entry){
                                    return Contains<TracerLevel>(LevelFilters, entry.GetLevel());
        }), m_filteredEntries.end());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CLogFile::SetThreadIdFilter(std::vector<int> ThreadIdFilters)
{
    CFuncTracer trace("CLogFile::SetThreadIdFilter", m_trace);
    try
    {
        m_filteredEntries.erase(std::remove_if(m_filteredEntries.begin(), m_filteredEntries.end(), [=](CLogEntry& entry){
                                    return !Contains<int>(ThreadIdFilters, entry.GetThreadId());
                                }), m_filteredEntries.end());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }

}
void CLogFile::SetProcIdFilter(std::vector<int> ProcIdFiters)
{
    CFuncTracer trace("CLogFile::SetProcIdFilter", m_trace);
    try
    {
        m_filteredEntries.erase(std::remove_if(m_filteredEntries.begin(), m_filteredEntries.end(), [=](CLogEntry& entry){
                                    return !Contains<int>(ProcIdFiters, entry.GetProcId());
        }), m_filteredEntries.end());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }

}
void CLogFile::SetClassNameFilter(std::vector<std::string> classes)
{
    CFuncTracer trace("CLogFile::SetClassNameFilter", m_trace);
    try
    {
        m_filteredEntries.erase(std::remove_if(m_filteredEntries.begin(), m_filteredEntries.end(), [=](CLogEntry& entry){
                                    return Contains<std::string>(classes, entry.Class());
                                }), m_filteredEntries.end());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CLogFile::SetFunctionFilter(std::vector<std::string> functions)
{
    CFuncTracer trace("CLogFile::SetFunctionFilter", m_trace);
    try
    {
        m_filteredEntries.erase(std::remove_if(m_filteredEntries.begin(), m_filteredEntries.end(), [=](CLogEntry& entry){
                                    return Contains<std::string>(functions, entry.FuncName());
                                }), m_filteredEntries.end());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }

}
void CLogFile::ClearFilter(void)
{
    CFuncTracer trace("CLogFile::ClearFilter", m_trace);
    try
    {
        m_filteredEntries.clear();
        std::copy(m_logEntries.begin(), m_logEntries.end(),
                  std::back_inserter(m_filteredEntries));
        m_Functions.clear();
        m_Classes.clear();
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }

}
bool CLogFile::Save(const char *filename)
{
    CFuncTracer trace("CLogFile::Save", m_trace);
    try
    {
        std::ofstream file(filename, std::fstream::out|std::fstream::trunc );
        std::for_each(m_filteredEntries.begin(), m_filteredEntries.end(), [=, &file](CLogEntry& entry){
            if (IsTimeAvailable())
                file << entry.Time() << " ";
            if (IsLevelAvailable())
                 file << entry.Level() << "\t";
            if (IsProcIdAvailable() || IsThreadIdAvailable())
                 file << "[" << std::to_string(entry.GetProcId()) << ":" << std::to_string(entry.GetThreadId()) << "]";
            file << "  -  ";
            if (IsFuncNameAvailable())
                 file << entry.Class() << "::" << entry.FuncName();
            file << " : ";
            if (IsDescriptionAvailable())
                file << entry.Description();
            file << std::endl;
        });
        file.flush();
        file.close();
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return false;
}
std::map<std::string, int> CLogFile::GetClassOccurences()
{
    CFuncTracer trace("CLogFile::GetClassOccurences", m_trace);
    try
    {
        if (m_ClassOccurences.size() > 0)
            return m_ClassOccurences;

        for_each(m_logEntries.begin(), m_logEntries.end(), [=](CLogEntry& entry){
            if (m_ClassOccurences.find(entry.Class()) == m_ClassOccurences.end())
                m_ClassOccurences.insert(std::make_pair(entry.Class(), 1));
            else
                m_ClassOccurences[entry.Class()] += 1;
        });
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return m_ClassOccurences;
}
std::map<std::string, int> CLogFile::GetFunctionOccurences()
{
    CFuncTracer trace("CLogFile::GetFunctionOccurences", m_trace);
    try
    {
        if (m_FunctionOccurences.size() > 0)
            return m_FunctionOccurences;

        for_each(m_logEntries.begin(), m_logEntries.end(), [=](CLogEntry& entry){
            std::string fullFunction = entry.Class() + "::" + entry.FuncName();
            if (m_FunctionOccurences.find(fullFunction) == m_FunctionOccurences.end())
                m_FunctionOccurences.insert(std::make_pair(fullFunction, 1));
            else
                m_FunctionOccurences[fullFunction] += 1;
        });
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return m_FunctionOccurences;
}
std::map<std::string, int> CLogFile::GetFunctionEntries()
{
    CFuncTracer trace("CLogFile::GetFunctionEntries", m_trace);
    try
    {
        if (m_FunctionEntries.size() > 0)
            return m_FunctionEntries;

        std::for_each(m_logEntries.begin(), m_logEntries.end(), [=](CLogEntry& entry){
            if (entry.Description().find("Entr") != std::string::npos)
            {
                std::string fullFunction = entry.Class() + "::" + entry.FuncName();
                if (m_FunctionEntries.find(fullFunction) == m_FunctionEntries.end())
                    m_FunctionEntries.insert(std::make_pair(fullFunction, 1));
                else
                    m_FunctionEntries[fullFunction] += 1;
            }
        });
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return m_FunctionEntries;
}
std::map<std::string, int> CLogFile::GetFunctionExits()
{
    CFuncTracer trace("CLogFile::GetFunctionExits", m_trace);
    try
    {
        if (m_FunctionExits.size() > 0)
            return m_FunctionExits;

        std::for_each(m_logEntries.begin(), m_logEntries.end(), [=](CLogEntry& entry){
            if (entry.Description().find("Exit") != std::string::npos)
            {
                std::string fullFunction = entry.Class() + "::" + entry.FuncName();
                if (m_FunctionExits.find(fullFunction) == m_FunctionExits.end())
                    m_FunctionExits.insert(std::make_pair(fullFunction, 1));
                else
                    m_FunctionExits[fullFunction] += 1;
            }
        });
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return m_FunctionExits;
}
void CLogFile::SetMark(long long id, bool bMark)
{
    CFuncTracer trace("CLogFile::SetMark", m_trace, false);
    try
    {
        int count = std::count_if(m_logEntries.begin(), m_logEntries.end(), [=, &id, &bMark](CLogEntry& entry){
            if (  (entry.GetID() == id)
                ||(id < 0))
            {
                entry.SetMark(bMark);
                return true;
            }
            return false;
        });
        trace.Trace("#%ld Marks changed in m_logEntries", count);
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void CLogFile::SetRequiredText(long long id, const std::string& text, bool bRequired )
{
    CFuncTracer trace("CLogFile::setRequiredText", m_trace);
    try
    {
        int count = std::count_if(m_logEntries.begin(), m_logEntries.end(), [=, &id, &text, &bRequired](CLogEntry& entry){
            if (  (entry.GetID() == id)
                ||(id < 0))
            {
                entry.SetSearchMark(bRequired, text);
                return true;
            }
            return false;
        });
        trace.Trace("#%ld required text (%s)", count, text.c_str());
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

// Private functions
void CLogFile::automaticDetectFormat(std::vector<std::string>& fields)
{
    CFuncTracer trace("CLogFile::parse", m_trace);
    try
    {
        int iFieldIdx = 0;
        fields = CConversion::split(m_sLine, ' ', fields, true);
        bool bFuncSeparatorFound = false;
        bool bDescriptionSeparatorFound = false;

        for (std::string sfield : fields)
        {
            // Remove spaces in the string
            sfield = CConversion::trim(sfield);

            if (  (sfield.length() == 1)
                &&(sfield ==  "-"))
            {
                bFuncSeparatorFound = true;
            }
            else if (  (sfield.length() == 1)
                     &&(sfield == ":"))
            {
                bDescriptionSeparatorFound = true;
            }
            else if (  (  (sfield.find("TRACE") != std::string::npos)
                        ||(sfield.find("INFO") != std::string::npos)
                        ||(sfield.find("WARNING") != std::string::npos)
                        ||(sfield.find("ERROR") != std::string::npos)
                        ||(sfield.find("FATAL") != std::string::npos))
                     &&(bDescriptionSeparatorFound == false) )
            {
                m_LevelIdx = iFieldIdx;
            }
            else if (  (sfield.length() == 12)
                     &&(sfield[2] == ':')
                     &&(sfield[5] == ':')
                     &&(sfield[8] == '.'))
            {
                m_TimeIdx = iFieldIdx;
            }
            else if (  (sfield.find(":") != std::string::npos)
                     &&(sfield[0] == '[')
                     &&(sfield[(sfield.length() - 1)] == ']'))
            {
                m_ProdIdIdx = iFieldIdx;
                m_ThreadIdIdx = iFieldIdx;
            }
            else if (bFuncSeparatorFound == true)
            {
                if (bDescriptionSeparatorFound == true)
                {
                    if (m_DescIdx == -1)
                    {
                        m_DescIdx = iFieldIdx;
                    }
                }
                else
                {
                    m_FuncIdx = iFieldIdx;
                    m_ClassNameIdx = iFieldIdx;
                }
            }

            iFieldIdx ++;
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
bool CLogFile::isTraceLevelValid(const char *level)
{
    std::vector<std::string> validTraceLevels ={"TRACE", "INFO", "WARNING", "ERROR", "FATAL_ERROR"};
    return Contains(validTraceLevels, level);
}
void CLogFile::parse(void)
{
    CFuncTracer trace("CLogFile::parse", m_trace);
    CScopeTimer timer("CLogFile::parse", 0, [=, &trace](const std::string& msg)
        {
            trace.Info("Timings : ");
            trace.Info("   %s", msg.c_str());
        });
    std::ifstream file(m_name);
    try
    {
        timer.SetTime("Start");
        std::vector<std::string> fields;
        m_logEntries.clear();
        timer.SetTime("Clear entries");
        while(std::getline(file, m_sLine))
        {
            if (m_sLine.length() == 0)
                continue;

            if (!m_bDetectedFormat)
            {
                automaticDetectFormat(fields);
            }
            else
            {
                // Split string and add the correct strings
                fields.clear();
                fields = CConversion::split(m_sLine, ' ', fields, true);
                for (std::string& sfield : fields)
                    sfield = CConversion::trim(sfield);
            }
            if (IsTimeAvailable())
            {
                if (static_cast<int>(fields.size()) > m_TimeIdx)
                {
                    m_sTime = fields[m_TimeIdx];
                }
                else
                    m_sTime = "";
            }
            if (IsLevelAvailable())
            {
                if (static_cast<int>(fields.size()) > m_LevelIdx)
                {
                    m_sLevel = fields[m_LevelIdx];
                }
                else
                    m_sLevel = "";
            }
            if (IsProcIdAvailable())
            {
                if (static_cast<int>(fields.size()) > m_ProdIdIdx)
                {
                    m_sProcId = fields[m_ProdIdIdx].substr(1, fields[m_ProdIdIdx].find(":") - 1);
                }
                else
                    m_sProcId = "";
            }
            if (IsThreadIdAvailable())
            {
                if (static_cast<int>(fields.size()) > m_ThreadIdIdx)
                {
                    m_sThreadId = fields[m_ThreadIdIdx].substr(fields[m_ProdIdIdx].find(":") + 1);
                    m_sThreadId = m_sThreadId.substr(0, m_sThreadId.find_last_of("]"));
                }
                else
                    m_sThreadId = "";
            }
            if (IsClassNameAvailable())
            {
                if (static_cast<int>(fields.size()) > m_ClassNameIdx)
                {
                    m_sClassname = fields[m_ClassNameIdx].substr(0, fields[m_ClassNameIdx].find_first_of(":"));
                    if (m_sClassname.length() > m_maxClassLength)
                        m_maxClassLength = m_sClassname.length();
                }
                else
                    m_sClassname = "";
            }
            if (IsFuncNameAvailable())
            {
                if (static_cast<int>(fields.size()) > m_FuncIdx)
                {
                    m_sFuncName = fields[m_FuncIdx].substr(fields[m_FuncIdx].find_last_of(":") + 1);
                    if (m_sFuncName.length() > m_maxFuncLength)
                        m_maxFuncLength = m_sFuncName.length();
                }
                else
                    m_sFuncName = "";
            }
            if (IsDescriptionAvailable())
            {
                if (static_cast<int>(fields.size()) > m_DescIdx)
                {
                    std::string s = " : ";
                    int start_pos = 0;
                    if (fields[m_DescIdx] == "-")
                    {
                        s = fields[m_DescIdx + 1];
                        m_sDescription = " - ";
                        start_pos = m_sLine.find(s);
                    }
                    else
                    {
                        s += fields[m_DescIdx];
                        m_sDescription = "";
                        start_pos = m_sLine.find(s) + 3;
                    }

                    m_sDescription += m_sLine.substr(start_pos);
                    if (m_sDescription.length() > m_maxDescLength)
                        m_maxDescLength = m_sDescription.length();
                }
                else
                    m_sDescription = "";
            }

            if (isTraceLevelValid(m_sLevel.c_str()))
            {
                m_bDetectedFormat = true;
                m_logEntries.push_back(CLogEntry(m_trace, m_sTime.c_str(), m_sLevel.c_str(), m_sProcId.c_str(),
                                                 m_sThreadId.c_str(), m_sClassname.c_str(), m_sFuncName.c_str(), m_sDescription.c_str()));
                m_filteredEntries.push_back(CLogEntry(m_trace, m_sTime.c_str(), m_sLevel.c_str(), m_sProcId.c_str(),
                                                      m_sThreadId.c_str(), m_sClassname.c_str(), m_sFuncName.c_str(), m_sDescription.c_str()));
            }
            else
            {
                if (! m_bDetectedFormat)
                {
                    fields.clear();
                    continue;
                }

                if (1)
                {
                    if (m_logEntries.size() > 0)
                    {
                        m_logEntries.push_back(CLogEntry(m_trace,
                                                         m_logEntries[m_logEntries.size() - 1].Time().c_str(),
                                                         m_logEntries[m_logEntries.size() - 1].Level().c_str(),
                                                         m_logEntries[m_logEntries.size() - 1].ProcID().c_str(),
                                                         m_logEntries[m_logEntries.size() - 1].ThreadID().c_str(),
                                                         m_logEntries[m_logEntries.size() - 1].Class().c_str(),
                                                         m_logEntries[m_logEntries.size() - 1].FuncName().c_str(),
                                                         m_sLine.c_str()));
                        m_filteredEntries.push_back(CLogEntry(m_trace,
                                                              m_logEntries[m_logEntries.size() - 1].Time().c_str(),
                                                              m_logEntries[m_logEntries.size() - 1].Level().c_str(),
                                                              m_logEntries[m_logEntries.size() - 1].ProcID().c_str(),
                                                              m_logEntries[m_logEntries.size() - 1].ThreadID().c_str(),
                                                              m_logEntries[m_logEntries.size() - 1].Class().c_str(),
                                                              m_logEntries[m_logEntries.size() - 1].FuncName().c_str(),
                                                              m_sLine.c_str()));
                    }
                }
                else
                {
                    m_logEntries[m_logEntries.size() - 1].AddDescription(m_sLine.c_str());
                    m_filteredEntries[m_filteredEntries.size() - 1].AddDescription(m_sLine.c_str());
                }
            }
        }
        timer.SetTime("End");
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
