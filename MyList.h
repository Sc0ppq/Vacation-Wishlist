#pragma once
#include <QAbstractListModel>
#include <QStringList>
#include <vector>
#include "Oferta.h" 
class ListModel : public QAbstractListModel {
    Q_OBJECT

public:
    ListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void addItem(const QString& item);

    void updateOffers(const std::vector<Oferta>& oferte);

private:
    QStringList m_items;
};

