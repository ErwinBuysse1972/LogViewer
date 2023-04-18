#pragma once
#include <QStandardItemModel>

#include <vector>
#include <string>
#include "cfunctracer.h"
#include "ctracer.h"

class QCheckableModel : public QStandardItemModel
{
public:
    explicit QCheckableModel(std::shared_ptr<CTracer> tracer, QObject *parent = nullptr)
        : QStandardItemModel(parent)
        , m_trace(tracer)
    {
        CFuncTracer trace("QCheckableModel::QCheckableModel", m_trace);
    }

    ~QCheckableModel()
    {
        std::for_each(m_stdItems.begin(), m_stdItems.end(), [=](std::pair<int, QStandardItem*> p){
            if (p.second)
            {
                delete p.second;
                p.second = nullptr;
            }
        });
        m_stdItems.clear();
    }
    int rowCount(const QModelIndex &) const override
    {
        CFuncTracer trace("QCheckableModel::rowCount", m_trace, false);
        trace.Trace("#rowcount : %ld", m_stdItems.size());
        return m_stdItems.size();
    }
    int columnCount(const QModelIndex &) const override
    {
        CFuncTracer trace("QCheckableModel::columnCount", m_trace, false);
        trace.Trace("count : %ld", 1);
        return 1;
    }
    int GetIndex(std::string sText)
    {
        CFuncTracer trace("QCheckableModel::GetIndex", m_trace);
        int index = -1;
        try
        {
            for_each(m_stdItems.begin(), m_stdItems.end(), [=, &index, &sText](std::pair<int, QStandardItem*> p){
                if (p.second->text().toStdString() == sText)
                    index = p.first;
            });
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
        return index;
    }
    std::vector<std::string> GetSelectedItems(void)
    {
        CFuncTracer trace("QCheckableModel::GetSelectedItems", m_trace);
        std::vector<std::string> selectedItems;
        try
        {
            std::for_each(m_stdItems.begin(), m_stdItems.end(), [=,&selectedItems](std::pair<int, QStandardItem*> p){
                if (p.second->data(Qt::CheckStateRole) == Qt::Checked)
                    selectedItems.push_back(p.second->text().toStdString());
            });
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
        return selectedItems;
    }
    std::vector<std::string> GetUnselectedItems(void)
    {
        CFuncTracer trace("QCheckableModel::GetUnselectedItems", m_trace);
        std::vector<std::string> UnselectedItems;
        try
        {
            std::for_each(m_stdItems.begin(), m_stdItems.end(), [=,&UnselectedItems](std::pair<int, QStandardItem*> p){

                if (p.second->data(Qt::CheckStateRole) == Qt::Unchecked)
                    UnselectedItems.push_back(p.second->text().toStdString());
            });
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred : %s", ex.what());
        }
        return UnselectedItems;
    }
    bool IsItemChecked(std::string sText)
    {
        CFuncTracer trace("QCheckableModel::IsItemChecked", m_trace);
        try
        {
            trace.Trace("text : %s", sText.c_str());
            int index = GetIndex(sText);
            if (m_stdItems.find(index)!= m_stdItems.end())
            {
                if (m_stdItems[index]->data(Qt::CheckStateRole) == Qt::Checked)
                    return true;
            }
        }
        catch(std::exception& ex)
        {
            trace.Error("Exception occurred :%s", ex.what());
        }
        return false;
    }
    void append(std::map<std::string, bool> elements)
    {
        CFuncTracer trace("QCheckableModel::columnCount", m_trace);
        try
        {
            int index = 0;
            QStandardItemModel::beginInsertRows({}, m_stdItems.size(), m_stdItems.size() + elements.size());
            for (std::pair<std::string, bool> p : elements)
            {

                QStandardItem *item = new QStandardItem(QString::fromStdString(p.first));
                item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                if (p.second == true)
                    item->setData(Qt::Checked, Qt::CheckStateRole);
                else
                    item->setData(Qt::Unchecked, Qt::CheckStateRole);
                setItem(index, 0, item);
                m_stdItems.insert(std::make_pair(index, item));
                index++;
            }
            this->endInsertRows();
        }
        catch(std::exception& ex)
        {
        trace.Error("Exception occurred : %s", ex.what());
        }
    }
private:
    // int is the index
    std::map<int, QStandardItem*> m_stdItems;
    std::shared_ptr<CTracer> m_trace;
};
