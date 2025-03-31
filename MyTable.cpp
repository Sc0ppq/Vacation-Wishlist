#include "MyTable.h"
#include <QBrush>
#include <QColor>
#include "Oferta.h" // Include the header file for the Oferta class

OfferTableModel::OfferTableModel(QObject* parent)
    : QAbstractTableModel(parent) {
}

void OfferTableModel::setOffers(const std::vector<Oferta>& offers) {
    beginResetModel();
    m_offers = offers;
    endResetModel();
}

int OfferTableModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_offers.size());
}

int OfferTableModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return 5;  // Number of columns: NR, DENUMIRE, DESTINATIE, TIP, PRET
}

QVariant OfferTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::BackgroundRole) {
        switch (index.row() % 3) {
        case 0: return QBrush{ QColor{173, 216, 230} }; // #ADD8E6 Light Blue
        case 1: return QBrush{ QColor{144, 238, 144} }; // #90EE90 Light Green
        case 2: return QBrush{ QColor{240, 128, 128} };  // #F08080 Light Coral
        default: return QVariant();
        }
    }

    if (role == Qt::DisplayRole) {
        const Oferta& offer = m_offers.at(index.row());
        switch (index.column()) {
        case 0: return index.row() + 1;  // NR
        case 1: return QString::fromStdString(offer.getDenumire());  // DENUMIRE
        case 2: return QString::fromStdString(offer.getDestinatie());  // DESTINATIE
        case 3: return QString::fromStdString(offer.getTip());  // TIP
        case 4: return offer.getPret();  // PRET
        default: return QVariant();
        }
    }

    return QVariant();
}

QVariant OfferTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "NR";
        case 1: return "DENUMIRE";
        case 2: return "DESTINATIE";
        case 3: return "TIP";
        case 4: return "PRET";
        default: return QVariant();
        }
    }
    return QVariant();
}
