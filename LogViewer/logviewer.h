#pragma once
#include <QMainWindow>
#include <QAbstractTableModel>
#include <QGridLayout>
#include <QTableView>
#include <QPushButton>
#include <QGroupBox>
#include <QSortFilterProxyModel>
#include <QInputDialog>
#include <QTabWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QTimeEdit>
#include <QShortcut>
#include "QCheckableComboBox.h"

#include "cfunctracer.h"
#include "ctracer.h"
#include "Logic/clogfile.h"


class LogViewer : public QWidget
{
public:
    LogViewer(std::shared_ptr<CTracer> tracer, QWidget *parent = nullptr);
    ~LogViewer();

public slots:
//    void addTask();

private slots:
    void on_cbxWordOnly_stateChanged(int arg1);
    void on_cbxCaseSensitive_stateChanged(int arg1);
    void on_cbxInversesearch_stateChanged(int arg1);

    void open();
    void save();
    void close();

    void onFunctionFilter_clicked(void);
    void onLevelFilter_clicked(void);
    void onSearchFilter_clicked(void);
    void onProcThreadIdFilter_clicked(void);
    void onClearFilter_clicked(void);

    void onTabChanged(int index);
    void on_toggle_mark(void);
    void on_goto_next_mark(void);
    void on_search_text(void);

private:
    QHBoxLayout *m_hlayout;
    QVBoxLayout *m_vlayout;

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QAction *actOpen;
    QAction *actClose;
    QAction *actSave;
    QTabWidget *tabWidget;

    QGroupBox *gbxLevel;
    QLabel *lblLevel;
    QComboBox *cboLevel;
    QCheckableModel *cbo_levelModel;
    QPushButton *btnLevelFiltering;

    QGroupBox *gbxFunctionFiltering;
    QComboBox *cboClass;
    QCheckableModel *cbo_classModel;
    QLabel *lblClass;
    QLabel *lblFunction;
    QComboBox *cboFunction;
    QCheckableModel *cboFunctionModel;
    QPushButton *btnFunctionFilter;

    QGroupBox *gbxFilter;
    QCheckBox *cbxCaseSensitive;
    QLabel *lblSearch;
    QCheckBox *cbxInversesearch;
    QPushButton *btnFilter;
    QPushButton *btnClearFilter;
    QLabel *lblStartTime;
    QLabel *lblEndTime;
    QTimeEdit *dtEndTime;
    QCheckBox *cbxWordOnly;
    QComboBox *cbxSearch;

    QShortcut *keyCtrlM; // key used for toggle marks
    QShortcut *keyF3;   // key used for goto next mark
    QShortcut *keyCtrlF; // key used for searching


    std::shared_ptr<CTracer> m_trace;
    int m_currentTabIdx;
    std::shared_ptr<CLogFile> m_currentLogFile;
    std::map<std::string, std::shared_ptr<CLogFile>> m_mpLogFiles;
    std::vector<TracerLevel> m_filteredLevels;
    bool m_bWordOnly;
    bool m_bCaseSensitive;
    bool m_bInverseSearch;

    void init_styles(void);
    void init_createActions(void);
    void init_createMenus(void);
    void init_createFilterGroupBox(void);
    void init_createFunctionFilteringGroupBox(void);
    void init_createLevelFilteringGroupBox(void);
    void init_createShortCut(void);
    void init_retranslateUi(void);

    void update_classFilter(void);
    void update_functionFilter(void);
    void update_traceLevels(void);
    void update_current_tab(void);

    void saveXmlBegin(void);
    void saveCsvEntry(void);
    void saveJsonEntry(void);
    void saveXmlEntry(void);
    void saveTextEntry(void);
};
