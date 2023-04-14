#include "logviewer.h"

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include <QDebug>
#include <QtGui>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include "cfunctracer.h"
#include "ctracer.h"
#include "cscopedtimer.h"
#include "QLogFileWidget.h"
#include "Logic/clogfile.h"
#include "inc/Logic/cconversion.h"


LogViewer::LogViewer(std::shared_ptr<CTracer> tracer, QWidget *parent)
    : m_hlayout(nullptr)
    , m_vlayout(nullptr)
    , menuBar(nullptr)
    , fileMenu(nullptr)
    , actOpen(nullptr)
    , actClose(nullptr)
    , actSave(nullptr)
    , tabWidget(nullptr)
    , gbxLevel(nullptr)
    , lblLevel(nullptr)
    , cboLevel(nullptr)
    , cbo_levelModel(nullptr)
    , btnLevelFiltering(nullptr)
    , gbxFunctionFiltering(nullptr)
    , cboClass(nullptr)
    , cbo_classModel(nullptr)
    , lblClass(nullptr)
    , lblFunction(nullptr)
    , cboFunction(nullptr)
    , cboFunctionModel(nullptr)
    , btnFunctionFilter(nullptr)
    , gbxFilter(nullptr)
    , cbxCaseSensitive(nullptr)
    , lblSearch(nullptr)
    , cbxInversesearch(nullptr)
    , btnFilter(nullptr)
    , btnClearFilter(nullptr)
    , lblStartTime(nullptr)
    , lblEndTime(nullptr)
    , dtEndTime(nullptr)
    , cbxWordOnly(nullptr)
    , cbxSearch(nullptr)
    , toggleMark(nullptr)
    , GotoLine(nullptr)
    , SearchText(nullptr)
    , m_trace(tracer)
    , m_bWordOnly(false)
{
    CFuncTracer trace("LogViewer::LogViewer", m_trace);
    try
    {
//        ui->setupUi(this);

        m_vlayout = new QVBoxLayout();
        m_hlayout = new QHBoxLayout();

        init_createActions();
        init_createMenus();

        init_createLevelFilteringGroupBox();
        init_createFunctionFilteringGroupBox();
        init_createFilterGroupBox();

        init_retranslateUi();
        init_styles();

        tabWidget = new QTabWidget(this);
        m_vlayout->addLayout(m_hlayout);
        m_vlayout->addWidget(tabWidget);

        setLayout(m_vlayout);

        connect(cbxWordOnly, &QCheckBox::stateChanged, this, &LogViewer::on_cbxWordOnly_stateChanged);
        connect(cbxCaseSensitive, &QCheckBox::stateChanged, this, &LogViewer::on_cbxCaseSensitive_stateChanged);
        connect(cbxInversesearch, &QCheckBox::stateChanged, this, &LogViewer::on_cbxInversesearch_stateChanged);
        connect(tabWidget, &QTabWidget::currentChanged, this, &LogViewer::onTabChanged);
        connect(btnLevelFiltering, &QPushButton::clicked, this, &LogViewer::onLevelFilter_clicked);
        connect(btnFunctionFilter, &QPushButton::clicked, this, &LogViewer::onFunctionFilter_clicked);
        connect(btnFilter, &QPushButton::clicked,this, &LogViewer::onSearchFilter_clicked);
        connect(btnClearFilter, &QPushButton::clicked, this, &LogViewer::onClearFilter_clicked);
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

LogViewer::~LogViewer()
{
    CFuncTracer trace("LogViewer::~LogViewer", m_trace);
    try
    {
        if (m_hlayout != nullptr)
        {
            delete m_hlayout;
            m_hlayout = nullptr;
        }
        if (m_vlayout != nullptr)
        {
            delete m_vlayout;
            m_vlayout = nullptr;
        }
        if (menuBar != nullptr)
        {
            delete menuBar;
            menuBar = nullptr;
        }
        if (fileMenu != nullptr)
        {
            delete fileMenu;
            fileMenu = nullptr;
        }
        if (actOpen != nullptr)
        {
            delete actOpen;
            actOpen = nullptr;
        }
        if (actClose != nullptr)
        {
            delete actClose;
            actClose = nullptr;
        }
        if (actSave != nullptr)
        {
            delete actSave;
            actSave = nullptr;
        }
        if (tabWidget != nullptr)
        {
            delete tabWidget;
            tabWidget = nullptr;
        }
        if (lblLevel != nullptr)
        {
            delete lblLevel;
            lblLevel = nullptr;
        }
        if (cboLevel != nullptr)
        {
            delete cboLevel;
            cboLevel = nullptr;
        }
        if (cbo_levelModel != nullptr)
        {
            delete cbo_levelModel;
            cbo_levelModel = nullptr;
        }
        if (btnLevelFiltering != nullptr)
        {
            delete btnLevelFiltering;
            btnLevelFiltering = nullptr;
        }
        if (gbxLevel != nullptr)
        {
            delete gbxLevel;
            gbxLevel = nullptr;
        }
        if (cboClass != nullptr)
        {
            delete cboClass;
            cboClass = nullptr;
        }
        if (cbo_classModel != nullptr)
        {
            delete cbo_classModel;
            cbo_classModel = nullptr;
        }
        if (lblClass != nullptr)
        {
            delete lblClass;
            lblClass = nullptr;
        }
        if (lblFunction != nullptr)
        {
            delete lblFunction;
            lblFunction = nullptr;
        }
        if (cboFunction != nullptr)
        {
            delete cboFunction;
            cboFunction = nullptr;
        }
        if (cboFunctionModel != nullptr)
        {
            delete cboFunctionModel;
            cboFunctionModel = nullptr;
        }
        if (btnFunctionFilter != nullptr)
        {
            delete btnFunctionFilter;
            btnFunctionFilter = nullptr;
        }
        if (gbxFunctionFiltering != nullptr)
        {
            delete gbxFunctionFiltering;
            gbxFunctionFiltering = nullptr;
        }
        if (cbxCaseSensitive != nullptr)
        {
            delete cbxCaseSensitive;
            cbxCaseSensitive = nullptr;
        }
        if (lblSearch != nullptr)
        {
            delete lblSearch;
            lblSearch = nullptr;
        }
        if (cbxInversesearch != nullptr)
        {
            delete cbxInversesearch;
            cbxInversesearch = nullptr;
        }
        if (btnFilter != nullptr)
        {
            delete btnFilter;
            btnFilter = nullptr;
        }
        if (btnClearFilter != nullptr)
        {
            delete btnClearFilter;
            btnClearFilter = nullptr;
        }
        if (lblStartTime != nullptr)
        {
            delete lblStartTime;
            lblStartTime = nullptr;
        }
        if (lblEndTime != nullptr)
        {
            delete lblEndTime;
            lblEndTime = nullptr;
        }
        if (dtEndTime != nullptr)
        {
            delete dtEndTime;
            dtEndTime = nullptr;
        }
        if (cbxWordOnly != nullptr)
        {
            delete cbxWordOnly;
            cbxWordOnly = nullptr;
        }
        if (cbxSearch != nullptr)
        {
            delete cbxSearch;
            cbxSearch = nullptr;
        }
        if (toggleMark != nullptr)
        {
            delete toggleMark;
            toggleMark = nullptr;
        }
        if (GotoLine != nullptr)
        {
            delete GotoLine;
            GotoLine = nullptr;
        }
        if (SearchText != nullptr)
        {
            delete SearchText;
            SearchText = nullptr;
        }
        if (gbxFilter != nullptr)
        {
            delete gbxFilter;
            gbxFilter = nullptr;
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void LogViewer::init_styles(void)
{
    CFuncTracer trace("LogViewer::init_styles", m_trace);
    try
    {
        gbxFilter->setStyleSheet("QGroupBox:disabled{color:grey;}QGroupBox:enabled{color: black;font-weight:bold;}");
        gbxFunctionFiltering->setStyleSheet("QGroupBox:disabled{color:grey;}QGroupBox:enabled{color: black;font-weight:bold;}");
        gbxLevel->setStyleSheet("QGroupBox:disabled{color:grey;}QGroupBox:enabled{color: black;font-weight:bold;}");
        cbxWordOnly->setStyleSheet("QCheckBox:disabled{ color: grey; }QCheckBox:enabled{color: black;font-weight:bold;}");
        cbxCaseSensitive->setStyleSheet("QCheckBox:disabled{ color: grey; }QCheckBox:enabled{color: black;font-weight:bold;}");
        cbxInversesearch->setStyleSheet("QCheckBox:disabled{ color: grey; }QCheckBox:enabled{color: black;font-weight:bold;}");
        lblClass->setStyleSheet("QLabel:disabled{color:grey;}QLabel:enabled{color:black;font-weight:bold;}");
        lblEndTime->setStyleSheet("QLabel:disabled{color:grey;}QLabel:enabled{color:black;font-weight:bold;}");
        lblFunction->setStyleSheet("QLabel:disabled{color:grey;}QLabel:enabled{color:black;font-weight:bold;}");
        lblLevel->setStyleSheet("QLabel:disabled{color:grey;}QLabel:enabled{color:black;font-weight:bold;}");
        lblSearch->setStyleSheet("QLabel:disabled{color:grey;}QLabel:enabled{color:black;font-weight:bold;}");
        lblStartTime->setStyleSheet("QLabel:disabled{color:grey;}QLabel:enabled{color:black;font-weight:bold;}");
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void LogViewer::init_createActions(void)
{
    CFuncTracer trace("LogViewer::init_createActions", m_trace);
    try
    {
        actOpen = new QAction(tr("&Open"), this);
        actOpen->setShortcut(QKeySequence::Open);
        actOpen->setStatusTip(tr("Opens a log file or analyse"));
        connect(actOpen, &QAction::triggered, this, &LogViewer::open);

        actClose = new QAction(tr("&Close"), this);
        actClose->setShortcut(QKeySequence::Close);
        actClose->setStatusTip(tr("Closes the log file"));
        connect(actClose, &QAction::triggered, this, &LogViewer::close);

        actSave = new QAction(tr("&Save"), this);
        actSave->setShortcut(QKeySequence::Save);
        actSave->setStatusTip(tr("Save the filtered log file"));
        connect(actSave, &QAction::triggered, this, &LogViewer::save);
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::init_createMenus(void)
{
    CFuncTracer trace("LogViewer::init_createMenus", m_trace);
    try
    {
        menuBar = new QMenuBar();
        fileMenu = new QMenu(tr("&File"));
        menuBar->addMenu(fileMenu);
        fileMenu->addAction(actOpen);
        fileMenu->addAction(actClose);
        fileMenu->addAction(actSave);
        m_vlayout->setMenuBar(menuBar);
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::init_createFilterGroupBox(void)
{
    CFuncTracer trace("LogViewer::init_createFilterGroupBox", m_trace);
    try
    {
        gbxFilter = new QGroupBox(this);
        gbxFilter->setObjectName("gbxFilter");
        gbxFilter->setEnabled(true);
        gbxFilter->setGeometry(QRect(970, 10, 901, 141));
        gbxFilter->setMinimumSize(901,141);
        cbxCaseSensitive = new QCheckBox(gbxFilter);
        cbxCaseSensitive->setObjectName("cbxCaseSensitive");
        cbxCaseSensitive->setGeometry(QRect(150, 40, 131, 23));
        lblSearch = new QLabel(gbxFilter);
        lblSearch->setObjectName("lblSearch");
        lblSearch->setGeometry(QRect(10, 80, 51, 25));
        cbxInversesearch = new QCheckBox(gbxFilter);
        cbxInversesearch->setObjectName("cbxInversesearch");
        cbxInversesearch->setGeometry(QRect(280, 40, 131, 23));
        btnFilter = new QPushButton(gbxFilter);
        btnFilter->setObjectName("btnFilter");
        btnFilter->setGeometry(QRect(690, 110, 89, 25));
        btnClearFilter = new QPushButton(gbxFilter);
        btnClearFilter->setObjectName("btnClearFilter");
        btnClearFilter->setGeometry(QRect(790, 110, 89, 25));
        lblStartTime = new QLabel(gbxFilter);
        lblStartTime->setObjectName("lblStartTime");
        lblStartTime->setGeometry(QRect(440, 40, 81, 26));
        lblEndTime = new QLabel(gbxFilter);
        lblEndTime->setObjectName("lblEndTime");
        lblEndTime->setGeometry(QRect(680, 40, 71, 26));
        dtEndTime = new QTimeEdit(gbxFilter);
        dtEndTime->setObjectName("dtEndTime");
        dtEndTime->setGeometry(QRect(760, 40, 118, 26));
        cbxWordOnly = new QCheckBox(gbxFilter);
        cbxWordOnly->setObjectName("cbxWordOnly");
        cbxWordOnly->setEnabled(true);
        cbxWordOnly->setGeometry(QRect(10, 40, 131, 23));
        cbxWordOnly->setAutoFillBackground(true);
        cbxSearch = new QComboBox(gbxFilter);
        cbxSearch->setObjectName("cbxSearch");
        cbxSearch->setGeometry(QRect(80, 80, 771, 25));
        cbxSearch->setEditable(true);
        m_hlayout->addWidget(gbxFilter);
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::init_createFunctionFilteringGroupBox(void)
{
    CFuncTracer trace("LogViewer::init_createFunctionFilteringGroupBox", m_trace);
    try
    {
        gbxFunctionFiltering = new QGroupBox(this);
        gbxFunctionFiltering->setObjectName("gbxFunctionFiltering");
        gbxFunctionFiltering->setEnabled(true);
        gbxFunctionFiltering->setGeometry(QRect(320, 10, 641, 141));
        gbxFunctionFiltering->setMinimumSize(641,141);
        cboClass = new QComboBox(gbxFunctionFiltering);
        cboClass->setObjectName("cboClass");
        cboClass->setGeometry(QRect(110, 40, 501, 25));
        cbo_classModel = new QCheckableModel(m_trace);
        cboClass->setModel(cbo_classModel);
        lblClass = new QLabel(gbxFunctionFiltering);
        lblClass->setObjectName("lblClass");
        lblClass->setGeometry(QRect(40, 50, 67, 25));
        lblFunction = new QLabel(gbxFunctionFiltering);
        lblFunction->setObjectName("lblFunction");
        lblFunction->setGeometry(QRect(40, 80, 67, 25));
        cboFunction = new QComboBox(gbxFunctionFiltering);
        cboFunction->setObjectName("cboFunction");
        cboFunction->setGeometry(QRect(110, 70, 501, 25));
        cboFunctionModel = new QCheckableModel(m_trace);
        cboFunction->setModel(cboFunctionModel);
        btnFunctionFilter = new QPushButton(gbxFunctionFiltering);
        btnFunctionFilter->setObjectName("btnFunctionFilter");
        btnFunctionFilter->setGeometry(QRect(470, 110, 141, 25));
        m_hlayout->addWidget(gbxFunctionFiltering);
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::init_createLevelFilteringGroupBox(void)
{
    CFuncTracer trace("LogViewer::init_createLevelFilteringGroupBox", m_trace);
    try
    {
        gbxLevel = new QGroupBox(this);
        gbxLevel->setObjectName("gbxLevel");
        gbxLevel->setEnabled(true);
        gbxLevel->setGeometry(QRect(20, 10, 291, 141));
        gbxLevel->setMinimumSize(291,141);
        QFont font1;
        font1.setBold(false);
        gbxLevel->setFont(font1);
        gbxLevel->setInputMethodHints(Qt::ImhNone);
        lblLevel = new QLabel(gbxLevel);
        lblLevel->setObjectName("lblLevel");
        lblLevel->setGeometry(QRect(10, 60, 67, 25));
        cboLevel = new QComboBox(gbxLevel);
        cboLevel->setObjectName("cboLevel");
        cboLevel->setGeometry(QRect(70, 60, 201, 25));
        cbo_levelModel = new QCheckableModel(m_trace);
        cboLevel->setModel(cbo_levelModel);
        btnLevelFiltering = new QPushButton(gbxLevel);
        btnLevelFiltering->setObjectName("btnLevelFiltering");
        btnLevelFiltering->setGeometry(QRect(130, 100, 141, 25));
        m_hlayout->addWidget(gbxLevel);
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::init_createShortCut(void)
{
    CFuncTracer trace("LogViewer::init_createShortCut", m_trace);
    try
    {
        QKeySequence togglekey(Qt::CTRL + Qt::Key_M);
        QKeySequence searchkey(Qt::CTRL + Qt::Key_F);
        QKeySequence gotoLine(Qt::Key_F3);

        toggleMark = new QShortcut(togglekey, this);
        GotoLine = new QShortcut(gotoLine, this);
        SearchText= new QShortcut(searchkey, this);

        connect(toggleMark, &QShortcut::activated, this, &LogViewer::on_toggle_mark);
        connect(GotoLine, &QShortcut::activated, this, &LogViewer::on_goto_next_mark);
        connect(SearchText, &QShortcut::activated, this, &LogViewer::on_search_text);
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::init_retranslateUi(void)
{
    CFuncTracer trace("LogViewer::init_retranslateUi", m_trace);
    try
    {
        //LogViewer->setWindowTitle(QCoreApplication::translate("LogViewer", "LogViewer", nullptr));
        gbxFilter->setTitle(QCoreApplication::translate("LogViewer", "Text Filtering", nullptr));
        cbxCaseSensitive->setText(QCoreApplication::translate("LogViewer", "Case Sensitive", nullptr));
        lblSearch->setText(QCoreApplication::translate("LogViewer", "Search", nullptr));
        cbxInversesearch->setText(QCoreApplication::translate("LogViewer", "Inverse search", nullptr));
        btnFilter->setText(QCoreApplication::translate("LogViewer", "Filter", nullptr));
        btnClearFilter->setText(QCoreApplication::translate("LogViewer", "Clear Filter", nullptr));
        lblStartTime->setText(QCoreApplication::translate("LogViewer", "Start time", nullptr));
        lblEndTime->setText(QCoreApplication::translate("LogViewer", "End time", nullptr));
        dtEndTime->setDisplayFormat(QCoreApplication::translate("LogViewer", "HH:mm:ss", nullptr));
        cbxWordOnly->setText(QCoreApplication::translate("LogViewer", "Word Only", nullptr));

        gbxFunctionFiltering->setTitle(QCoreApplication::translate("LogViewer", "Function Filtering", nullptr));
        lblClass->setText(QCoreApplication::translate("LogViewer", "Class", nullptr));
        lblFunction->setText(QCoreApplication::translate("LogViewer", "Function", nullptr));
        btnFunctionFilter->setText(QCoreApplication::translate("LogViewer", "Function Filter", nullptr));

        gbxLevel->setTitle(QCoreApplication::translate("LogViewer", "Level Filtering", nullptr));
        lblLevel->setText(QCoreApplication::translate("LogViewer", "Level", nullptr));
        btnLevelFiltering->setText(QCoreApplication::translate("LogViewer", "Level Filter", nullptr));

    /*
        btnLoadFile->setText(QCoreApplication::translate("LogViewer", "Load", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("LogViewer", "toolBar", nullptr));*/

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void LogViewer::update_classFilter(void)
{
    CFuncTracer trace("LogViewer::update_classFilter", m_trace);
    try
    {
        if (m_currentLogFile.get() != nullptr)
        {
            std::map<std::string, bool> classes = m_currentLogFile->GetClasses();
            // clear combo box
            cboClass->clear();
            // Add to combo box
            cbo_classModel->append(classes);
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void LogViewer::update_functionFilter(void)
{
    CFuncTracer trace("LogViewer::update_functionFilter", m_trace);
    try
    {
        if (m_currentLogFile.get() != nullptr)
        {
            std::map<std::string, bool> functions = m_currentLogFile->GetFunctions();

            // Clear combo box
            cboFunction->clear();
            // Add to combo box
            cboFunctionModel->append(functions);
        }
        else
            trace.Error("m_currentLogFile = nullptr");

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void LogViewer::update_traceLevels(void)
{
    CFuncTracer trace("LogViewer::update_traceLevels", m_trace);
    try
    {
        if (m_currentLogFile.get() != nullptr)
        {
            std::map<std::string, bool> tracelevels = m_currentLogFile->GetTracelevels();

            //clear combo box
            cboLevel->clear();
            //Add to combo box
            cbo_levelModel->append(tracelevels);
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void LogViewer::update_current_tab(void)
{
    CFuncTracer trace("LogViewer::update_current_tab", m_trace);
    try
    {
        QLogFileWidget *current_tab = dynamic_cast<QLogFileWidget*>(tabWidget->widget(m_currentTabIdx));
        std::vector<CLogEntry> lines = m_currentLogFile->GetEntries();
        trace.Trace("Lines : %ld", lines.size());
        current_tab->GetModel()->clear();
        current_tab->GetModel()->append(lines);
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void LogViewer::on_cbxWordOnly_stateChanged(int /*arg1*/)
{
    CFuncTracer trace("LogViewer::on_cbxWordOnly_stateChanged", m_trace, false);
    try
    {
        trace.Trace("state : %s", (cbxWordOnly->checkState() == Qt::Checked)? "CHECKED" : "NOT CHECKED");
        if (cbxWordOnly->checkState() == Qt::Checked)
        {
            m_bWordOnly = true;
        }
        else
        {
            m_bWordOnly = false;
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::on_cbxCaseSensitive_stateChanged(int /*arg1*/)
{
    CFuncTracer trace("LogViewer::on_cbxCaseSensitive_stateChanged", m_trace, false);
    try
    {
        trace.Trace("state : %s", (cbxCaseSensitive->checkState() == Qt::Checked)? "CHECKED" : "NOT CHECKED");

        if (cbxCaseSensitive->checkState() == Qt::Checked)
            m_bCaseSensitive = true;
        else
            m_bCaseSensitive = false;
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::on_cbxInversesearch_stateChanged(int /*arg1*/)
{
    CFuncTracer trace("LogViewer::on_cbxInversesearch_stateChanged", m_trace, false);
    try
    {
        trace.Trace("state : %s", (cbxInversesearch->checkState() == Qt::Checked)? "CHECKED" : "NOT CHECKED");
        if (cbxInversesearch->checkState() == Qt::Checked)
        {
            cbxSearch->setStyleSheet("QComboBox:enabled{color:red;font-weight:bold;}");
            m_bInverseSearch = true;
        }
        else
        {
            cbxSearch->setStyleSheet("QComboBox:enabled{color:black;font-weight:bold;}");
            m_bInverseSearch = false;
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::onFunctionFilter_clicked(void)
{
    CFuncTracer trace("LogViewer::onFunctionFilter_clicked", m_trace);
    try
    {
        std::vector<std::string> unselectedFunctions = cboFunctionModel->GetUnselectedItems();
        std::vector<std::string> unselectedClasses = cbo_classModel->GetUnselectedItems();

        m_currentLogFile->ClearFilter();
        m_currentLogFile->SetClassNameFilter(unselectedClasses);
        m_currentLogFile->SetFunctionFilter(unselectedFunctions);

        update_current_tab();

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void LogViewer::onLevelFilter_clicked(void)
{
    CFuncTracer trace("LogViewer::onLevelFilter_clicked", m_trace);
    try
    {
        std::vector<std::string> unselectedTracelvls = cbo_levelModel->GetUnselectedItems();

        m_filteredLevels.clear();
        std::for_each(unselectedTracelvls.begin(), unselectedTracelvls.end(), [=](const std::string& lvl){
            m_filteredLevels.push_back(CLogEntry::Convert(lvl));
        });

        m_currentLogFile->ClearFilter();
        m_currentLogFile->SetLevelFilter(m_filteredLevels);

        // Update GUI
        update_current_tab();
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void LogViewer::onSearchFilter_clicked(void)
{
    CFuncTracer trace("LogViewer::onSearchFilter_clicked", m_trace);
    try
    {
        std::vector<std::string> textfilters;
        std::string combotext = cbxSearch->currentText().toStdString();
        textfilters = CConversion::split(combotext, ',', textfilters, true);

        m_currentLogFile->ClearFilter();
        if (!m_bInverseSearch)
            m_currentLogFile->SetDescriptionFilter(textfilters, m_bCaseSensitive, m_bWordOnly);
        else
            m_currentLogFile->SetInverseDescriptionFilter(textfilters, m_bCaseSensitive, m_bWordOnly);


        cbxSearch->addItem(cbxSearch->currentText());

        // Update the GUI
        update_current_tab();
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void LogViewer::onProcThreadIdFilter_clicked(void)
{
    CFuncTracer trace("LogViewer::onProcThreadIdFilter_clicked", m_trace);
    try
    {

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void LogViewer::onClearFilter_clicked(void)
{
    CFuncTracer trace("LogViewer::onClearFilter_clicked", m_trace);
    try
    {
        m_currentLogFile->ClearFilter();
        // update the GUI
        update_current_tab();
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred: %s", ex.what());
    }
}
void LogViewer::onTabChanged(int index)
{
    CFuncTracer trace("LogViewer::onTabChanged", m_trace);
    try
    {
        trace.Info("index : %ld", index);
        m_currentTabIdx = index;
        QLogFileWidget *current_tab = dynamic_cast<QLogFileWidget*>(tabWidget->widget(index));
        if (current_tab != nullptr)
        {
            auto it = m_mpLogFiles.find(current_tab->GetFilename());
            if (it != m_mpLogFiles.end())
            {
                m_currentLogFile = it->second;
                update_classFilter();
                update_functionFilter();
                update_traceLevels();
            }
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::on_toggle_mark(void)
{
    CFuncTracer trace("LogViewer::on_toggle_mark", m_trace);
    try
    {
        QLogFileWidget *current_tab = dynamic_cast<QLogFileWidget*>(tabWidget->widget(m_currentTabIdx));
        if (current_tab != nullptr)
            current_tab->ToggleMark(current_tab->GetCurrentIndex());
        else
            trace.Warning("Current tab is not found");
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::on_goto_next_mark(void)
{
    CFuncTracer trace("LogViewer::on_goto_next_mark", m_trace);
    try
    {
        QLogFileWidget *current_tab = dynamic_cast<QLogFileWidget*>(tabWidget->widget(m_currentTabIdx));
        if (current_tab != nullptr)
            current_tab->GotoToNextMark();
        else
            trace.Warning("Current tab is not found");
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::on_search_text(void)
{
    CFuncTracer trace("LogViewer::on_search_text", m_trace);
    try
    {

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void LogViewer::open()
{
    CFuncTracer trace("LogViewer::open", m_trace);
    CScopeTimer timer("LogViewer::open", 0, [=, &trace](const std::string& msg)
                      {
                          trace.Info("Timings : ");
                          trace.Info("   %s", msg.c_str());
                      });
    try
    {
        QString filename = QFileDialog::getOpenFileName(this, "Open log file", QDir::currentPath(),"All files (*.*) ;; Log files (*.log)");
        if (!filename.isNull())
        {
            trace.Trace("Filename : %s" , filename.toStdString().c_str());
            std::shared_ptr<CLogFile> file = std::make_shared<CLogFile>(filename.toStdString().c_str(), m_trace);
            std::vector<CLogEntry> lines = file->GetEntries();
            trace.Trace("Lines : %ld", lines.size());
            QWidget *viewer = new QLogFileWidget(m_trace, lines, this);

            if (m_mpLogFiles.find(file->Name()) == m_mpLogFiles.end())
            {
                m_mpLogFiles.insert(std::make_pair(file->Name(), file));
                if (m_mpLogFiles.size() == 1)
                {
                    m_currentLogFile = file;
                    m_currentTabIdx = 0;
                }
            }
            ((QLogFileWidget*)viewer)->SetTabIndex(tabWidget->indexOf(viewer));
            ((QLogFileWidget*)viewer)->SetFileName(filename.toStdString());
            tabWidget->addTab(viewer, filename);
        }
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::save()
{
    CFuncTracer trace("LogViewer::save", m_trace);
    try
    {
/*        QString filename = QFileDialog::getSaveFileName(this, "Save log file", QDir::currentPath(),"");
        if (!filename.isNull())
        {
            trace.Trace("Filename : %s", filename.toStdString().c_str());
            QLogFileWidget *QLogFile = dynamic_cast<QLogFileWidget *>(tabWidget[m_currentTabIdx]);
            if (QLogFile != nullptr)
            {
                auto itFile = m_mpLogFiles.find(QLogFile->GetFilename());
                if (itFile != m_mpLogFiles.end())
                {
                    // Get filtered entries
                    std::vector<CLogEntry> entries = itFile->GetEntries();
                    std::string m_saveFileName = filename.toStdString();
                    std::string file_extension = m_saveFileName.substr(m_saveFileName.find_last_of(".") + 1);
                    std::transform(file_extension.begin(), file_extension.end(), file_extension.begin(), ::lowercase);
                    if (file_extension == "xml")
                    {
                        saveXmlBegin();
                    }

                    std::for_each(entries.begin(), entries.end(), [=, &file_extension](const CLogEntry& entry){
                        switch(file_extension)
                        {
                            case "csv": saveCsvEntry(); break;
                            case "json": saveJsonEntry(); break;
                            case "xml": saveXmlEntry(); break;
                            case "log":
                            case "txt":
                                saveTextEntry();
                                break;

                            default:
                                break;
                        }
                    });
                }
            }
        }*/
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::close()
{
    CFuncTracer trace("LogViewer::close", m_trace);
    try
    {
/*        QLogFileWidget *QLogFile = dynamic_cast<QLogFileWidget *>(tabWidget[m_currentTabIdx]);
        if (QLogFile != nullptr)
        {
            auto itFile = m_mpLogFiles.find(m_currentLogFile->Name());
            if (itFile != m_mpLogFiles.end())
            {
                if (QLogFile->GetFilename() == m_currentLogFile->Name())
                {
                    tabWidget->removeTab(QLogFile->GetTabIndex());
                    m_mpLogFiles.erase(itFile);
                }
                else
                    trace.Error("QLogfile :%s is not equal to current log file : %s", QLogFile->GetFilename().c_str(), m_currentLogFile->Name().c_str());
            }
            else
                trace.Error("Currentlogfile %s is not found inside the map", m_currentLogFile->Name());
        }
        else
            trace.Error("QLogFile is not of the correct type");*/
    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void LogViewer::saveXmlBegin(void)
{
    CFuncTracer trace("",m_trace);
    try
    {

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
    }
}
void LogViewer::saveCsvEntry(void)
{
    CFuncTracer trace("",m_trace);
    try
    {

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
    }
}
void LogViewer::saveJsonEntry(void)
{
    CFuncTracer trace("",m_trace);
    try
    {

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
    }
}
void LogViewer::saveXmlEntry(void)
{
    CFuncTracer trace("",m_trace);
    try
    {

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
    }
}
void LogViewer::saveTextEntry(void)
{
    CFuncTracer trace("",m_trace);
    try
    {

    }
    catch(std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
    }
}
