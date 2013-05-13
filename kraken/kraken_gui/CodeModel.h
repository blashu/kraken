#ifndef __H_CODE_MODEL__
#define __H_CODE_MODEL__

#include <memory>
#include <boost/bimap.hpp>
#include <QStringListModel>

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
    typedef bimap<int, codeItemLocation_t> listingMap;
    typedef listingMap::value_type codeItemLocationValue_t;

    std::shared_ptr<CodeListingInterface> _codeListing;
    listingMap _listingToModelMap;

    void addItemToStringListAndMap(QStringList& list, QString& item, codeItemLocation_t itemLocation);
    QStringList getProgramListing();
    CodeBlockItemInterface* getCodeItemByModelIndex(const QModelIndex& index) const;
    QModelIndex getModelIndexByCodeItemLocation(const codeItemLocation_t& codeItemLocation) const;
};

#endif
