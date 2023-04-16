#pragma once
#include "Logic/clogfile.h"
#include <QAbstractTableModel>
#include <QGridLayout>
#include <QTableView>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QInputDialog>
#include <QHeaderView>
#include <QColor>
#include <QMessageBox>

enum class eColumns
{
    eDateTime = 0,
    eTraceLevel = 1,
    eProc = 2,
    eThread = 3,
    eClass = 4,
    eFunction = 5,
    eDescription = 6,
    eNumOfColumns = 7
};

class QLogFileModel : public QAbstractTableModel
{
private:
    QList<CLogEntry> m_entries;
    std::shared_ptr<CTracer> m_trace;
public:
    QLogFileModel(std::shared_ptr<CTracer> tracer, QObject* parent = {})
        : QAbstractTableModel{parent}
        , m_trace(tracer)
    {
        CFuncTracer trace("QLogFileModel::QLogFileModel", m_trace);
    }
    int rowCount(const QModelIndex &) const override
    {
        CFuncTracer trace("QLogFileModel::rowCount", m_trace, false);
        trace.Trace("#rows : %ld", m_entries.count());
        return m_entries.count();
    }
    int columnCount(const QModelIndex &) const override
    {
        CFuncTracer trace("QLogFileModel::columnCount", m_trace);
        return static_cast<int>(eColumns::eNumOfColumns);
    }
    QVariant data(const QModelIndex &index, int role) const override
    {
        CFuncTracer trace("QLogFileModel::data", m_trace, false);
        const CLogEntry& entry = m_entries[index.row()];
        if (  (role == Qt::DisplayRole)
            ||(role == Qt::EditRole))
        {
            switch(index.column())
            {
                case (int)eColumns::eDateTime: return QString::fromStdString(entry.Time());
                case (int)eColumns::eTraceLevel: return QString::fromStdString(entry.Level());
                case (int)eColumns::eProc: return QString::fromStdString(std::to_string(entry.GetProcId()));
                case (int)eColumns::eThread: return QString::fromStdString(std::to_string(entry.GetThreadId()));
                case (int)eColumns::eClass: return QString::fromStdString(entry.Class());
                case (int)eColumns::eFunction: return QString::fromStdString(entry.FuncName());
                case (int)eColumns::eDescription: return QString::fromStdString(entry.Description());
                default: return {};
            }
        }

        if (role == Qt::FontRole)
        {
            if (entry.IsEntryRequired())
            {
                trace.Trace("Required text : %s", entry.GetRequiredText().c_str());
            }
        }

        if (role == Qt::BackgroundRole)
        {
            QColor backcolor;
            backcolor.setRgb(255,255,255);
            if (entry.IsMarked())
                backcolor.setRgb(212,235,242);

            QBrush background(backcolor);
            return background;
        }

        return {};
    }
    QVariant headerData(int section, Qt::Orientation /*orientation*/, int role) const override
    {
        CFuncTracer trace("QLogFileModel::headerData", m_trace);
        try
        {
            if ((role != Qt::DisplayRole) || (role != Qt::Horizontal)) return {};
            switch(section)
            {
                case (int)eColumns::eDateTime: return "Time";
                case (int)eColumns::eTraceLevel: return "level";
                case (int)eColumns::eProc: return "PID";
                case (int)eColumns::eThread: return "TID";
                case (int)eColumns::eClass: return "Class";
                case (int)eColumns::eFunction: return "Function";
                case (int)eColumns::eDescription: return "Description";
                default: return {};
            }
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
        return {};
    }
    void append(std::vector<CLogEntry> entries)
    {
        CFuncTracer trace("QLogFileModel::append", m_trace);
        try
        {
            beginInsertRows({}, m_entries.count(), m_entries.count() + entries.size());
            for(const CLogEntry& entry : entries)
                m_entries.append(entry);
            endInsertRows();
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
    }
    void clear()
    {
        CFuncTracer trace("QLogFileModel::clear", m_trace);
        try
        {
            m_entries.clear();
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
    }
    void rowToggleMark(const QModelIndex& index)
    {
        CFuncTracer trace("QLogFileModel::rowToggleMark", m_trace);
        try
        {
            if (index.row() >= m_entries.size())
            {
                trace.Error("Row does not exist inside the model! (index : %ld)", index);
                return;
            }

            if (m_entries[index.row()].IsMarked())
                m_entries[index.row()].SetMark( false );
            else
                m_entries[index.row()].SetMark( true );
            QModelIndex first = createIndex(index.row(), 0);
            QModelIndex last = createIndex(index.row(), (int)eColumns::eNumOfColumns);
            emit dataChanged(first, last);
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
    }
    std::optional<CLogEntry> getLogEntry(int index) const
    {
        CFuncTracer trace("QLogFileModel::getLogEntry", m_trace);
        CLogEntry value;
        try
        {
            trace.Trace("index : %d", index);
            if (index >= m_entries.size())
            {
                trace.Error("Row does not exist inside the model! (index : %ld)", index);
                return std::nullopt;
            }
            return m_entries[index];
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
        return std::nullopt;
    }
    int rowGetNextToggleMark(QModelIndex& currentIdx)
    {
        CFuncTracer trace("QLogFileModel::rowGetNextToggleMark", m_trace);
        try
        {
            if (currentIdx.row() < m_entries.size())
            {
                // From current position to the end of the file
                for (int idx = currentIdx.row() + 1; idx < m_entries.size(); idx++)
                {
                    if (m_entries[idx].IsMarked() == true)
                        return idx;
                }
                // From the beginning of the file until the current position
                for (int idx = 0; idx < currentIdx.row(); idx++)
                {
                    if (m_entries[idx].IsMarked() == true)
                        return idx;
                }
                trace.Error("No next mark found!");
            }
            else
                trace.Error("row is out of range: %d", currentIdx.row());
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
        return -1;
    }
    void HighlightText(int row, std::string& text, QColor HighLightColor, QColor BackgroundColor)
    {
        CFuncTracer trace("QLogFileModel::HighlightText", m_trace);
        try
        {

        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
    }
    QModelIndex CreateIndex(int row, int column)
    {
        CFuncTracer trace("QLogFileModel::CreateIndex", m_trace);
        return createIndex(row, column);
    }
};
class QLogFileWidget : public QWidget
{
public:
    explicit QLogFileWidget(std::shared_ptr<CTracer> tracer, std::vector<CLogEntry> entries, __attribute__((unused))QWidget *parent = nullptr)
        : m_tabindex(-1)
        , m_Layout(nullptr)
        , m_View(nullptr)
        , m_Model(nullptr)
        , m_trace(tracer)
    {
        CFuncTracer trace("QLogFileWidget::QLogFileWidget", tracer);
        try
        {
            init_createForm(entries);
            connect(m_View, &QTableView::clicked, this, &QLogFileWidget::row_click);
            connect(m_View, &QTableView::doubleClicked, this, &QLogFileWidget::row_double_click);
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
    }

    ~QLogFileWidget()
    {
        CFuncTracer trace("QLogFileModel~QLogFileModel", m_trace);
        try
        {
            if (m_Layout != nullptr)
            {
                delete m_Layout;
                m_Layout = nullptr;
            }
            if (m_View != nullptr)
            {
                delete m_View;
                m_View = nullptr;
            }

            if (m_Model != nullptr)
            {
                delete m_Model;
                m_Model = nullptr;
            }

        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred: %s", ex.what());
        }
    }

    QLogFileModel *GetModel(void)
    {
        CFuncTracer trace("QLogFileWidget::GetModel", m_trace);
        return m_Model;
    }
    void SetTabIndex(int index)
    {
        CFuncTracer trace("QLogFileWidget::setTabIndex", m_trace);
        m_tabindex = index;
    }
    int GetTabIndex(void)
    {
        CFuncTracer trace("QLogFileWidget::GetTabIndex", m_trace);
        return m_tabindex;
    }
    void SetFileName(std::string file)
    {
        CFuncTracer trace("QLogFileWidget::SetFileName", m_trace);
        m_FileName = file;
    }
    std::string GetFilename(void)
    {
        CFuncTracer trace("QLogFileWidget::GetFileName", m_trace);
        return m_FileName;
    }
    QModelIndex GetCurrentIndex(void)
    {
        CFuncTracer trace("QLogFileWidget::GetCurrentIndex", m_trace);
        return m_View->currentIndex();
    }
    void ToggleMark(QModelIndex index)
    {
        CFuncTracer trace("QLogFileWidget::ToggleMark", m_trace);
        m_Model->rowToggleMark(index);
    }
    void GotoToNextMark(void)
    {
        CFuncTracer trace("QLogFileWidget::GotoToNextMark", m_trace);
        try
        {
            // Find next mark
            QModelIndex index = m_View->currentIndex();
            int row = m_Model->rowGetNextToggleMark(index);
            if (row >= 0)
            {
                QModelIndex nextMark = m_Model->CreateIndex(row, 0);
                m_View->setCurrentIndex(nextMark);
            }
            else
            {
                QMessageBox::warning(this, "WARNING - No Marks", "No next marks were found",
                                     QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::NoButton);
                trace.Error("No next Mark found!");
            }
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occured : %s", ex.what());
        }
    }

private slots:
    void row_double_click(const QModelIndex &  index)
    {
        CFuncTracer trace("QLogFileWidget::row_double_click", m_trace);
        try
        {
            trace.Trace("index : %ld", index);

        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
    }
    void row_click(__attribute__((unused)) const QModelIndex &  index)
    {
        CFuncTracer trace("QLogFileWidget::row_click", m_trace);
        try
        {
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
    }

private:
    int m_tabindex;
    std::string m_FileName;
    QHBoxLayout *m_Layout;
    QTableView *m_View;
    QLogFileModel *m_Model;
    std::shared_ptr<CTracer> m_trace;

    void init_createForm(std::vector<CLogEntry> entries)
    {
        CFuncTracer trace("QLogFileWidget::init_createForm", m_trace);
        try
        {
            m_View = new QTableView(this);
            m_View->setSelectionBehavior( QAbstractItemView::SelectRows);
            m_Model = new QLogFileModel(m_trace, this);
            m_Layout = new QHBoxLayout(this);
            m_Layout->setContentsMargins(0,0,0,0);
            m_Layout->addWidget(m_View);

            m_Model->append(entries);
            m_View->setModel(m_Model);
            m_View->resizeColumnsToContents();
            m_View->setStyleSheet("QTableView:disabled{color:grey;}QTableView:enabled{color:black;font-weight:normal;}");
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
    }
};
