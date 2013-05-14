#ifndef __H_CODE_MODEL__
#define __H_CODE_MODEL__

#include <memory>
#include <boost/bimap.hpp>
#include <QStringListModel>
#include <kraken/Decompiler.h>

#include <kraken/CodeListingInterface.h>

using namespace kraken;
using namespace boost::bimaps;

class CodeModel : public QStringListModel
{
    Q_OBJECT

  public:
    CodeModel() {}
    CodeModel(QString string, QObject* pobj);

    void showProgramListing();
    int getCodeBlockCount() const;
    int getCodeBlockItemCount() const;    

    bool isCodeItemBranchType(const QModelIndex& index) const;
    QModelIndex getFirstReferencedCodeItemIndex(const QModelIndex& index) const;

  private:
    typedef bimap<int, code_item_location_t> listingMap;
    typedef listingMap::value_type codeItemLocationValue_t;

    Decompiler _decompiler;
    CodeListingInterface* _codeListing;
    listingMap _listingToModelMap;

    void addItemToStringListAndMap(QStringList& list, QString& item, code_item_location_t itemLocation);
    QStringList getProgramListing();
    CodeBlockItemInterface* getCodeItemByModelIndex(const QModelIndex& index) const;
    QModelIndex getModelIndexByCodeItemLocation(const code_item_location_t& codeItemLocation) const;
};

#endif
