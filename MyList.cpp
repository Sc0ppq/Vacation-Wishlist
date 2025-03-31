#include "MyList.h"

ListModel::ListModel(QObject* parent) : QAbstractListModel(parent) {
}

int ListModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_items.size();
}

QVariant ListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_items.size() || role != Qt::DisplayRole)
        return QVariant();
    return m_items.at(index.row());
}

void ListModel::addItem(const QString& item) {
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(item);
    endInsertRows();
}

void ListModel::updateOffers(const std::vector<Oferta>& oferte) {
    beginResetModel();
    m_items.clear();
    for (const auto& oferta : oferte) {
        QString formattedOffer = QString("Denumire: %1 | Destinatie: %2 | Tip: %3 | Pret: %4")
            .arg(QString::fromStdString(oferta.getDenumire()))
            .arg(QString::fromStdString(oferta.getDestinatie()))
            .arg(QString::fromStdString(oferta.getTip()))
            .arg(oferta.getPret());
        m_items.append(formattedOffer);
    }
    endResetModel();
}
