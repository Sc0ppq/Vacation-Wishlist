#pragma once
#include <QAbstractTableModel>
#include <QBrush>
#include <QColor>
#include <vector>
#include "Oferta.h"

class OfferTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    OfferTableModel(QObject* parent = nullptr);

    void setOffers(const std::vector<Oferta>& offers);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    std::vector<Oferta> m_offers;
};
